#ifndef _YYDEBUG_H_
#define _YYDEBUG_H_

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <pthread.h>

#define PRINT_CONFIGURE	PRINT_CONFIGURE_YES
#define PRINT_CONFIGURE_YES	1
#define PRINT_CONFIGURE_NO 	0
#define PRINT_DEBUG                 1

#define DEBUG_INFO __FILE__, __LINE__, __FUNCTION__


extern int  YYPrint_str(char *s, char *filename, int line);
extern int  YYPrint_str_value(char *s, int value, char *filename, int line);
extern int  YYPrintDebug(char *s, int value, char *fileName, int line, const char *func);
extern int  YYPrintDebugType1(char *notice, char *s, int value, char *s1, int value1, char *fileName, int line, const char *func, int len);

extern int  YYPrintBegin(void);
extern int  YYPrintEnd(void);
#endif
///////////////////////////////////end of file//////////////////////////////////