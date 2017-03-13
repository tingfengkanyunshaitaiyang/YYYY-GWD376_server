/*
 * Gwd376Linux.c
 *
 *  Created on: Oct 19, 2016
 *      Author: lang
 */
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
#include "Gwd376Linux.h"
#include "YYDebug.h"
#include <QTime>




//
int counterPackageRev;

unsigned char LoginOK[] = {0x68, 0x32, 0x00, 0x32, 0x00, 0x68,\
		0x00, 0x34, 0x02, 0x10, 0x27, 0x02, 0x00, 0x60, 0x00, 0x00, 0x01, 0x00, 0xd0, 0x16};


unsigned char HeartBeatOK[] = {0x68, 0x32, 0x00, 0x32, 0x00, 0x68,\
		0x00, 0x34, 0x02, 0x10, 0x27, 0x02, 0x00, 0x60, 0x00, 0x00, 0x01, 0x00, 0xd0, 0x16};

void WaitLoop()
{
	printf("ͣWaitLoop\r\n");
	while(1)
	{
		usleep(3000);
	}
}
void ToCharHex(unsigned char buf[BUF_LEN],
                            char   bufToHexChar[BUF_LEN * 3],
                            int   Ret)//
{
    int counter = 0;
    char dataHigh = 0;
    char dataLow = 0;

    for(counter = 0; counter < Ret; counter++)
    {
        dataHigh = (buf[counter] / ((unsigned char)16));

        if ((dataHigh >= 0) && (dataHigh <= 9))
        {
            bufToHexChar[counter * 3] = dataHigh + '0';
        }
        else
        {
            switch (dataHigh)
            {
                case 10:
                {
                    bufToHexChar[counter * 3] = 'A';
                    break;
                }
                case 11:
                {
                    bufToHexChar[counter * 3] = 'B';
                    break;
                }
                case 12:
                {
                    bufToHexChar[counter * 3] = 'C';
                    break;
                }
                case 13:
                {
                    bufToHexChar[counter * 3] = 'D';
                    break;
                }
                case 14:
                {
                    bufToHexChar[counter * 3] = 'E';
                    break;
                }
                case 15:
                {
                    bufToHexChar[counter * 3] = 'F';
                    break;
                }
                default:
                {
                	printf("dataHigh is error!\r\n");
                    break;
                }
            }
        }

        dataLow = (buf[counter] % ((unsigned char)16));
        if ((dataLow >= 0) && (dataLow <= 9))
        {
            bufToHexChar[counter * 3 + 1] = dataLow + '0';
        }
        else
        {
            switch (dataLow)
            {
                case 10:
                {
                    bufToHexChar[counter * 3 + 1] = 'A';
                    break;
                }
                case 11:
                {
                    bufToHexChar[counter * 3 + 1] = 'B';
                    break;
                }
                case 12:
                {
                    bufToHexChar[counter * 3 + 1] = 'C';
                    break;
                }
                case 13:
                {
                    bufToHexChar[counter * 3 + 1] = 'D';
                    break;
                }
                case 14:
                {
                    bufToHexChar[counter * 3 + 1] = 'E';
                    break;
                }
                case 15:
                {
                    bufToHexChar[counter * 3 + 1] = 'F';
                    break;
                }
                default:
                {
                	printf("datdataLow is error!\r\n");
                    break;
                }
            }
        }
        bufToHexChar[counter * 3 + 2] = ' ';
    }
    bufToHexChar[counter * 3] = '\0';

	if((counter * 3) > BUF_LEN)
	{
		printf("ToCharHex is error\r\n");
	}
}

int LoginRecvLinux(int sClient)
{
	int s;
	char   buf[BUF_LEN];
	unsigned char bufUnsignedchar[BUF_LEN];
	int sendRet;
	char bufToHexChar[BUF_LEN * 3];
	int   Ret;
	char					sendbuff[BUF_LEN];

	s = sClient;

	memset(buf, '\0',sizeof(buf));
	YYPrint_str_value("wait for client login 2  s = sClient=", s, __FILE__, __LINE__);

	while(1)
	{
		if((Ret=recv(sClient, buf, sizeof(buf), 0))>0)
		{
			printf("recv_sock:   %d,   ", s);
			printf("receive num = %d  ", Ret);
			memcpy(bufUnsignedchar, buf, sizeof(bufUnsignedchar));
			ToCharHex(bufUnsignedchar, bufToHexChar, Ret);//
			printf("buf = %s\n", bufToHexChar);

			printf("bufToHexChar = %d\n", strlen(bufToHexChar));//strlen


			//PrintDebugFile( buf, bufToHexChar);
			break;
		}
	}

	memcpy(sendbuff, LoginOK, 20);
	sendRet = send(sClient, sendbuff, 20, 0);
	if(sendRet <= 0)
	{
	   printf("Send error  sendRet:%d\r\n", sendRet);
	   WaitLoop();
	}
	printf("send %d\n",sendRet);

	return FUNCTION_RETURN_SUCCESS;
}
void CRCSum(char *buf, int len)
{
	uint16 tmpAddCs = 0;
	uint16 iCount = 6;
	//buf
	for(iCount;iCount < 18;iCount ++)
	{
		tmpAddCs += buf[iCount];
	}
	buf[len - 2] = (unsigned char)tmpAddCs;
}
void CRCSum1(unsigned char *buf, int len)
{
	uint16 tmpAddCs = 0;
	uint16 iCount = 6;
	//buf
	for(iCount; iCount < 18; iCount ++)
	{
		tmpAddCs += buf[iCount];
	}
	buf[len - 2] = (unsigned char)tmpAddCs;
}

void FuncToCharHex(char buf[BUF_LEN], char bufToHexChar[BUF_LEN * 3],int len)
{
	unsigned char bufUnsignedchar[BUF_LEN];
	memcpy(bufUnsignedchar, buf, sizeof(bufUnsignedchar));
	ToCharHex(bufUnsignedchar,
					bufToHexChar,
					len);
}
int GWD376_PackagingPacketServer(int typePack, char *buf, int *len, uint16 fn)
{
	unsigned char PacketReset[] = {0x68, 0x32, 0x00, 0x32, 0x00, 0x68,\
		0x41, //c
		0x34, 0x02, 0x10, 0x27, 0x02, //a
		0x01, //afn
		0x60, //seq
		0x00, 0x00, 0x01, 0x00, 
		0xd0, //crc
		0x16};//end

	GWD376_PACKET packetData;
	int length;

	char				bufRecv[BUF_LEN];
	unsigned char bufUnsignedchar[BUF_LEN];
	char bufToHexChar[BUF_LEN * 3];
	uint8 cardNumber[8];
	uint8 tradeSerialNumber[6];
	//REQUEST_TERMINAL_CARD_TRADE_DOWN_DATA realDataAfn0x10F200;

	switch (typePack)
	{
		case AFN_OK_OR_NOT:
		{
			break;
		}
		case AFN_RESET:
		{			
			packetData.begin.beginSign = PACKET_BEGIN;
			packetData.begin.len.LenBitArea.len2 = 0x02;
			packetData.begin.len1.LenBitArea.len2 = 0x02;
			packetData.begin.beginSign1 = PACKET_BEGIN;
			
			packetData.userDataArea.controllArea.c = 0x41;
			packetData.userDataArea.adressArea.A1 = 0x234;
			packetData.userDataArea.adressArea.A2 = 10000;
			packetData.userDataArea.adressArea.A3 = 0x00;
			
			packetData.userDataArea.appLayer.afn = AFN_RESET;
			packetData.userDataArea.appLayer.seq = 0x70;
			
			GWD376_DataUnitSign dataUnitSign1;
			dataUnitSign1.pn = 0x0000;
			dataUnitSign1.fn = 0x0001;			
			
			length = sizeof(packetData.userDataArea) + sizeof(dataUnitSign1);
						//+ sizeof(packetData.userDataArea.adressArea)
			//printf("packetData.userDataArea len is=%d", length);

			packetData.begin.len.LenBitArea.len1 =	length;
			packetData.begin.len1.LenBitArea.len1 = length;
			packetData.end = PACKET_END;

			int lengthAll = 0;
			lengthAll = sizeof(packetData) - 2;

			memcpy(buf, (void *)&packetData, lengthAll);

			memcpy(&buf[lengthAll], (void *)&dataUnitSign1, sizeof(dataUnitSign1));
			lengthAll += sizeof(dataUnitSign1);
			//the above is fixed way
			
			//the below is fixed way
			lengthAll += 2;
			CRCSum(buf, lengthAll);
			memcpy(&buf[lengthAll - 1], (void *)&packetData.end, sizeof(packetData.end));
			*len = lengthAll;

			#if 1	
			FuncToCharHex(buf, bufToHexChar, lengthAll);
			printf("AFN_RESET = %s\n", bufToHexChar);
			#endif

			break;
		}
		case AFN_SET_PARAMETER:
		{
			packetData.begin.beginSign = PACKET_BEGIN;
			packetData.begin.beginSign1 = PACKET_BEGIN;

			packetData.userDataArea.controllArea.c = 0x4A;
			packetData.userDataArea.adressArea.A1 = TERMINAL_AREA;
			packetData.userDataArea.adressArea.A2 = TERMINAL_ADDRESS;
			packetData.userDataArea.adressArea.A3 = 0x00;
			
			packetData.userDataArea.appLayer.afn = AFN_SET_PARAMETER;//
			packetData.userDataArea.appLayer.seq = 0x60;

			GWD376_DataUnitSign dataUnitSign1;
			dataUnitSign1.pn = 0x0000;

			if (fn == 201)
			{
				dataUnitSign1.fn = 0x1901;
			}
			else if (fn == 202)
			{
				dataUnitSign1.fn = 0x1902;
			}
			else if (fn == 203)
			{
				dataUnitSign1.fn = 0x1904;
			}
			else if (fn == 206)
			{
				dataUnitSign1.fn = 0x1920;
			}
			else
			{
				printf("fn is error = %d  %s %d", fn, __FILE__, __LINE__);
			}

			int lengthAll = 0;
			lengthAll = sizeof(packetData) - 2;

			memcpy(buf, (void *)&packetData, lengthAll);

			memcpy(&buf[lengthAll], (void *)&dataUnitSign1, sizeof(dataUnitSign1));
			lengthAll += sizeof(dataUnitSign1);


            if (dataUnitSign1.fn == 0x1901)//FN=201
            {
                ELECT_PRICE electPrice;
                uint8 timeTemp[7] = {1, 2, 3, 4, 5, 6, 7};
                memcpy(electPrice.Time, timeTemp, sizeof(electPrice.Time));

                uint8 priceTemp[3] = {6, 1, 2};
                memcpy(electPrice.TineElectPrice, priceTemp, sizeof(electPrice.TineElectPrice));
                memcpy(electPrice.PeakElectPrice, priceTemp, sizeof(electPrice.PeakElectPrice));
                memcpy(electPrice.FlatElectPrice, priceTemp, sizeof(electPrice.FlatElectPrice));
                memcpy(electPrice.VallyElectPrice, priceTemp, sizeof(electPrice.VallyElectPrice));

                memcpy(&buf[lengthAll], (void *)&electPrice, sizeof(ELECT_PRICE));
                lengthAll += sizeof(ELECT_PRICE);
            }
            else if (dataUnitSign1.fn == 0x1902)//FN=202
            {
                SERVER_PRICE serverPrice;
                uint8 timeTemp1[7] = {1, 2, 3, 4, 5, 6, 7};
                memcpy(serverPrice.Time, timeTemp1, sizeof(serverPrice.Time));

                uint8 priceTemp1[2] = {1, 2};
                memcpy(serverPrice.TineServerPrice, priceTemp1, sizeof(serverPrice.TineServerPrice));
                memcpy(serverPrice.PeakServerPrice, priceTemp1, sizeof(serverPrice.PeakServerPrice));
                memcpy(serverPrice.FlatServerPrice, priceTemp1, sizeof(serverPrice.FlatServerPrice));
                memcpy(serverPrice.VallyServerPrice, priceTemp1, sizeof(serverPrice.VallyServerPrice));
                memcpy(&buf[lengthAll], (void *)&serverPrice, sizeof(SERVER_PRICE));
                lengthAll += sizeof(SERVER_PRICE);
            }
            else if (dataUnitSign1.fn == 0x1904)//FN=203
            {

            }
            else if (dataUnitSign1.fn == 0x1920)//FN=206
			{
				uint8 accountEquipmentID[6] = {1, 2, 3, 4, 5, 6};
				uint8 chargeRunNum[8] = {1, 2, 3, 4, 5, 6, 7, 8};

				memcpy(&buf[lengthAll], (void *)&accountEquipmentID, sizeof(accountEquipmentID));
				lengthAll += sizeof(accountEquipmentID);

				memcpy(&buf[lengthAll], (void *)&chargeRunNum, sizeof(chargeRunNum));
				lengthAll += sizeof(chargeRunNum);
			}
            else
            {
                printf("dataUnitSign1.fn is error = %d  %s %d", dataUnitSign1.fn, __FILE__, __LINE__);
            }

			TIME_LABLE tpTable;

			tpTable.TpCount = 6;
			tpTable.TpTime.Day = 6;
			tpTable.TpTime.Hour = 6;
			tpTable.TpTime.Minute = 6;
			tpTable.TpTime.Second = 6;
			tpTable.Delay = 6;

			memcpy(&buf[lengthAll], (void *)&tpTable, sizeof(tpTable));
			lengthAll += sizeof(tpTable);

			//the below is fixed way
			lengthAll += 2;
			CRCSum(buf, lengthAll);

			uint16 lengthUser = (((lengthAll - 6) << 2) | 0x02);
			memcpy(&buf[1], (void *)&lengthUser, sizeof(lengthUser));
			memcpy(&buf[3], (void *)&lengthUser, sizeof(lengthUser));


			packetData.end = PACKET_END;
			memcpy(&buf[lengthAll - 1], (void *)&packetData.end, sizeof(packetData.end));
			*len = lengthAll;

			#if 1	
			FuncToCharHex(buf, bufToHexChar, lengthAll);
			printf("AFN_SET_PARAMETER: = %s		fn=%d	dataUnitSign1.fn=0x%x\r\n", bufToHexChar, fn, dataUnitSign1.fn);
			#endif

			break;
		}
		case AFN_CONTROL:
		{
			//6byte
			packetData.begin.beginSign = PACKET_BEGIN;
			packetData.begin.len.LenBitArea.len2 = 0x02;
			packetData.begin.len1.LenBitArea.len2 = 0x02;
			packetData.begin.beginSign1 = PACKET_BEGIN;

			//5byte
			packetData.userDataArea.controllArea.c = 0x4A;//
			packetData.userDataArea.adressArea.A1 = TERMINAL_AREA;//
			packetData.userDataArea.adressArea.A2 = TERMINAL_ADDRESS;//
			packetData.userDataArea.adressArea.A3 = 0x00;//

			//
			packetData.userDataArea.appLayer.afn = AFN_CONTROL;
			packetData.userDataArea.appLayer.seq = 0x60;

			//
			GWD376_DataUnitSign dataUnitSign1;
			dataUnitSign1.pn = 0x0000;
			dataUnitSign1.fn = 0x0340,		//F31 
			//end of
			//end of

			length = sizeof(packetData.userDataArea) + sizeof(dataUnitSign1);
			//+ sizeof(packetData.userDataArea.adressArea)
			//printf("packetData.userDataArea len is=%d", length);

			packetData.begin.len.LenBitArea.len1 = length;
			packetData.begin.len1.LenBitArea.len1 = length;
			packetData.end = PACKET_END;

			int lengthAll = 0;
			lengthAll = sizeof(packetData) -2;

			memcpy(buf, (void *)&packetData, lengthAll);

			memcpy(&buf[lengthAll], (void *)&dataUnitSign1, sizeof(dataUnitSign1));
			lengthAll += sizeof(dataUnitSign1);
			///////////////////////////////////////////the above is fixed way//////////////////////////////////////////////

			//
			GWD376_CONTROL_CMD_TIME GWD376_ControlCmdTime;//
			printf("sizeof(GWD376_CONTROL_CMD_TIME) = %d\n", sizeof(GWD376_CONTROL_CMD_TIME));

			GWD376_ControlCmdTime.secend = 1;
			GWD376_ControlCmdTime.minute = 1;
			GWD376_ControlCmdTime.hour = 16;
			GWD376_ControlCmdTime.day = 30;
			GWD376_ControlCmdTime.month = 8;
			GWD376_ControlCmdTime.year = 16;
			memcpy(&buf[lengthAll], (void *)&GWD376_ControlCmdTime, sizeof(GWD376_ControlCmdTime));
			lengthAll += sizeof(GWD376_ControlCmdTime);
			//end of 

			//
			TIME_LABLE tpTable;

			tpTable.TpCount = 6;
			tpTable.TpTime.Day = 6;
			tpTable.TpTime.Hour = 6;
			tpTable.TpTime.Minute = 6;
			tpTable.TpTime.Second = 6;
			tpTable.Delay = 6;

			memcpy(&buf[lengthAll], (void *)&tpTable, sizeof(tpTable));
			lengthAll += sizeof(tpTable);
			//end of 

			///////////////////////////////////////////the below is fixed way//////////////////////////////////////////////
			lengthAll += 2;
			CRCSum(buf, lengthAll);

			memcpy(&buf[lengthAll - 1], (void *)&packetData.end, sizeof(packetData.end));
			//*len = sizeof(PacketReset);
			*len = lengthAll;

			#if 1	
			FuncToCharHex(buf, bufToHexChar, lengthAll);
			printf("AFN_CONTROL = %s\n", bufToHexChar);
			#endif

			break;
		}
		case AFN_REQUEST_TERMINAL_INFO:
		{
			//6byte
			packetData.begin.beginSign = PACKET_BEGIN;
			packetData.begin.len.LenBitArea.len2 = 0x02;//
			packetData.begin.len1.LenBitArea.len2 = 0x02;
			packetData.begin.beginSign1 = PACKET_BEGIN;

			//5byte
			packetData.userDataArea.controllArea.c = 0x4A;
			packetData.userDataArea.adressArea.A1 = TERMINAL_AREA;
			packetData.userDataArea.adressArea.A2 = TERMINAL_ADDRESS;
			packetData.userDataArea.adressArea.A3 = 0x00;

			packetData.userDataArea.appLayer.afn = AFN_REQUEST_TERMINAL_INFO;
			packetData.userDataArea.appLayer.seq = 0x60;

			GWD376_DataUnitSign dataUnitSign1;
			dataUnitSign1.pn = 0x0000;
			dataUnitSign1.fn = 0x0001;
			//end of 
			//end of

			length = sizeof(packetData.userDataArea) + sizeof(dataUnitSign1);

			packetData.begin.len.LenBitArea.len1 =	length;
			packetData.begin.len1.LenBitArea.len1 = length;
			packetData.end = PACKET_END;

			int lengthAll = 0;//
			lengthAll = sizeof(packetData) - 2;//

			memcpy(buf, (void *)&packetData, lengthAll);

			memcpy(&buf[lengthAll], (void *)&dataUnitSign1, sizeof(dataUnitSign1));
			lengthAll += sizeof(dataUnitSign1);
			///////////////////////////////////////////the above is fixed way//////////////////////////////////////////

			//
			TIME_LABLE tpTable;

			tpTable.TpCount = 6;
			tpTable.TpTime.Day = 6;
			tpTable.TpTime.Hour = 6;
			tpTable.TpTime.Minute = 6;
			tpTable.TpTime.Second = 6;
			tpTable.Delay = 6;

			memcpy(&buf[lengthAll], (void *)&tpTable, sizeof(tpTable));
			lengthAll += sizeof(tpTable);
			//end of 

			/////////////////////////////////////////the below is fixed way/////////////////////////////////////////////
			lengthAll += 2;
			CRCSum(buf, lengthAll);

			memcpy(&buf[lengthAll - 1], (void *)&packetData.end, sizeof(packetData.end));
			*len = lengthAll;

			#if 1	
			FuncToCharHex(buf, bufToHexChar, lengthAll);
			printf("AFN_REQUEST_TERMINAL_INFO = %s\n", bufToHexChar);
			#endif

			break;
		}
		case AFN_REQUEST_TYPE1://0x0C=12
		{
			//6byte
			packetData.begin.beginSign = PACKET_BEGIN;

			packetData.begin.beginSign1 = PACKET_BEGIN;

			//5byte
			packetData.userDataArea.controllArea.c = 0x4A;//
			packetData.userDataArea.adressArea.A1 = TERMINAL_AREA;//A1
			packetData.userDataArea.adressArea.A2 = TERMINAL_ADDRESS;//A2
			packetData.userDataArea.adressArea.A3 = 0x00;//A3

			//
			packetData.userDataArea.appLayer.afn = AFN_REQUEST_TYPE1;
			packetData.userDataArea.appLayer.seq = 0x60;

			//
			GWD376_DataUnitSign dataUnitSign1;
			dataUnitSign1.pn = 0x0000;

			if (fn == 227)
			{
				dataUnitSign1.fn = 0x1c04;
			}
			else if (fn == 228)
			{
				dataUnitSign1.fn = 0x1c08;
			}
			else if (fn == 226)
			{
				dataUnitSign1.fn = 0x1c02;
			}
            else if (fn == 2)
            {
                dataUnitSign1.fn = F2;
            }
            else if (fn == 203)
            {
                dataUnitSign1.fn = F203;
            }
            else if (fn == 204)
            {
                dataUnitSign1.fn = F204;
            }
            else if (fn == 205)
            {
                dataUnitSign1.fn = F205;
            }
			else
			{
				printf("fn is error = %d  %s %d", fn, __FILE__, __LINE__);
			}
			//end of 
			//end of

			int lengthAll = 0;//
			lengthAll = sizeof(packetData) - 2;//

			memcpy(buf, (void *)&packetData, lengthAll);

			memcpy(&buf[lengthAll], (void *)&dataUnitSign1, sizeof(dataUnitSign1));
			lengthAll += sizeof(dataUnitSign1);
			/////////////////////////////////////////the above is fixed way////////////////////////////////////////////////
			if((dataUnitSign1.fn == 0x1c04)//FN=227	
				| (dataUnitSign1.fn == 0x1c08))//FN=228 
			{
				uint8 chargeID[6] = {1, 2, 3, 4, 5, 6};
				uint8 chargeInterface = 1;

				memcpy(&buf[lengthAll], (void *)&chargeID, sizeof(chargeID));
				lengthAll += sizeof(chargeID);

				memcpy(&buf[lengthAll], (void *)&chargeInterface, sizeof(chargeInterface));
				lengthAll += sizeof(chargeInterface);

			}
			else
			{
				if (dataUnitSign1.fn == 0x1c02)//F226
				{
					CONTROL_TERMINA_DOWN_DATA noCardStart;

                    uint8 tempTerminalId[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2};
                    uint8 tempTerminalId16[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};

                    noCardStart.TradeNo[0] = hex2bcd8((tempTerminalId[0] * 10) + tempTerminalId[1]);
                    noCardStart.TradeNo[1] = hex2bcd8((tempTerminalId[2] * 10) + tempTerminalId[3]);
                    noCardStart.TradeNo[2] = hex2bcd8((tempTerminalId[4] * 10) + tempTerminalId[5]);
                    noCardStart.TradeNo[3] = hex2bcd8((tempTerminalId[6] * 10) + tempTerminalId[7]);
                    noCardStart.TradeNo[4] = hex2bcd8((tempTerminalId[8] * 10) + tempTerminalId[9]);
                    noCardStart.TradeNo[5] = hex2bcd8((tempTerminalId[10] * 10) + tempTerminalId[11]);

                    noCardStart.TerminalId[0] = hex2bcd8((tempTerminalId[0] * 10) + tempTerminalId[1]);
                    noCardStart.TerminalId[1] = hex2bcd8((tempTerminalId[2] * 10) + tempTerminalId[3]);
                    noCardStart.TerminalId[2] = hex2bcd8((tempTerminalId[4] * 10) + tempTerminalId[5]);
                    noCardStart.TerminalId[3] = hex2bcd8((tempTerminalId[6] * 10) + tempTerminalId[7]);
                    noCardStart.TerminalId[4] = hex2bcd8((tempTerminalId[8] * 10) + tempTerminalId[9]);
                    noCardStart.TerminalId[5] = hex2bcd8((tempTerminalId[10] * 10) + tempTerminalId[11]);

                    noCardStart.TerminalIo = 0;

                    noCardStart.UserId[0] = hex2bcd8((tempTerminalId16[0] * 10) + tempTerminalId16[1]);
                    noCardStart.UserId[1] = hex2bcd8((tempTerminalId16[2] * 10) + tempTerminalId16[3]);
                    noCardStart.UserId[2] = hex2bcd8((tempTerminalId16[4] * 10) + tempTerminalId16[5]);
                    noCardStart.UserId[3] = hex2bcd8((tempTerminalId16[6] * 10) + tempTerminalId16[7]);
                    noCardStart.UserId[4] = hex2bcd8((tempTerminalId16[8] * 10) + tempTerminalId16[9]);
                    noCardStart.UserId[5] = hex2bcd8((tempTerminalId16[10] * 10) + tempTerminalId16[11]);
                    noCardStart.UserId[6] = hex2bcd8((tempTerminalId16[12] * 10) + tempTerminalId16[13]);
                    noCardStart.UserId[7] = hex2bcd8((tempTerminalId16[14] * 10) + tempTerminalId16[15]);

                    noCardStart.status = G_CHARGING_CONTROL;		//------>>>>>>

					memcpy(&buf[lengthAll], (void *)&noCardStart, sizeof(noCardStart));
					lengthAll += sizeof(noCardStart);
				}

				//
				TIME_LABLE tpTable;

				tpTable.TpCount = 6;
				tpTable.TpTime.Day = 6;
				tpTable.TpTime.Hour = 6;
				tpTable.TpTime.Minute = 6;
				tpTable.TpTime.Second = 6;
				tpTable.Delay = 6;

				memcpy(&buf[lengthAll], (void *)&tpTable, sizeof(tpTable));
				lengthAll += sizeof(tpTable);
				//end of
			}
			///////////////////////////////////////the below is fixed way//////////////////////////////////////////////////
			lengthAll += 2;
			CRCSum(buf, lengthAll);

			uint16 lengthUser = (((lengthAll - 6) << 2) | 0x02);
			memcpy(&buf[1], (void *)&lengthUser, sizeof(lengthUser));
			memcpy(&buf[3], (void *)&lengthUser, sizeof(lengthUser));
			//end of

			packetData.end = PACKET_END;
			memcpy(&buf[lengthAll - 1], (void *)&packetData.end, sizeof(packetData.end));
			*len = lengthAll;

			#if 1	
			FuncToCharHex(buf, bufToHexChar, lengthAll);
			if((dataUnitSign1.fn == 0x1c04)//FN=227	
				|| (dataUnitSign1.fn == 0x1c08))//FN=228 
			{
				printf("AFN_REQUEST_TYPE1 = %s   fn=0x%x\n", bufToHexChar ,dataUnitSign1.fn);
			}
			else
			{
				printf("AFN_REQUEST_TYPE1  = %s\n", bufToHexChar);
			}
			#endif

			break;
		}
        case AFN_REAL_DATA://0x10
		{
			packetData.begin.beginSign = PACKET_BEGIN;
			packetData.begin.beginSign1 = PACKET_BEGIN;

			//5byte
			packetData.userDataArea.controllArea.c = 0x4A;//
			packetData.userDataArea.adressArea.A1 = TERMINAL_AREA;//A1
			packetData.userDataArea.adressArea.A2 = TERMINAL_ADDRESS;//A2
			packetData.userDataArea.adressArea.A3 = 0x00;//A3

			packetData.userDataArea.appLayer.afn = AFN_REAL_DATA;
			packetData.userDataArea.appLayer.seq = 0x60;

			
			GWD376_DataUnitSign dataUnitSign1;

            if (fn == 201)
            {
                dataUnitSign1.fn = F201;
                fn = F201;
            }
            else if (fn == 200)
            {
                dataUnitSign1.fn = F200;
                fn = F200;
            }
            else if (fn == 203) //yuanpei
            {
                dataUnitSign1.fn = F203;
                fn = F203;
            }   //
            else if (fn == 204) //yuanpei
            {
                dataUnitSign1.fn = F204;
                fn = F204;
            }
            else if (fn == F201) //yuanpei
            {
                dataUnitSign1.fn = F201;
            }
            else
            {
                YYPrintDebug("no match fn/", 0, DEBUG_INFO);
            }
			dataUnitSign1.pn = 0x0000;
			//end of 

			int lengthAll = 0;
			lengthAll = sizeof(packetData) - 2;

			memcpy(buf, (void *)&packetData, lengthAll);

			memcpy(&buf[lengthAll], (void *)&dataUnitSign1, sizeof(dataUnitSign1));
			lengthAll += sizeof(dataUnitSign1);
			//////////////////////////////////////the above if fixed way///////////////////////////////////////////////////

			if (fn == 0x0019)
			{
				REQUEST_TERMINAL_CARD_TRADE_DOWN_DATA realDataAfn0x10F200;
				memset(&realDataAfn0x10F200, '1', sizeof(realDataAfn0x10F200));

				memcpy(&buf[lengthAll], (void *)&realDataAfn0x10F200, sizeof(realDataAfn0x10F200));
				lengthAll += sizeof(realDataAfn0x10F200);
			}
            else if (fn == F201)
			{
                YYPrintDebug("打包::发送结算信息应答报文/", 0, DEBUG_INFO);

                TRADE_DOWN_DATA date;
                uint8 testData[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2};

                uint8 tempTerminalId[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};



                date.cardId[0] = hex2bcd8((tempTerminalId[0] * 10) + tempTerminalId[1]);
                date.cardId[1] = hex2bcd8((tempTerminalId[2] * 10) + tempTerminalId[3]);
                date.cardId[2] = hex2bcd8((tempTerminalId[4] * 10) + tempTerminalId[5]);
                date.cardId[3] = hex2bcd8((tempTerminalId[6] * 10) + tempTerminalId[7]);
                date.cardId[4] = hex2bcd8((tempTerminalId[8] * 10) + tempTerminalId[9]);
                date.cardId[5] = hex2bcd8((tempTerminalId[10] * 10) + tempTerminalId[11]);
                date.cardId[6] = hex2bcd8((tempTerminalId[12] * 10) + tempTerminalId[13]);
                date.cardId[7] = hex2bcd8((tempTerminalId[14] * 10) + tempTerminalId[15]);


                date.tradeNo[0] = hex2bcd8((testData[0] * 10) + testData[1]);
                date.tradeNo[1] = hex2bcd8((testData[2] * 10) + testData[3]);
                date.tradeNo[2] = hex2bcd8((testData[4] * 10) + testData[5]);
                date.tradeNo[3] = hex2bcd8((testData[6] * 10) + testData[7]);
                date.tradeNo[4] = hex2bcd8((testData[8] * 10) + testData[9]);
                date.tradeNo[5] = hex2bcd8((testData[10] * 10) + testData[11]);

                memcpy(&buf[lengthAll], (void *)&date, sizeof(TRADE_DOWN_DATA));
                lengthAll += sizeof(TRADE_DOWN_DATA);
            }
            else if (fn == F203)//added by yuanpei@
            {//预约充电
                AppointmentChargeDownData ApptCharge;
                uint8 testDataApp[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

                uint8 tempTerminalIdApp[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2};

                ApptCharge.SettlementTerminal[0] = hex2bcd8((tempTerminalIdApp[0] * 10) + tempTerminalIdApp[1]);
                ApptCharge.SettlementTerminal[1] = hex2bcd8((tempTerminalIdApp[2] * 10) + tempTerminalIdApp[3]);
                ApptCharge.SettlementTerminal[2] = hex2bcd8((tempTerminalIdApp[4] * 10) + tempTerminalIdApp[5]);
                ApptCharge.SettlementTerminal[3] = hex2bcd8((tempTerminalIdApp[6] * 10) + tempTerminalIdApp[7]);
                ApptCharge.SettlementTerminal[4] = hex2bcd8((tempTerminalIdApp[8] * 10) + tempTerminalIdApp[9]);
                ApptCharge.SettlementTerminal[5] = hex2bcd8((tempTerminalIdApp[10] * 10) + tempTerminalIdApp[11]);

                ApptCharge.TerminalIo = 0;
                memcpy(ApptCharge.CardId, (void *)testDataApp, sizeof(ApptCharge.CardId));

                ApptCharge.BookDate.Year = ((hex2bcd16(QDateTime::currentDateTime().date().year())&0x00ff)<<8)+(hex2bcd16(QDateTime::currentDateTime().date().year())>>8);
                ApptCharge.BookDate.Month = hex2bcd8(QDateTime::currentDateTime().date().month());
                ApptCharge.BookDate.Day = hex2bcd8(QDateTime::currentDateTime().date().day());
                ApptCharge.BookTime.Hour = hex2bcd8(QTime::currentTime().hour());
                ApptCharge.BookTime.Minute = hex2bcd8(QTime::currentTime().minute() + 3);
                ApptCharge.BookTime.Second = hex2bcd8(QTime::currentTime().second());
                ApptCharge.BookLength = 0;
                ApptCharge.ImBookLength = 12;

                memcpy(&buf[lengthAll], (void *)&ApptCharge, sizeof(ApptCharge));
                lengthAll += sizeof(ApptCharge);
            }
            else if (fn == F204)//added by YY&YY@2017-02-16.
            {
                YYPrintDebug("打包::停止预约报文/", 0, DEBUG_INFO);

                CANCEL_BOOK_DATA cancelBookData;
                uint8 testDataApp[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

                uint8 tempTerminalIdApp[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2};

                cancelBookData.terminalID[0] = hex2bcd8((tempTerminalIdApp[0] * 10) + tempTerminalIdApp[1]);
                cancelBookData.terminalID[1] = hex2bcd8((tempTerminalIdApp[2] * 10) + tempTerminalIdApp[3]);
                cancelBookData.terminalID[2] = hex2bcd8((tempTerminalIdApp[4] * 10) + tempTerminalIdApp[5]);
                cancelBookData.terminalID[3] = hex2bcd8((tempTerminalIdApp[6] * 10) + tempTerminalIdApp[7]);
                cancelBookData.terminalID[4] = hex2bcd8((tempTerminalIdApp[8] * 10) + tempTerminalIdApp[9]);
                cancelBookData.terminalID[5] = hex2bcd8((tempTerminalIdApp[10] * 10) + tempTerminalIdApp[11]);

                cancelBookData.gunNO = 0;
                memcpy(cancelBookData.cardNumber, (void *)testDataApp, sizeof(cancelBookData.cardNumber));

                memcpy(&buf[lengthAll], (void *)&cancelBookData, sizeof(cancelBookData));
                lengthAll += sizeof(cancelBookData);
            }
            else
            {

            }

			/////////////////////////////////////////the below if fixed way///////////////////////////////////////////////////
			lengthAll += 2;//
			CRCSum(buf, lengthAll);//CRC

			uint16 lengthUser = (((lengthAll - 6) << 2) | 0x02);
			memcpy(&buf[1], (void *)&lengthUser, sizeof(lengthUser));
			memcpy(&buf[3], (void *)&lengthUser, sizeof(lengthUser));
			//end of

			packetData.end = PACKET_END;
			memcpy(&buf[lengthAll - 1], (void *)&packetData.end, sizeof(packetData.end));
			*len = lengthAll;

			#if 1
			FuncToCharHex(buf, bufToHexChar, lengthAll);
			printf("0x10 AFN_REAL_DATA = %s\n", bufToHexChar);
			#endif

			break;
		}
		case AFN_HEARTBEAT:
		{

			break;
		}

		default:
		{
			break;
		}
	}

	return FUNCTION_RETURN_SUCCESS;
}
int ServerSendDataTransmitAfn0x10F200(int s, int fnType)
{
	char					sendbuff[BUF_LEN];
	int sendRet;

	int lenSend;

	GWD376_PackagingPacketServer(AFN_REAL_DATA, sendbuff, &lenSend, fnType);//

	//memcpy(sendbuff, PacketReset, sizeof(PacketReset));
	//sendRet = send(s, sendbuff, sizeof(sendbuff), 0);
	sendRet = send(s, sendbuff, lenSend, 0);
	if(sendRet <= 0)
	{
		printf("F200 send fial------sendRet:%d\r\n", sendRet);
	}
	else
	{
		printf("F200 send seccuss------sendRet:%d\r\n", sendRet);
	}
	return FUNCTION_RETURN_SUCCESS;
}


int ServerSendDataTransmitAfn0x10F201(int s, int fnType)
{
	char					sendbuff[BUF_LEN];
	int sendRet;

	int lenSend;

	GWD376_PackagingPacketServer(AFN_REAL_DATA, sendbuff, &lenSend, fnType);//

    printf("lenSend=%d\r\n", lenSend);

	//memcpy(sendbuff, PacketReset, sizeof(PacketReset));
	//sendRet = send(s, sendbuff, sizeof(sendbuff), 0);
	sendRet = send(s, sendbuff, lenSend, 0);
    if (sendRet <= 0)
	{
		printf("F201 send fial------sendRet:%d\r\n", sendRet);
	}
	else
	{
		printf("F201 send seccuss------sendRet:%d\r\n", sendRet);
	}
	return FUNCTION_RETURN_SUCCESS;
}

int ServerCompareAllPacketLinux(char buf[BUF_LEN], int len, int s)
{
    char bufToHexChar[BUF_LEN * 3];
	char					sendbuff[BUF_LEN];
	int sendRet;

	uint16 pnHign = 0;
	uint16 pnLow = 0;
	uint16 fnHign = 0;
	uint16 fnLow = 0;
	uint16 pn = 0;
	uint16 fn = 0;
    int afn =  buf[12];

	REAL_TIME_STATE_UP_DATA realData;

    switch (afn)
	{
		case AFN_OK_OR_NOT:
		{
			printf("AFN_OK_OR_NOT %d\n", len);
			break;
		}
		case AFN_RESET:
		{
			printf("AFN_RESET %d\n", len);
			break;
		}
		case AFN_HEARTBEAT:
		{
			#if 0
			FuncToCharHex(buf, bufToHexChar, len);
			printf("AFN_HEARTBEAT=%s\r\n", bufToHexChar);
			#else
			//printf("AFN_HEARTBEAT\r\n");
			#endif

			memcpy(sendbuff, HeartBeatOK, sizeof(HeartBeatOK));
			sendRet = send(s, sendbuff, sizeof(HeartBeatOK), 0);
            if (sendRet <= 0)
			{
                YYPrintDebug("发送到心跳应答报文 失败      sendRet=", sendRet, DEBUG_INFO);
			}
            else
			{
                YYPrintDebug("发送到心跳应答报文 成功      sendRet=", sendRet, DEBUG_INFO);
			}
			break;
		}
		case AFN_SET_PARAMETER://
		{
			FuncToCharHex(buf, bufToHexChar, len);
			printf("AFN_SET_PARAMETER-------=%s\r\n", bufToHexChar);
			break;
		}
		case AFN_REQUEST_TERMINAL_INFO:
		{
			FuncToCharHex(buf, bufToHexChar, len);
			printf("AFN_REQUEST_TERMINAL_INFO=%s\r\n", bufToHexChar);
			break;
		}
		case AFN_REQUEST_TYPE1:
		{
			FuncToCharHex(buf, bufToHexChar, len);
			printf("AFN_REQUEST_TYPE1 =%s len =%d\r\n", bufToHexChar, len);
			break;
		}
		case AFN_REQUEST_TYPE2:
		{
			FuncToCharHex(buf, bufToHexChar, len);
			printf("AFN_REQUEST_TYPE2 =%s\r\n", bufToHexChar);
			break;
		}
		case AFN_REQUEST_TYPE3:
		{
            fnHign = buf[16];
            fnLow= buf[17];

            pnHign = buf[14];
            pnLow = buf[15];

            //fn = ((fnHign & 0x00FF) << 8) | (fnLow & 0x00FF);
            fn = (fnHign & 0x00FF) | ( (fnLow & 0x00FF)<< 8);
            //pn = ((pnHign & 0x00FF) << 8) | (pnLow & 0x00FF);

            switch(fn)
            {
                case F1://F200
                {
                    YYPrintBegin();
                    YYPrintDebugType1("接收<-故障报警报文    ",  "AFN", AFN_REQUEST_TYPE3, "FN", fn,DEBUG_INFO, len);
                    FuncToCharHex(buf, bufToHexChar, len);
                    printf("%s", bufToHexChar);
                    YYPrintEnd();
                    break;
                }
                default:
                {
                    YYPrintDebug("AFN_REQUEST_TYPE3         fn is error!      fn=", fn, DEBUG_INFO);
                    break;
                }
            }
			break;
		}
		case AFN_REAL_DATA:
		{
			fnHign = buf[16];
			fnLow= buf[17];

			pnHign = buf[14];
			pnLow = buf[15];

            fn = (fnHign & 0x00FF) | ( (fnLow & 0x00FF)<< 8);

            //pn = ((pnHign & 0x00FF) << 8) | (pnLow & 0x00FF);

            switch(fn)//F219	= 0x1B04
			{
                case F219:
				{
                    YYPrintBegin();
                    YYPrintDebugType1("接收<-实时状态数据    ",  "AFN", afn, "FN", fn,DEBUG_INFO, len);
                    FuncToCharHex(buf, bufToHexChar, len);
                    printf("%s", bufToHexChar);
                    YYPrintEnd();
					break;
				}
                case F201:
                {
                    YYPrintBegin();
                    YYPrintDebugType1("接收<-结算信息    ",  "AFN", afn, "FN", fn,DEBUG_INFO, len);
                    FuncToCharHex(buf, bufToHexChar, len);
                    printf("%s", bufToHexChar);
                    YYPrintEnd();

                    YYPrintBegin();
                    YYPrintDebugType1("发送->结算信息应答报文    ",  "AFN", afn, "FN", fn,DEBUG_INFO, len);
                    SendTradeInfoResponse(s);
                    YYPrintEnd();
                    break;
                }
                case F203:
                {
                    YYPrintBegin();
                    YYPrintDebugType1("接收<-预约应答报文    ",  "AFN", afn, "FN", fn,DEBUG_INFO, len);
                    FuncToCharHex(buf, bufToHexChar, len);
                    printf("%s", bufToHexChar);
                    YYPrintEnd();
                    break;
                }
                case F204:
                {
                    YYPrintBegin();
                    YYPrintDebugType1("接收<-停止预约应答报文    ",  "AFN", afn, "FN", fn,DEBUG_INFO, len);
                    FuncToCharHex(buf, bufToHexChar, len);
                    printf("%s", bufToHexChar);
                    YYPrintEnd();
                    break;
                }
				default:
				{
                    YYPrintDebug("AFN_REAL_DATA ------------------        fn is error!      fn=", fn, DEBUG_INFO);
					break;
				}
			}
			break;
		}
		default:
		{
            YYPrintDebug("AFN is error!      AFN=", afn, DEBUG_INFO);
			break;
		}
	}
	return FUNCTION_RETURN_SUCCESS;
}


int ServerSendRequestType1(int s, int fnType)
{
	char					sendbuff[BUF_LEN];
	int sendRet;

	int lenSend;
	
	GWD376_PackagingPacketServer(AFN_REQUEST_TYPE1, sendbuff, &lenSend, fnType);//

	//memcpy(sendbuff, PacketReset, sizeof(PacketReset));
	//sendRet = send(s, sendbuff, sizeof(sendbuff), 0);
	sendRet = send(s, sendbuff, lenSend, 0);
	if(sendRet <= 0)
	{
		printf("ServerSendRequestType1->fail  sendRet:%d\r\n", sendRet);		   
	}
	else
	{
		printf("ServerSendRequestType1->ok  sendRet:%d\r\n", sendRet);
	}
	return FUNCTION_RETURN_SUCCESS;
}
int ServerSendSetParameter(int s, uint16 fn)
{
	char					sendbuff[BUF_LEN];
	int sendRet;

	int lenSend;
	
	GWD376_PackagingPacketServer(AFN_SET_PARAMETER, sendbuff, &lenSend, fn);

	//memcpy(sendbuff, PacketReset, sizeof(PacketReset));
	//sendRet = send(s, sendbuff, sizeof(sendbuff), 0);
	sendRet = send(s, sendbuff, lenSend, 0);
	if(sendRet <= 0)
	{
		printf("send()->fail fn=%d    sendRet:%d\r\n", fn, sendRet); 
	}
	else
	{
		if (fn == 201)
		{
			printf("fn == 201 sendRet:%d\r\n", sendRet);
		}
		else if (fn == 202)
		{
			printf("fn == 202 sendRet:%d\r\n", sendRet);
		}
		else if (fn == 203)
		{
			printf("fn == 203 sendRet:%d\r\n", sendRet);
		}
		else if (fn == 206)
		{
			printf("fn == 206  sendRet:%d\r\n", sendRet);
		}
		else
		{
			printf("fn is error  sendRet:%d\r\n", sendRet);
		}		
	}
	return FUNCTION_RETURN_SUCCESS;
}


int ServerSendRealData(int s, uint16 fn)
{
    char					sendbuff[BUF_LEN];
    int sendRet;

    int lenSend;

    GWD376_PackagingPacketServer(AFN_REAL_DATA, sendbuff, &lenSend, fn);


    sendRet = send(s, sendbuff, lenSend, 0);
    if(sendRet <= 0)
    {
        printf("send()->fail fn=%d    sendRet:%d\r\n", fn, sendRet);
    }
    else
    {
        if (fn == 201)
        {
            printf("fn == 201 sendRet:%d\r\n", sendRet);
        }
        else if (fn == 203)
        {
            printf("fn == 203 sendRet:%d\r\n", sendRet);
        }
        else if (fn == 204)
        {
            printf("fn == 204 sendRet:%d\r\n", sendRet);
        }
        else
        {
            printf("fn is error  sendRet:%d\r\n", sendRet);
        }
    }
    return FUNCTION_RETURN_SUCCESS;
}

int ServerSendResetCmd(int s)
{
	char					sendbuff[BUF_LEN];
	int sendRet;

	int lenSend;
	
	GWD376_PackagingPacketServer(AFN_RESET, sendbuff, &lenSend, INVALID_FN_VALUE);

	//memcpy(sendbuff, PacketReset, sizeof(PacketReset));
	//sendRet = send(s, sendbuff, sizeof(sendbuff), 0);
	sendRet = send(s, sendbuff, lenSend, 0);
	if(sendRet <= 0)
	{
		printf("ServerSendResetCmd->fail  sendRet:%d\r\n", sendRet);		   
	}
	else
	{
		printf("ServerSendResetCmd->ok  sendRet:%d\r\n", sendRet);
	}
	return FUNCTION_RETURN_SUCCESS;
}

int ServerSendControl(int s)
{
	char	sendbuff[BUF_LEN];
	int sendRet;

	int lenSend;
	
	GWD376_PackagingPacketServer(AFN_CONTROL, sendbuff, &lenSend, INVALID_FN_VALUE);

	//memcpy(sendbuff, PacketReset, sizeof(PacketReset));
	//sendRet = send(s, sendbuff, sizeof(sendbuff), 0);
	sendRet = send(s, sendbuff, lenSend, 0);
	if(sendRet <= 0)
	{
		printf("ServerSendControl->fail  sendRet:%d\r\n", sendRet);		   
	}
	else
	{
		printf("ServerSendControl->ok  sendRet:%d\r\n", sendRet);
	}
	return FUNCTION_RETURN_SUCCESS;
}

int ServerSendRequestTerminalInfo(int s)
{
	char sendbuff[BUF_LEN];
	int sendRet;

	int lenSend;
	
	GWD376_PackagingPacketServer(AFN_REQUEST_TERMINAL_INFO, sendbuff, &lenSend, INVALID_FN_VALUE);

	//memcpy(sendbuff, PacketReset, sizeof(PacketReset));
	//sendRet = send(s, sendbuff, sizeof(sendbuff), 0);
	sendRet = send(s, sendbuff, lenSend, 0);
	if(sendRet <= 0)
	{
		printf("ServerSendRequestTerminalInfo->fail  sendRet:%d\r\n", sendRet);		   
	}
	else
	{
		printf("ServerSendRequestTerminalInfo->ok   sendRet:%d\r\n", sendRet);
	}
	return FUNCTION_RETURN_SUCCESS;
}
int ServerRecvAllPacketLinux(int sClient)
{
	int s;
	char   buf[BUF_LEN];
	int   Ret;

	s = sClient;

	memset(buf, '\0',sizeof(buf));
	#if 0//(DEBUG_ALL == 1)
	printf("ServerRecvAllPacketLinux(SOCKET sClient)::recv\n");
	#endif
	Ret = recv(s, buf, BUF_LEN, 0);

	if(Ret > 0)
	{

        printf("收包计数器 counterPackageRev=%4d receive num=%-4d\n", counterPackageRev++, Ret);

        ServerCompareAllPacketLinux(buf, Ret, s);
	}
	return FUNCTION_RETURN_SUCCESS;
}

int SendTradeInfoResponse(int s)//发送结算信息应答报文
{
    char sendbuff[BUF_LEN];
    int sendRet;
    char bufToHexChar[BUF_LEN * 3];
    int lenSend;

    GWD376_PackagingPacketServer(AFN_REAL_DATA, sendbuff, &lenSend, F201);

    FuncToCharHex(sendbuff, bufToHexChar, lenSend);
    printf("%s", bufToHexChar);//打印报文

    sendRet = send(s, sendbuff, lenSend, 0);
    if (sendRet <= 0)
    {
        YYPrintDebug("发送->结算信息应答报文->fail  sendRet", sendRet, DEBUG_INFO);
    }
    else
    {
        YYPrintDebug("发送->结算信息应答报文->OK  sendRet", sendRet, DEBUG_INFO);
    }
    return FUNCTION_RETURN_SUCCESS;
}

uint8 hex2bcd8(uint8 hex) //added by yuanpei@
{
    uint8 ret = 0,retTemp = 0;
    ret = hex%10;
    retTemp = (hex/10)<<4;
    return ret + retTemp;
}

uint16 hex2bcd16(uint16 hex)
{
    uint16 ret[4];
    ret[0] = (hex/1000%10)<<12;
    ret[1] = (hex/100%10)<<8;
    ret[2] = (hex/10%10)<<4;
    ret[3] = (hex/1%10)<<0;

    return ret[0]+ret[1]+ret[2]+ret[3];
}


void LogMsgGDW376(char* msg, int len)
{
    //msg = msg;
    #if 1

    //qDebug()<<"FILE:"<<__FILE__<<"  LINE:"<<__LINE__<<"  FUNC:"<<__FUNCTION__;

    time_t now;    //实例化time_t结构
    struct tm  *timenow;    //实例化tm结构指针

    time(&now);//time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
    timenow = localtime(&now);//localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)

    printf("%s    len=%d\n", msg, len);
    printf("::%s",asctime(timenow));//asctime函数把时间转换成字符，通过printf()函数输出
    #endif
}
//////////////////////////////////////////////////end of file////////////////////////////////////////////////
