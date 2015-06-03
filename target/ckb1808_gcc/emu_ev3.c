#include <kernel.h>
#include <t_syslog.h>
#include <stdarg.h>
#include <errno.h>

#undef errno
int errno;

void* _sbrk(int nbytes) {
    void* ptr = (char*)tlsf_malloc(nbytes);
    if(!ptr) {
        syslog(LOG_ERROR, "[_sbrk] Memory allocation failed.");
        errno = ENOMEM;
        return (void*)(-1);
    } else {
        return ptr;
    }
}

void bluetooth_spp_putchar(uint8_t c) {
    syslog(LOG_WARNING, "[emu] Fake bluetooth_spp_putchar() is called.");
}
