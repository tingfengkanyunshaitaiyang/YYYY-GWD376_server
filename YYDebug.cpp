#include "YYDebug.h"

int  YYPrint_str(char *s, char *filename, int line)
{
	#if PRINT_CONFIGURE == PRINT_CONFIGURE_YES
	printf("%s--@%s@line:%d\n", s, filename, line);
	#endif
	return 0;
}

int  YYPrint_str_value(char *s, int value, char *filename, int line)
{
	#if PRINT_CONFIGURE == PRINT_CONFIGURE_YES
	printf("%s%d--@%s@line:%d\n", s, value, filename, line);
	#endif
	return 0;
}

int  YYPrintDebug(char *s, int value, char *fileName, int line, const char *func)
{
    #if PRINT_DEBUG
    printf("\n%s%d    FILE:%s  L:line:%d  FUNC: %s", s, value, fileName, line, func);

    struct tm *t;
    time_t tt;
    time(&tt);
    t = localtime(&tt);
    printf("%4d年%02d月%02d日 %02d-%02d-%02d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    #endif
    return 0;
}

int  YYPrintDebugType1(char *notice, char *s, int value, char *s1, int value1, char *fileName, int line, const char *func, int len)
{
#if PRINT_DEBUG
    struct tm *t;
    time_t tt;
    time(&tt);
    t = localtime(&tt);
    printf("%s  len =%d", notice, len);
    printf("\r\nFILE:%s  L:%d  FUNC: %s", fileName, line, func);
    printf("\r\n%4d年%02d月%02d日:%02d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    printf("\r\n%s = 0X%02X", s, value);
    printf("\r\n%s = 0X%02X\r\n", s1, value1);
#endif
    return 0;
}

int  YYPrintBegin(void)
{
#if PRINT_DEBUG
    printf("\r\n<-------------------------------------------------------------------->\r\n");
    //printf("\r\n");
#endif
    return 0;
}

int  YYPrintEnd(void)
{
#if PRINT_DEBUG
    printf("\r\n<-------------------------------------------------------------------->\r\n");
    printf("\r\n");
#endif
    return 0;
}

///////////////////////////////////end of file//////////////////////////////////