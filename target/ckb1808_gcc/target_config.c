/*
 * ターゲット依存モジュール（CKB-1808用）
 */
#include "kernel_impl.h"
#include <sil.h>
#include "target_syssvc.h"
#include "am1808.h"

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{
	/*
	 * チップ依存の初期化
	 */
	chip_initialize();
}

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
    // Flush low level output by write 2K bytes.
    for(int i = 0; i < 2048; ++i)
        target_fput_log('\0');
	
    while(1);
}
