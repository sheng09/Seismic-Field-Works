#include <stdio.h>
#include <stdlib.h>
#include "liberrmsg.h"
// 声明出错信息
char* errmsg[] = {
    /* 0 */       "No error",
    /* 1 */       "Open file error",
    /* 2 */       "Read file interrupted",
    /* 3 */       "Bad arguments",
    /* 4 */       "Memeroy is not enough",
    /* 5 */       "Service is down; try later",
    /* 6 */       "Unknow information",
    /* 7 */       "A socket operation has failed",
    /* 8 */       "Permission denied",
    /* 9 */       "Bad configuration file format",
    /* 10 */      "Communication time out",
    /* 11 */      "More arguments are required",
    /* 12 */      "Allocate memory failed",
    /* 13 */      "Data is out of range"
};

// 打印出错信息函数
void perrmsg( char* info, ERR_TYPE errno)
{
    if ( info ){
        printf("%s: %s\n", info, errmsg[errno] );
        return;
    }
    printf("Error: %s\n", errmsg[errno] );
}