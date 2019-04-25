/**
 * EV3 Platform (Core Services Layer)
 */

typedef struct ev3_driver_information {
	ISR init_func;
	ISR softreset_func;
} ev3_driver_t;

extern ER platform_register_driver(const ev3_driver_t *p_driver);

ER platform_soft_reset();
bool_t platform_is_ready();

/**
 * Tasks
 */
extern void ev3_main_task(intptr_t exinf);
extern void platform_busy_task(intptr_t exinf);

/**
 * Exceptions
 */
extern void ev3_prefetch_handler(void *p_excinf);
extern void ev3_data_abort_handler(void *p_excinf);

