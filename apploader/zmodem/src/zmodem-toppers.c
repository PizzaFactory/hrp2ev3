#define HAS_STDINT_H

#include <kernel.h>
#include <itron.h>
#include <t_syslog.h>
#include <stdio.h>
#include "gen_defs.h"

#include "syssvc/serial.h"
#include "target_serial.h"

#define MAX_PATH (255)

typedef UB BYTE;
//typedef UB uchar;

static ulong getfreediskspace(const char* path, ulong unit) {
	return (-1); // TODO: use a meaningful value
}

/****************************************************************************/
/* Return the filename portion of a full pathname                           */
/****************************************************************************/
static char* getfname(const char* path)
{
    const char* fname;
    const char* bslash;

    fname=strrchr(path,'/');
    bslash=strrchr(path,'\\');
    if(bslash>fname)
        fname=bslash;
    if(fname!=NULL)
        fname++;
    else
        fname=(char*)path;
    return((char*)fname);
}

#include "zmodem.c"

static int lputs(void* unused, int level, const char* str) {
	return printf("%s\n", str);
}

/*************************/
/* Send a byte to remote */
/*************************/
static int send_byte(void* unused, uchar ch, unsigned timeout) {
	int len = 1;

	ER_UINT erlen = serial_wri_dat(SIO_PORT_BT, &ch, len);
	assert(erlen > 0);

	if(erlen==len) {
//		if(debug_tx)
//			lprintf(LOG_DEBUG,"TX: %s",chr(ch));
		return(0);
	}

	return(-1);
}

/****************************************************************************/
/* Receive a byte from remote (single-threaded version)						*/
/****************************************************************************/
static int recv_byte(void* unused, unsigned timeout /* seconds */)
{
	// TODO: read

	uint8_t buf;

	ER_UINT erlen = serial_rea_dat(SIO_PORT_BT, &buf, sizeof(buf));
	assert(erlen > 0);

	if(erlen == sizeof(buf))
		return buf;

	return(NOINP);
}

/**
 * Memory file cookie
 */

struct memfile_cookie {
    char   *buf;        /* Dynamically sized buffer for data */
    size_t  allocated;  /* Size of buf */
    size_t  endpos;     /* Number of characters in buf */
    off_t   offset;     /* Current file offset in buf */
};

ssize_t
memfile_write(void *c, const char *buf, size_t size)
{
    char *new_buff;
    struct memfile_cookie *cookie = c;

    /* Buffer too small? Keep doubling size until big enough */

    if (size + cookie->offset > cookie->allocated) {
    	return -1;
    }

    memcpy(cookie->buf + cookie->offset, buf, size);

    cookie->offset += size;
    if (cookie->offset > cookie->endpos)
        cookie->endpos = cookie->offset;

    return size;
}

ssize_t
memfile_read(void *c, char *buf, size_t size)
{
    ssize_t xbytes;
    struct memfile_cookie *cookie = c;

    /* Fetch minimum of bytes requested and bytes available */

    xbytes = size;
    if (cookie->offset + size > cookie->endpos)
        xbytes = cookie->endpos - cookie->offset;
    if (xbytes < 0)     /* offset may be past endpos */
       xbytes = 0;

    memcpy(buf, cookie->buf + cookie->offset, xbytes);

    cookie->offset += xbytes;
    return xbytes;
}

int
memfile_seek(void *c, off_t *offset, int whence)
{
    off_t new_offset;
    struct memfile_cookie *cookie = c;

    if (whence == SEEK_SET)
        new_offset = *offset;
    else if (whence == SEEK_END)
        new_offset = cookie->endpos + *offset;
    else if (whence == SEEK_CUR)
        new_offset = cookie->offset + *offset;
    else
        return -1;

    if (new_offset < 0)
        return -1;

    cookie->offset = new_offset;
    *offset = new_offset;
    return 0;
}

int
memfile_close(void *c)
{
    struct memfile_cookie *cookie = c;

    cookie->allocated = 0;
    cookie->buf = NULL;

    return 0;
}




static zmodem_t zm;

ER wait_file_sender() {
	// Initialize zm
	zmodem_init(&zm,NULL,NULL,NULL,send_byte,recv_byte,NULL,NULL,NULL,NULL);
    //	zm.block_size = zm.max_block_size = 8192;

	// Waiting for ZRQINIT
	while(1) {
		int type = zmodem_recv_header_raw(&zm, 0);
		if(type != ZRQINIT) {
			syslog(LOG_ERROR,"Received %d instead of ZRQINIT", type);
		} else break;
	}

	return E_OK;
}

// Not reentrant
ER zmodem_recv_file(void *buf, SIZE size) {
	char	fname[MAX_PATH+1];
	int file_bytes, ftime, total_files, total_bytes;
	BOOL	success=FALSE;
	int i;

	i=zmodem_recv_init(&zm);

	if(zm.cancelled)
		return(1);
	if(i<0)
		return(-1);
	switch(i) {
		case ZFILE:
			SAFECOPY(fname,zm.current_file_name);
			file_bytes = zm.current_file_size;
			ftime = zm.current_file_time;
			total_files = zm.files_remaining;
			total_bytes = zm.bytes_remaining;
			lprintf(&zm, LOG_DEBUG,"Incoming filename: %.64s ",fname);
			break;
		case ZFIN:
		case ZCOMPL:
			return(!success);
		default:
			return(-1);
	}

    cookie_io_functions_t  memfile_func = {
        .read  = memfile_read,
        .write = memfile_write,
        .seek  = memfile_seek,
        .close = memfile_close
    };

    FILE *fp;

    /* Set up the cookie before calling fopencookie() */
    struct memfile_cookie mycookie;
    mycookie.buf = buf;
    mycookie.allocated = size;
    mycookie.offset = 0;
    mycookie.endpos = 0;

    fp = fopencookie(&mycookie,"w+", memfile_func);

	int errors=zmodem_recv_file_data(&zm,fp,0);

	i=zmodem_recv_init(&zm);
	assert(i == ZFIN || i == ZCOMPL);

	printf("ZMODEM last header: 0x%x\n", i);

	printf("ZMODEM final errors: %d\n", errors);

	printf("ZMODEM file endpos: %d\n", mycookie.endpos);

	return E_OK;
}
