/*
 *  システムサービスのターゲット依存部（CKB-1808用）
 *
 *  システムサービスのターゲット依存部のインクルードファイル．このファ
 *  イルの内容は，コンポーネント記述ファイルに記述され，このファイルは
 *  無くなる見込み．
 */

#ifndef TOPPERS_TARGET_SYSSVC_H
#define TOPPERS_TARGET_SYSSVC_H

/*
 *  起動メッセージのターゲットシステム名
 */
#define TARGET_NAME "CKB-1808(AM1808(ARM926ES-J))"

/*
 *  システムログの低レベル出力のための文字出力
 *
 *  ターゲット依存の方法で，文字cを表示/出力/保存する．
 */
extern void	target_fput_log(char c);

/*
 *  ログタスクで使用するポート
 */
#define LOGTASK_PORTID  SIO_PORT_DEFAULT
#define TASK_PORTID     SIO_PORT_DEFAULT
#define LOWLEVEL_PORTID SIO_PORT_DEFAULT

#endif /* TOPPERS_TARGET_SYSSVC_H */
