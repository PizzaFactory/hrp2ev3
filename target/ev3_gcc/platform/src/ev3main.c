/*
 * ev3main.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: liyixiao
 */

#include "gpio_dri.h"
#include "motor_dri.h"
#include "uart_dri.h"
#include "analog_dri.h"
#include "ev3api.h"

#include <kernel.h>
#include <t_stdlib.h>
#include "syssvc/syslog.h"
#include "syssvc/serial.h"
#include "target_syssvc.h"
#include "target_serial.h"
#include "kernel_cfg.h"
#include "platform.h"

#define TMAX_DRI_NUM (16)
static ev3_driver_t drivers[TMAX_DRI_NUM];
static uint32_t tnum_drivers = 0;

static bool_t is_initialized;

void ev3_main_task(intptr_t exinf)
{
    ER_UINT ercd;

    is_initialized = false;

    SVC_PERROR(serial_opn_por(SIO_PORT_BT));
    SVC_PERROR(serial_ctl_por(SIO_PORT_BT, (IOCTL_CRLF)));

//    syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

    ercd = serial_opn_por(TASK_PORTID);
    if (ercd < 0 && MERCD(ercd) != E_OBJ) {
        syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
                                    itron_strerror(ercd), SERCD(ercd));
    }
    SVC_PERROR(serial_ctl_por(TASK_PORTID,
                            (IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));


    /**
     * Initialize all drivers
     */
	for(uint32_t i = 0; i < tnum_drivers; ++i)
		if (drivers[i].init_func != NULL) drivers[i].init_func(0);

	platform_soft_reset();

	syslog(LOG_NOTICE, "Platform initialization is completed.");

	is_initialized = true;

#if 0 // Legacy code
    initialize_analog_dri();
    initialize_uart_dri();
	initialize_motor_dri();
	initialize_sound_dri();
	initialize_fatfs_dri();
	//    initialize_ev3();



//    syslog(LOG_ERROR, "TEST ZMODEM");
//    uint8_t c;
//    while(1) {
//    	serial_rea_dat(SIO_PORT_UART, &c, 1);
//		ER ercd;
//		switch (c) {
//		case 'r':
//			ercd = zmodem_recv_file(app_text_mempool, sizeof(app_text_mempool));
//			syslog(LOG_ERROR, "ZMODEM ercd = %d.", ercd);
//			break;
//		default:
//			syslog(LOG_ERROR, "Key %c pressed.", c);
//		}
//    }


    //EV3::ev3 = new EV3::EV3();

//#define TEST_ANALOG_SENSOR
//

//#ifdef TEST_ANALOG_SENSOR

//    while(1) {
//        /*
//         *  Debug Analog Port 2
//         */
//        int sv = analog_get_short(1);
//        printk("Sensor PIN1: %d\n", sv);
//        printk("Sensor PIN6: %d\n", analog_get_short_pin6(1));
//
//        GPIO67.OUT_DATA ^= GPIO_ED_PIN7 | GPIO_ED_PIN14;
//        target_fput_log('H');
//        target_fput_log('E');
//        target_fput_log('R');
//        target_fput_log('E');
//        target_fput_log('\n');
//        tslp_tsk(1000);
//    }
//#endif
//#ifdef TEST_UART
//  //init_pwm();
//  //ev3_motor_set_speed(PortA, 50);
//  //tslp_tsk(3000);
//  //ev3_motor_brake(PortA, true);
//  init_uart();
//  while(1) {
//      /*
//       *  Debug UART Port 2
//       */
//      int sv = uart_get_short(1);
//      printk("Sensor value: %d\n", sv);
//
//      GPIO67.OUT_DATA ^= GPIO_ED_PIN7 | GPIO_ED_PIN14;
//      target_fput_log('H');
//      target_fput_log('E');
//      target_fput_log('R');
//      target_fput_log('E');
//      target_fput_log('\n');
//      tslp_tsk(1000);
//  }
//#endif
#endif

}

ER platform_register_driver(const ev3_driver_t *p_driver) {
	if (tnum_drivers < TMAX_DRI_NUM) {
		drivers[tnum_drivers++] = *p_driver;
		return E_OK;
	} else {
		syslog(LOG_ERROR, "%s(): Too many device drivers", __FUNCTION__);
		return E_NOID;
	}
}

/**
 * Note: This function can only be called when the application is stopped.
 */
ER platform_soft_reset() {
	/**
	 * Soft reset
	 */
	for(uint32_t i = 0; i < tnum_drivers; ++i)
		if (drivers[i].softreset_func != NULL) drivers[i].softreset_func(0);

	return E_OK;
}

bool_t platform_is_ready() {
	return is_initialized;
}

void
svc_perror(const char *file, int_t line, const char *expr, ER ercd) {
    if (ercd < 0) {
        t_perror(LOG_ERROR, file, line, expr, ercd);
    }
}

#if 0 // Legacy code

/**
 * This task should be activated when the platform is busy,
 * which can pause all user tasks.
 */
void
platform_busy_task(intptr_t exinf) {
//	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_DEBUG), LOG_UPTO(LOG_EMERG)));
	while(1);
}


#endif
