/*
 * Gwd376Linux.h
 *
 *  Created on: Oct 19, 2016
 *      Author: lang
 */

#ifndef GWD376LINUX_H_
#define GWD376LINUX_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

/************************************************************************************************************
*|     start   |                          user area                                                | over  |*
*|68 len len 68|control|adress|  afn  |  seq  | (data unit sign)1|(data)1|(data unit sign)n|(data)n|crc|end|*
*
*|-------6-----| 1byte | 5byte| 1byte | 1byte | pn 2byte+fn 2byte| null  |    null         |       | 1 | 1 |*
*
*|-buf[0..5]---|buf[6]-|buf[7]|buf[12]|buf[13]| buf[14 15 16 17] |*
************************************************************************************************************/

#define DEBUG_ALL					1
#define BUF_LEN 1024

#define FUNCTION_RETURN_SUCCESS		0
#define FUNCTION_RETURN_FAIL		-1

#define uint16 short
#define uint32 int
#define uint8 unsigned char

#define PACKET_BEGIN	0x68
#define PACKET_END		0x16
#define USER_DATA_LEN	512
#define HEARTBEAT_INTERVAL 10

#define GDW376_PRINT_LOG
//#define AFN_LOGIN			0
//#define AFN_HEARTBEAT		1
#define AFN_OK_OR_NOT	0x00
#define AFN_RESET		0x01
#define AFN_HEARTBEAT	0x02

#define AFN_SET_PARAMETER		0x04
#define AFN_CONTROL				0x05
#define AFN_REQUEST_TERMINAL_INFO		0x09

#define AFN_REQUEST_TYPE1		0x0C
#define AFN_REQUEST_TYPE2		0x0D
#define AFN_REQUEST_TYPE3		0x0E

#define AFN_REAL_DATA			0x10
//end of GWD376

#define TERMINAL_AREA		0x0234
#define TERMINAL_ADDRESS	10000

typedef enum
{
    F1		= 0x0001,
    F2		= 0x0002,
    F3      = 0x0004,
    F4      = 0x0008,

    F31		= 0x0340,		//0x1E,
    F177 	= 0x1601,		//0XB0,
    F178	= 0X1602,
    F179	= 0X1604,

    F200	= 0x1880,//modified by Y@
    F201	= 0x1901,//modified by Y@
    F202	= 0x1902,//modified by Y@
    F203	= 0x1904,//modified by Y@
    F204	= 0x1908,//modified by Y@
    F205	= 0x1910,//modified by Y@
    F206	= 0x1920,//modified by Y@
    F207	= 0x1940,//modified by Y@

    F219	= 0x1B04,//modified by hjedison
    F220    = 0x1B08,
    F226	= 0x1c02,//modified by Y@
    F227	= 0x1c04,//modified by Y@
    F228	= 0x1c08//modified by Y@
}FN;
typedef struct //
{
    uint16  Year;
    uint8   Month;
    uint8   Day;
}ApptDate;
typedef struct
{
    uint8   Hour;
    uint8   Minute;
    uint8   Second;
}ApptTime;//
#pragma pack(push)
#pragma pack(1)
typedef struct
{
	uint8	TerminalNo[6];
	uint8	TerminalIo;
	uint8	TerminalIoType;
	uint8	IoLinkState;
	uint8	IoWorkState;
	uint8	DateState;
	uint8	MeterType;
	uint32	MeterRate;

	uint16	OutPutVolt;
	uint16	OutPutCurr;
	uint8	FaultCode;
	uint8	ErrCode;
	uint8	Soc;
	uint16	LeftTime;
	uint8	MeterAddr[6];
	uint8	ActivePower[3];

	uint8	ReactivePower[3];
	uint32	MeterActiveEnergy;
	uint32	MeterReactiveEnergy;
	uint8	HappenTime[7];
}REAL_TIME_STATE_UP_DATA;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct
{
	uint8	TerminalNo[8];
	uint8	TradeNo[6];
	uint8	TradeCardNo[6];
	uint8	TerminalIo;
	uint8	CardNo[8];
	uint8	MeterAddr[6];
	uint32	BefAllEnergy;
	uint16	AftAllEnergyMinus;

	uint32	BefTineEnergy;
	uint16	AftTineEnergyMinus;
	uint32	BefPeakEnergy;
	uint16	AftPeakEnergyMinus;
	uint32	BefFlatEnergy;
	uint16	AftFlatEnergyMinus;
	uint32	BefVallyEnergy;
	uint16	AftVallyEnergyMinus;

	uint32	MoneyBefCharge;
	uint32	MoneyAftCharge;
	uint16	AllPowerMoney;
	uint16	AllServiceMoney;

	uint8	TineElectPrice[3];
	uint16	TineServePrice;
	uint16	TineKwh;
	uint16	TIneMoney;
	uint16	TineServeMoney;

	uint8	PeakElectPrice[3];
	uint16	PeakServePrice;
	uint16	PeakKwh;
	uint16	PeakMoney;
	uint16	PeakServeMoney;

	uint8	FlatElectPrice[3];
	uint16	FlatServePrice;
	uint16	FlatKwh;
	uint16	FlatMoney;
	uint16	FlatServeMoney;

	uint8	VallyElectPrice[3];
	uint16	VallyServePrice;
	uint16	VallyKwh;
	uint16	VallyMoney;
	uint16	VallyServeMoney;

	uint8	StartChargeTime[6];
	uint16	ChargingTime;
	uint8	OverChargeFlag;
}REQUEST_TERMINAL_CARD_TRADE_DOWN_DATA;
#pragma pack(pop)//

#pragma pack(push)
#pragma pack(1)
typedef union
{
	uint8 c;
	struct
	{
		uint8 DIR:1;
		uint8 PRM:1;
		uint8 ACD:1;
		uint8 RES:1;
		uint8 FUNC:4;
	}BitArea;
}ControllArea;

typedef struct
{
	uint16 A1;//
	uint16 A2;//
	uint8  A3;//
}AdressArea;


typedef struct
{
	uint16 pn;//
	uint16 fn;//

}GWD376_DataUnitSign;

typedef struct
{
	uint8  Second;
	uint8  Minute;
	uint8  Hour;
	uint8  Day;
}TP_TIME;

typedef struct
{
	uint8	TpCount;
	TP_TIME	TpTime;
	uint8	Delay;
}TIME_LABLE;

typedef struct
{
	uint8 afn;//
	uint8 seq;//
	//
	//DataUnitSign dataUnitSign;//
}AppLayer;

typedef struct
{
	ControllArea controllArea;//
	AdressArea adressArea;//
	AppLayer appLayer;
}UserDataArea;

typedef union
{
	uint16 length;
	struct
	{
		uint16 len2:2;
		uint16 len1:14;
	}LenBitArea;
}Len;

typedef struct
{
	uint8 beginSign;
	Len len;
	Len len1;
	uint8 beginSign1;
}Begin;

typedef struct
{
	Begin begin;
    UserDataArea userDataArea;
	uint8 checkSum;
	uint8 end;
}GWD376_PACKET;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
	uint8 secend;//
	uint8 minute;//
	uint8 hour;//
	uint8 day;//
	uint8 month;//
	uint8 year; //
}GWD376_CONTROL_CMD_TIME;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
	uint8	TradeNo[6];	
	uint8	TerminalId[6];	
	uint8	TerminalIo;	
    uint8   UserId[8];
	uint8	status;
	//uint8	setStatus;
	//uint8	failReason;
	//uint8	soc;
}CONTROL_TERMINA_DOWN_DATA;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct
{
    uint8 cardId[8];
    uint8	tradeNo[6];
}TRADE_DOWN_DATA;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    uint8 SettlementTerminal[6];
    uint8 TerminalIo;
    uint8 CardId[8];
    ApptDate BookDate;
    ApptTime BookTime;
    uint8 BookLength;
    uint8 ImBookLength;
}AppointmentChargeDownData;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct
{
    uint8 terminalID[6];    //无线模块号
    uint8 gunNO;            //充电口
    uint8 cardNumber[8];	//卡号
    //UInt8 bookResult;       //(0未预约 1预约)
}CANCEL_BOOK_DATA;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    uint8	Time[7];
    uint8	TineElectPrice[3];
    uint8	PeakElectPrice[3];
    uint8	FlatElectPrice[3];
    uint8	VallyElectPrice[3];
}ELECT_PRICE;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    uint8	Time[7];
    uint8	TineServerPrice[2];
    uint8	PeakServerPrice[2];
    uint8	FlatServerPrice[2];
    uint8	VallyServerPrice[2];
}SERVER_PRICE;
#pragma pack(pop)


typedef enum
{ 
    DO_NOTHING = 0xff,//
    //CMD_RESET = 1,//
}CMD;

#define FN_NOTHING 0xff//fn
#define INVALID_FN_VALUE 0xFFFF

extern int G_CMD;
extern int G_FN;
extern int G_CHARGING_CONTROL;
extern int counterPackageRev;
extern int LoginRecvLinux(int sClient);
extern int ServerRecvAllPacketLinux(int sClient);
extern int CreatInputCMD(void);
extern int ServerSendRequestType1(int s, int fnType);
extern int ServerSendResetCmd(int s);
extern int ServerSendSetParameter(int s, uint16 fn);
extern int ServerSendControl(int s);
extern int ServerSendRequestTerminalInfo(int s);
extern int ServerSendRealData(int s, uint16 fn);
extern int SendTradeInfoResponse(int s);//发送结算信息应答报文
extern uint8 hex2bcd8(uint8 hex);
extern uint16 hex2bcd16(uint16 hex);
extern void LogMsgGDW376(char* msg, int len);
#endif /* GWD376LINUX_H_ */
//////////////////////////////////////////////////end of file////////////////////////////////////////////////
