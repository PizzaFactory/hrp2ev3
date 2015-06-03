/**
 * CKB-1808用ドライバ定義
 * (Emulate 'ev3.h')
 */

#ifndef TOPPERS_CKB1808_H
#define TOPPERS_CKB1808_H

#include "../ev3_gcc/ev3.h"

/*
 *  チップ(AM1808)のクロック周波数(MHz)
 */
#undef CORE_CLK_MHZ
#define CORE_CLK_MHZ 456

/*
 *  チップ(AM1808)のOSCIN周波数(MHz)
 */
#undef OSCIN_MHZ
#define OSCIN_MHZ 24

#endif /* TOPPERS_CKB1808_H */
