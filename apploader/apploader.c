#include <kernel.h>
#include "ev3api.h"
#include "dmloader.h"
#include "button_event_trigger.h"
#include "kernel_cfg.h"

typedef enum {
	STATUS_IDLE,
	STATUS_LOADING,
	STATUS_FINISHED,
	STATUS_RUNNING,
} STATUS;

static STATUS status;

static STK_T app_binary_buf[COUNT_STK_T(TMAX_APP_BINARY_SIZE)]  __attribute__((nocommon));;

static STK_T app_text_mempool[COUNT_STK_T(TMAX_APP_TEXT_SIZE)] __attribute__((section(".app_text_mempool"),nocommon));
static STK_T app_data_mempool[COUNT_STK_T(TMAX_APP_DATA_SIZE)] __attribute__((section(".app_data_mempool"),nocommon));

static void button_event_handler(intptr_t button);

static bool_t handle_btn_clicked = false;

static void kernel_evt_hdr(const T_EVTINF *evtinf) {
	static SYSTIM back_press_time;

    switch(evtinf->evtcd) {
    case EVTCD_BUTTON_CLICKED:
        assert(evtinf->arg[0] >= 0 || evtinf->arg[0] < TNUM_BUTTON);
        if(handle_btn_clicked)
        	button_event_handler(evtinf->arg[0]);
        break;

    case EVTCD_BUTTON_PRESSED:
    	assert(evtinf->arg[0] >= 0 || evtinf->arg[0] < TNUM_BUTTON);
    	if(evtinf->arg[0] == BACK_BUTTON && status == STATUS_RUNNING)
    		get_tim(&back_press_time);
    	break;

    case EVTCD_BUTTON_RELEASED:
    	assert(evtinf->arg[0] >= 0 || evtinf->arg[0] < TNUM_BUTTON);
    	if(evtinf->arg[0] == BACK_BUTTON && status == STATUS_RUNNING) {
    		SYSTIM now;
    		get_tim(&now);
    		if(now - back_press_time >= 500)
    			sig_sem(APP_TER_SEM);
    	}
    	break;

    default:
        assert(0);
    }
}

void app_download_notifier(intptr_t exinf) {
	static uint32_t ledcolor = LED_OFF;

	switch(status) {
	case STATUS_IDLE:
		if(ledcolor != LED_OFF && ledcolor != LED_ORANGE)
			ledcolor = LED_OFF;
		ledcolor ^= LED_ORANGE;
		ev3_set_led(ledcolor);
		ista_alm(APP_DL_NOTIFIER, 500);
		break;
	case STATUS_LOADING:
		ledcolor = LED_ORANGE;
		break;
	case STATUS_FINISHED:
		if(ledcolor != LED_OFF && ledcolor != LED_GREEN)
			ledcolor = LED_OFF;
		ledcolor ^= LED_GREEN;
		ev3_set_led(ledcolor);
		ista_alm(APP_DL_NOTIFIER, 500);
		break;
	default:
		assert(NULL);
	}
}

static void chg_status(STATUS new_status) {
	syslog(LOG_NOTICE, "chg_status to %d", new_status);
	// Disable notifier and button handlers
//	stp_alm(APP_DL_NOTIFIER);
	handle_btn_clicked = false;
//	ev3_set_on_button_clicked(ENTER_BUTTON, NULL, NULL);
//	ev3_set_on_button_clicked(BACK_BUTTON, NULL, NULL);

	status = new_status;
	switch(status) {
	case STATUS_IDLE:
		ter_tsk(APP_DL_TSK);
		sta_alm(APP_DL_NOTIFIER, 0);
		act_tsk(APP_DL_TSK);
		break;

	case STATUS_LOADING:
		sta_alm(APP_DL_NOTIFIER, 0);
		break;

	case STATUS_FINISHED:
		sta_alm(APP_DL_NOTIFIER, 0);
		break;

	case STATUS_RUNNING:
		stp_alm(APP_DL_NOTIFIER);
		sig_sem(APP_DL_FINISHED_SEM);
		break;

	default:
		assert(NULL);
	}

	// Enable notifier and button handlers
//	sta_alm(APP_DL_NOTIFIER, 0);
	handle_btn_clicked = true;
//	ev3_set_on_button_clicked(ENTER_BUTTON, button_event_handler, ENTER_BUTTON);
//	ev3_set_on_button_clicked(BACK_BUTTON, button_event_handler, BACK_BUTTON);
}

static void button_event_handler(intptr_t button) {
	switch(status) {
	case STATUS_IDLE:
		switch(button) {
//		case ENTER_BUTTON:
//			chg_status(STATUS_LOADING);
//			break;
		case BACK_BUTTON:
			ev3_power_off();
			break;
		default:
			assert(NULL);
		}
		break;

	case STATUS_LOADING:
		switch(button) {
		case ENTER_BUTTON:
			/* TODO: Do nothing or check finished */
			break;
		case BACK_BUTTON:
			chg_status(STATUS_IDLE);
			break;
		default:
			assert(NULL);
		}
		break;

	case STATUS_FINISHED:
		switch (button) {
		case ENTER_BUTTON:
		    chg_status(STATUS_RUNNING);
			break;
		case BACK_BUTTON:
			chg_status(STATUS_IDLE);
			break;
		default:
			assert(NULL);
		}
		break;

	default:
		assert(NULL);
	}
}

ER run_app_downloader(const void **app_data) {
	chg_status(STATUS_IDLE);
	ER ercd = wai_sem(APP_DL_FINISHED_SEM);
	*app_data = app_binary_buf;
	stp_alm(APP_DL_NOTIFIER);
	return ercd;
}

void apploader_task(intptr_t unused) {
	real_kernel_evt_hdr = kernel_evt_hdr;
#if 0
	// Test loader
    T_LDM_CAN ldm_can;
    ldm_can.domid = TDOM_APP;
    ldm_can.text_mempool = app_text_mempool;
    ldm_can.text_mempool_size = ROUND_STK_T(TMAX_APP_TEXT_SIZE);
    ldm_can.data_mempool = app_data_mempool;
    ldm_can.data_mempool_size = ROUND_STK_T(TMAX_APP_DATA_SIZE);
    while(1) {
    	dmloader_ins_ldm(NULL, &ldm_can);
    	tslp_tsk(5000);
    	dmloader_rmv_ldm(&ldm_can);
    }
#endif
#if 0
    ercd = run_app_downloader(NULL);
#endif
    ev3_motors_init(NONE_MOTOR, NONE_MOTOR, NONE_MOTOR, NONE_MOTOR);

    T_LDM_CAN ldm_can;
    ldm_can.domid = TDOM_APP;
    ldm_can.text_mempool = app_text_mempool;
    ldm_can.text_mempool_size = ROUND_STK_T(TMAX_APP_TEXT_SIZE);
    ldm_can.data_mempool = app_data_mempool;
    ldm_can.data_mempool_size = ROUND_STK_T(TMAX_APP_DATA_SIZE);

    while(1) {
    	const void *app_data;
    	ER ercd = run_app_downloader(&app_data);
    	assert(ercd == E_OK);
    	handle_btn_clicked = false;
    	ev3_set_led(LED_GREEN);
    	dmloader_ins_ldm(app_data, &ldm_can);
    	wai_sem(APP_TER_SEM);
    	dmloader_rmv_ldm(&ldm_can);
        for(MotorPort i = PortA; i <= PortD; i++)
            ev3_motor_config(i, NONE_MOTOR);
    }
}

void app_download_task(intptr_t exinf) {
	ER ercd;

	syslog(LOG_NOTICE, "Waiting for application sender (ZMODEM)...");
	ercd = wait_file_sender();
	assert(ercd == E_OK);

	chg_status(STATUS_LOADING);
    ercd = zmodem_recv_file(app_binary_buf, sizeof(app_binary_buf));
    assert(ercd == E_OK);

    syslog(LOG_NOTICE, "Received the application successfully.");
    chg_status(STATUS_FINISHED);
}
