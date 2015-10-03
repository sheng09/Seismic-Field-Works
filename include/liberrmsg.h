#ifndef __ERRMSG__
#define __ERRMSG__

// 声明出错代码
#define     ERR_NO_ERROR    0L  /* No error                 */
#define     ERR_OPEN_FILE   1L  /* Open file error          */
#define     ERR_READ_FILE   2L  /* Read file interrupted    */
#define     ERR_BAD_ARGS    3L  /* Bad arguments            */
#define     ERR_MEM_NONE    4L  /* Memeroy is not enough    */
#define     ERR_SERV_DOWN   5L  /* Service down try later   */
#define     ERR_UNKNOW_INFO 6L  /* Unknow information       */
#define     ERR_SOCKET_ERR  7L  /* Socket operation failed  */
#define     ERR_PERMISSION  8L  /* Permission denied        */
#define     ERR_BAD_FORMAT  9L  /* Bad configuration file   */
#define     ERR_TIME_OUT   10L  /* Communication time out   */
#define     ERR_MORE_ARGS  11L  /* More arguments are required */
#define     ERR_ALC_MEM    12L  /* Allocate memory failed */
#define     WARN_OUT_RANGE 13L  /* Data is out of range     */

typedef long ERR_TYPE;
// 打印出错信息函数
void perrmsg( char* info, ERR_TYPE errno);

#endif