#include <kernel.h>
#include "csl.h"
#include "chip_timer.h"
#include "syssvc/syslog.h"
#include "syssvc/banner.h"
#include "target_serial.h"
#include "syssvc/serial.h"
#include "syssvc/logtask.h"
#include "api.cfg.h"
#include "app.h"
#include "platform_interface_layer.h"


ID _ev3api_id_CYC_PRD_TSK_1;

ID _ev3api_id_CYC_PRD_TSK_2;

void _initialize_ev3api_cyc() {
	ER_ID ercd;
	T_CCYC pk_ccyc;

	pk_ccyc.cycatr = TA_STA;
	pk_ccyc.exinf = PRD_TSK_1;
	pk_ccyc.cychdr = task_activator;
	pk_ccyc.cyctim = PERIOD_PRD_TSK_1;
	pk_ccyc.cycphs = 0;
	ercd = _ev3_acre_cyc(&pk_ccyc);
	assert(ercd > 0);
	_ev3api_id_CYC_PRD_TSK_1 = ercd;

	pk_ccyc.cycatr = TA_NULL;
	pk_ccyc.exinf = PRD_TSK_2;
	pk_ccyc.cychdr = task_activator;
	pk_ccyc.cyctim = PERIOD_PRD_TSK_2;
	pk_ccyc.cycphs = 0;
	ercd = _ev3_acre_cyc(&pk_ccyc);
	assert(ercd > 0);
	_ev3api_id_CYC_PRD_TSK_2 = ercd;
}
