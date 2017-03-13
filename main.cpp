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

#define PORT_NUMBER 7500
#define BUFF_LEN_YY 1024
int G_CHARGING_CONTROL;
int G_CMD = DO_NOTHING;

//int G_CHARGING_CONTROL;

int G_FN = FN_NOTHING;
//sudo sysctl -a | grep net.ipv4.tcp_wmem		//look buffer size

struct Entity
{
    int sockfd;
    char username[20];
    char buf[CMSGLEN];
    struct sockaddr_in client;
    int stat;
};

void *func(void *arg);

struct Entity ent[5];

//void communicate_process(int index)
void *communicate_process(void *arg)
{
	char senBuf[BUFF_LEN_YY];
	char rcevBuf[BUFF_LEN_YY] = "123456789\n";
	int recvLen = 0;
	int reuse_addr = 1;
	int setsockoptRet = 0;
	int fcntlRet = 0;
	int clientConnected;
	//struct CLIENTMSG sendMsg;
	//struct CLIENTMSG recvMsg;
	clientConnected = *(int*)(arg);
	//memcpy(&clientConnected, arg, sizeof(clientConnected));
	printf("new connected is coming.     clientConnected=%d\n", clientConnected);



	fcntlRet = fcntl(clientConnected, F_SETFL, O_NONBLOCK) ;
	if(fcntlRet == -1)
	{
		YYPrint_str("fcntl() is fail!!!", __FILE__, __LINE__);
		perror("fcntl");
		exit(errno);
	}
	else
	{
		YYPrint_str("fcntl() is successful!!!", __FILE__, __LINE__);
	}
	//end of set socket O_NONBLOCK

	usleep(200);
	YYPrint_str("LoginRecvLinux(clientConnected);", __FILE__, __LINE__);
	LoginRecvLinux(clientConnected);
	while (1)
	{
		ServerRecvAllPacketLinux(clientConnected);

		usleep(100);
		
		switch(G_CMD)
		{		
			case DO_NOTHING:
			{				
				#if (DEBUG_ALL == 1)
				//printf("G_CMD = DO_NOTHING\n");	
				#endif
				break;
			}			
            case AFN_REAL_DATA://0x10 16 added by Y@2016.09.06
			{
                printf("G_CMD = AFN_REAL_DATA \n");

				int fn = 0;

				fn = G_FN;

                ServerSendRealData(clientConnected, fn);//AFN_REQUEST_TYPE1


                G_CMD = DO_NOTHING;//
				G_FN = FN_NOTHING;

				break;
            }
			case AFN_RESET:
			{
				printf("G_CMD = AFN_RESET\n");				

				ServerSendResetCmd(clientConnected);


				G_CMD = DO_NOTHING;
				G_FN = FN_NOTHING;
	
				break;
			}

			case AFN_SET_PARAMETER: //0x04
			{
				printf("G_CMD = AFN_SET_PARAMETER\r\n");		

				int fn = 0;
				fn = G_FN;

				ServerSendSetParameter(clientConnected, fn);//AFN_SET_PARAMETER

				G_CMD = DO_NOTHING;
				G_FN = FN_NOTHING;
	
				break;
			}
			case AFN_CONTROL:
			{
				printf("G_CMD = AFN_CONTROL\r\n");				

				ServerSendControl(clientConnected);//AFN_CONTROL


				G_CMD = DO_NOTHING;
				G_FN = FN_NOTHING;
	
				break;
			}
			case AFN_REQUEST_TERMINAL_INFO:
			{
				printf("G_CMD = AFN_REQUEST_TERMINAL_INFO\n");				

				ServerSendRequestTerminalInfo(clientConnected);

				G_CMD = DO_NOTHING;
				G_FN = FN_NOTHING;
				break;
			}
			case AFN_REQUEST_TYPE1:
			{
				printf("G_CMD = AFN_REQUEST_TYPE1\n");				

				//ServerSendRequestTerminalInfo(s);
				int fn = 0;
				fn = G_FN;
				ServerSendRequestType1(clientConnected, fn);//AFN_REQUEST_TYPE1
				G_CMD = DO_NOTHING;
				G_FN = FN_NOTHING;

				break;
			}
			default:
			{
				#if (DEBUG_ALL == 1)
				printf("G_CMD is error\r\n");
				#endif
				G_CMD = DO_NOTHING;
				G_FN = FN_NOTHING;
			    	break;
			}		
		}
	}
	pthread_exit(NULL);
}

int main()
{
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sockfd,connetfd;
	char ip[20] = "127.0.0.1";
	int port;
	int addrlen;
	struct CLIENTMSG clientMsg;
	pthread_t tid;

	int fcntlRet = 0;
	int reuse_addr = 1;
	
	int i;
	int err;
	int setsockoptRet = 0;
	int listenRet = 0;
	int connectedNumber = 0;

	//creat input command
	CreatInputCMD();
	//end of creat input command

	//clear client list	
	for(i=0;i<5;i++)
	{
	    ent[i].stat = 0;
	}
	//end of clear client list
	
	//creat socket (1)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket() error\n");
		exit(1);
	}
	else
	{
		YYPrint_str("socket() is OK !", __FILE__, __LINE__);
	}
	//end of creat socket
	
	/*//set socket O_NONBLOCK		(2)
	setsockoptRet = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)), sizeof(reuse_addr));
	if (setsockoptRet == 0)
	{
		YYPrint_str("setsockopt() is OK !", __FILE__, __LINE__);
	}
	else
	{
		YYPrint_str("setsockopt() is fail !", __FILE__, __LINE__);
	}
	//end of set socket O_NONBLOCK*/
	
	
	//bind (3)
	bzero(&server,sizeof(server));
	port = PORT_NUMBER;	
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	//server.sin_addr.s_addr = inet_addr(ip);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("bind() error\n");
		exit(1);
	}
	//end of bind

    //listen (4)
	listenRet = listen(sockfd,5);
	if (listen(sockfd,5) == -1)
	{
		perror("listen() error\n");
		exit(1);
	}
	else
	{
		YYPrint_str_value("now server is listening! port=", port, __FILE__, __LINE__);
	}
	//end of listen

	while(1)
	{
		addrlen = sizeof(client);
        connetfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&addrlen);
		if (connetfd == -1)
		{
			perror("accept() error\n");
			exit(1);
		}
		else
		{
			printf("accept is successful!  connetfd:%d\n", connetfd);
			err = pthread_create(&tid, NULL, communicate_process, (void *)&connetfd);
			if (err != 0)
			{
				printf("can't create thread: %s\n", strerror(err));
				//printids("main thread:");
				pthread_join(tid,NULL);
				return EXIT_SUCCESS;
			}
			else
			{
				YYPrint_str_value("pthread_create is successful! err=", err, __FILE__, __LINE__);
			}
		}
	}

	/*----------------------close-------------------*/
	close(sockfd);
	return 0;
}
//////////////////////////////////////////////////////////////end of file//////////////////////////////////////////////////////////////////