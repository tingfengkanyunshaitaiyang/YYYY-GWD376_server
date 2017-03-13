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
#include <errno.h>
#include "clientmsg.h"
#include "YYDebug.h"
#include "Gwd376Linux.h"

//nininininininisdsds
char ss[] = "\r\n******************************support command******************************\r\n\
clear console = 0 + ENTER\r\n\
show conmmand = 1 + ENTER\r\n\
\r\n\
1.<set结算电价   = AFN:4 + ENTER + FN:201 + ENTER>\r\n\
2.<set黑名单     = AFN:4 + ENTER + FN:203 + ENTER>\r\n\
2.<set分时服务费 = AFN:4 + ENTER + FN:203 + ENTER>\r\n\
3.<set桩编号     = AFN:4 + ENTER + FN:206 + ENTER>\r\n\
\r\n\
\r\n\
        实时数据召测 AFN=0X0C\r\n\
4.<召测终端日历时钟                 = AFN:12 + ENTER + FN:2 + ENTER>\r\n\
2.<召测黑白名单                     = AFN:12 + ENTER + FN:203 + ENTER>\r\n\
5.<召读充电桩的时钟及当前执行电价   = AFN:12 + ENTER + FN:204 + ENTER>\r\n\
6.<召读充电桩的时钟及当前执行服务费 = AFN:12 + ENTER + FN:205 + ENTER>\r\n\
7.<召测桩编号 = AFN:10 + ENTER + FN:2 + ENTER>\r\n\
\r\n\
4.<设置时间 set time = AFN:5 + ENTER>\r\n\
\r\n\
5.<no card charging 	= 12 + ENTER + 226 + ENTER + 1(start) 2(stop)>\r\n\
\r\n\
\r\n\
5.<召测结算信息 = AFN:16(0x10) + ENTER + FN:201 + ENTER>>\r\n\
5.<预约充电 = AFN:16(0x10) + ENTER + FN:203 + ENTER>>\r\n\
5.<停止预约充电 = AFN:16(0x10) + ENTER + FN:204 + ENTER>>\r\n\
******************************************************************************************\r\n";

void *InputCMD(void *arg)   
{
	int inCmd = 0;
	int fn = 0;

	printf("%s", ss);

	while(1)
	{
		scanf("%d", &inCmd);
		printf("inCmd=%d\n", inCmd);

		if ((inCmd == AFN_REQUEST_TYPE1) 
			|| (inCmd == AFN_REAL_DATA)
			|| (inCmd == AFN_SET_PARAMETER))
		{
			printf("=");		
			scanf("%d", &fn);
			printf("fn-------------=%d\n", fn);	

			if (fn == 226)
			{
				printf("G_CHARGING_CONTROL=");
				scanf("%d", &G_CHARGING_CONTROL);
				printf("G_CHARGING_CONTROL-------------=%d\n", G_CHARGING_CONTROL);
			}
		}

		if (inCmd == 0)
		{		
			system("clear");//
			inCmd = DO_NOTHING;//
			fn = FN_NOTHING;
		}

		if (inCmd == 1)//
		{
			printf("%s", ss);
			inCmd = DO_NOTHING;//
			fn = FN_NOTHING;
		}
		G_CMD = inCmd;//
		G_FN = fn;
		//usleep(200);//
	}
	return FUNCTION_RETURN_SUCCESS;
}

int CreatInputCMD(void)
{
	pthread_t tidInputCMD;
	int errInputCMD;
	
	errInputCMD = pthread_create(&tidInputCMD, NULL, InputCMD, NULL);
	if (errInputCMD != 0)
	{
		printf("can't create InputCMD errInputCMD: %s\n", strerror(errInputCMD));
		//printids("main thread:");
		pthread_join(tidInputCMD,NULL);
		return EXIT_SUCCESS;
	}
	else
	{
		YYPrint_str_value("pthread_create InputCMD is successful! err=", errInputCMD, __FILE__, __LINE__);
		usleep(200);//wait
	}
}
/**************************************end of file****************************************************/
