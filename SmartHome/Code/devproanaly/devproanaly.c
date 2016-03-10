#include "devproanaly.h"
#include "common.h"

#ifdef OS
	#include "MICO.h"
	#include "SppProtocol.h"
	#include "mico_tcl.h"
	#include "mico_tcl_ota.h"
	#include "geniot.h"
#else
#include "stdio.h"
#include "usart.h"
#include "uartapp.h"
#endif

#define REVFIFOLEN	1024			//接收FIFO长度
u8 revfifobuf[REVFIFOLEN];			//接收FIFOBUFER
FifoTyp framerevfifo;				//数据接收FIFO

#ifndef OS
	#define SENDFIFOLEN	1024			//发送FIFO长度
	u8 sendfifobuf[SENDFIFOLEN];		//发送FIFOBUFFER
	FifoTyp modsendfifo;				//模块数据发送FIFO
	#define UARTSENDLEN		100			//串口发送长度
	u8 uartsendbuf[UARTSENDLEN];		//串口发送BUFFER
#endif

FrameRevParaStu framerevpara;		//帧接收相关参数

#define FRAMEREVCNT		100			//接收定时 单位:ms 不宜太大，绝对不能超过重发时间270MS
#define FRAMESENDCNT	270			//重发定时 单位:ms

TimerParaStu framesendtimer; 		//帧发送定时器接口
TimerParaStu framerevtimer; 		//帧接收定时器接口

DataInterfaceStu dataup;			//设备数据上传接口
FrameSendParaStu framesendpara;		//帧发送相关参数

DataInterfaceStu datadown;			//下行数据到设备接口

const u8 cmdconfirm = CMDCONTROL;
const u8 cmdrddevinfo = CMDRDDEVINFO;
#define WORKMODELEN 3
u8 workmode[WORKMODELEN] = {CMDRETWORKMODE, 0x10, 0x00};
#define MODINFOLEN 14
u8 modinfo[MODINFOLEN] = {CMDRETMODINFO, 0x01, 0x01, 16, 1, 30, 1, 16, 1, 30, 1, 0, 0, 0};
#define LINKSTELEN 3
u8 linkste[LINKSTELEN] = {CMDRETLINKSTE, 0x00, 0x01};

u8 Uart1SendState;

void DevFrameDataHandle();
void DevRevCmdHandle();
void SetDataDownState();
void DevRevTimerHandle();

#if DEBUGFRAMEREV
/******************************************
Fun: 串口发送完中断处理
Input: huart:串口号
Output:void
Return:void
******************************************/
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(&huart1 == huart)
//	{
//		Uart1SendState = 0;
//	}
//}
#endif

/******************************************
Fun: 帧接收参数初始化
Input: void
Output:void
Return:void
******************************************/
//static void FrameRevInit(void)
static __INLINE void FrameRevInit(void)
{
	framerevpara.datastate = REVHEAD;
//	framerevpara.len = 0;
	framerevpara.wordstate = 0;
	//	framerevpara.offset = 0;
}

/******************************************
Fun: 帧发送参数初始化
Input: void
Output:void
Return:void
******************************************/
static __INLINE void FrameSendparaInit(void)
{
	//	framesendpara.len = 0;
	framesendpara.resendcnt = 0;
}

/******************************************
Fun: 接收字节数据字处理
Input: data:数据地址;len:长度
Output:NULL
Return:NULL
******************************************/
void RevDataWrFifo(u8* data, u8 len)
{
	u8 i;
	
	APIReloadTimer(&framerevtimer, FRAMEREVCNT);
	APIStartTimer(&framerevtimer);
	
	for(i=0; i<len; i++)
	{
		if(0 == framerevpara.wordstate)			//上一个数据不是0xF5
		{
			if(FRAMEHEADER == data[i])			//0XF5
			{
				framerevpara.wordstate = 1;
			}
			else							
			{
				if(REVHEAD == framerevpara.datastate) 		//正在接收头
				{
					APIStopTimer(&framerevtimer);
					return;
				}
				else					
				{
					FifoPushOneByte(&framerevfifo, data[i]);
				}
			}
		}
		else
		{
			framerevpara.wordstate = 0;
			
			if(REVHEAD == framerevpara.datastate)		//处理头
			{
				if(FRAMEHEADER == data[i])				//0xF5
				{
					framerevpara.datastate = REVDATA;
					FifoPushOneByte(&framerevfifo, data[i]);				//两个0xF5
					FifoPushOneByte(&framerevfifo, data[i]);
				}
				else
				{
					APIStopTimer(&framerevtimer);
					return;
				}
			}
			else
			{
				if(FRAMEHEADER == data[i])
				{
					DevFrameDataHandle();
					
					FifoPushOneByte(&framerevfifo, data[i]);				//两个0xF5
					FifoPushOneByte(&framerevfifo, data[i]);
				}
				else if(0x55 == data[i])
				{
					FifoPushOneByte(&framerevfifo, 0xF5);			//新加2016-0301
					FifoPushOneByte(&framerevfifo, data[i]);
				}
				else				//错误数据
				{
					framerevpara.errortype = 3;
					FifoDeletHead(&framerevfifo, (framerevfifo.Datalen-framerevpara.offset));
					APIStopTimer(&framerevtimer);
					FrameRevInit();
					return;
				}
			}
		}
	}
}

#if DEBUGFRAMEREV
/******************************************
Fun: 帧接收错误处理
Input: void
Output:void
Return:void
******************************************/
static void DevFrameErrHandle(void)
{
	switch(framerevpara.errortype)
	{
	case 0:
		break;
		
	case 1:
		printf("数据校验错误\r\n");
		break;
		
	case 2:
		printf("数据解密错误\r\n");
		break;
		
	case 3:
		printf("错误帧\r\n");
		break;
		
	case 4:
		printf("校验正确\r\n");
		break;
		
	default:
		break;		
	}
	framerevpara.errortype = 0;
}
#endif

/******************************************
Fun: 帧接收定时时间到处理
Input: void
Output:void
Return:void
******************************************/
void DevRevTimerHandle()
{
	if(0 != framerevtimer.state)
	{
		framerevtimer.state = 0;
		APIStopTimer(&framerevtimer);
		DevFrameDataHandle();
	}
}

/******************************************
Fun: 接收设备数据命令解析
Input: void
Output:void
Return:void
******************************************/
void DevRevCmdHandle(void)
{
	if(0 != framerevpara.offset)
	{
		u8 debugflag = 0;
		static u8 cmdflag = 0;
		static u8 templen = 0;
		static u8 cmd = 0;
		
		if(0 == cmd)
		{
			if(1 == FifoCopy(&templen, &framerevfifo, 3, 1))
			{
				templen += 4;
			}
			else
			{
				debugflag = 1;	
			}
			if(1 != FifoCopy(&cmd, &framerevfifo, 4, 1))
			{
				debugflag = 1;
			}
			
			switch(cmd)
			{
				/*设备返回给网关的命令*/
			case CMDSTE:		
			case CMDINVALID:			
			case CMDCONFIRM:
			case CMDRETDEVID:
			case CMDRETDEVINFO:
			case CMDRETDEVPROFILE:
			case CMDACKDEVPROFILE:
			case CMDACKUPPACK:
			case CMDACKUPAPPCHK:
				datadown.state = 0;					//停止重发
				APIStopTimer(&framesendtimer);
				FrameSendparaInit();
				
				cmdflag = 1;
				break;
				
				/*设备主动发给网关的命令*/
			case CMDALARM:
			case CMDINFOREPORT:
			case CMDRDENCRYPARA:
				framesendpara.sendcmd.bit.confirm = 1;
				cmdflag = 1;
				break;
				
				/*设备主动发起给模块的命令，必须立即处理接收到的数据和命令*/				
			case CMDSETWORKMODE:
				framesendpara.sendcmd.bit.workmode = 1;
				cmdflag = 2;
				break;
				
			case CMDRDMODINFO:
				framesendpara.sendcmd.bit.modinfo = 1;
				cmdflag = 2;
				break;
				
			case CMDRDLINKSTE:
				framesendpara.sendcmd.bit.linkste = 1;
				cmdflag = 2;
				break;
				
			default:
				cmdflag = 2;
				break;
			}
		}
		if(1 == cmdflag)
		{
			if(0 == dataup.state)
			{
				if(templen == FifoCopy((u8*)&dataup, &framerevfifo, 0, templen))
				{
					dataup.state = 2;
					cmdflag = 2;
				}
				else
				{
					debugflag = 1;
				}
			}
		}
		if(2 == cmdflag)
		{
			framerevpara.offset -= templen;
			if(templen == FifoDeletTail(&framerevfifo, templen))
			{
				cmd = 0;
				templen = 0;
				cmdflag = 0;
			}
			else
			{
				debugflag = 1;						
			}
		}
		
		if(0 != debugflag)
		{
			cmd = 0;
			templen = 0;
			cmdflag = 0;
			FifoInit(&framerevfifo, revfifobuf, REVFIFOLEN);
			memset(&framerevpara, 0, sizeof(framerevpara));
		}
	}
}

/******************************************
Fun: 发送命令处理
Input: void
Output:void
Return:void
******************************************/
void SendCmdHandle()
{
	if(0 == datadown.state)
	{
		if(0 != framesendpara.sendcmd.bit.confirm)
		{
			if(DataDownWr((u8*)&cmdconfirm, 1))
			{	
				framesendpara.sendcmd.bit.confirm = 0;
			}
		}
		else if(0 != framesendpara.sendcmd.bit.workmode)
		{
			if(DataDownWr(workmode, WORKMODELEN))
			{
				framesendpara.sendcmd.bit.workmode = 0;			
			}
		}
		else if(0 != framesendpara.sendcmd.bit.modinfo)
		{
			if(DataDownWr(modinfo, MODINFOLEN))
			{
				framesendpara.sendcmd.bit.modinfo = 0;
			}
		}
		else if(0 != framesendpara.sendcmd.bit.linkste)
		{
			if(DataDownWr(linkste, LINKSTELEN))
			{
				framesendpara.sendcmd.bit.linkste = 0;
			}
		}
		else if(0 != framesendpara.sendcmd.bit.versions)
		{
			if(DataDownWr((u8*)&cmdrddevinfo, 1))
			{
				framesendpara.sendcmd.bit.versions = 0;
			}
		}
	}
}

/******************************************
Fun: 完整帧数据处理
Input: void
Output:void
Return:void
******************************************/
void DevFrameDataHandle()
{
	u8 tempbuffer[250]={0};
	DataInterfaceStu tempbuf = {0};
	u8 templen = 0;

	templen = framerevfifo.Datalen-framerevpara.offset;
	FifoCopy(tempbuffer, &framerevfifo, framerevpara.offset, templen);
	FifoDeletHead(&framerevfifo, templen);

	u8 i=0, j=0;
	
	tempbuf.framehead[0] = tempbuffer[i++];			//帧头
	tempbuf.framehead[1] = tempbuffer[i++];			//帧头
	tempbuf.framectrl.byte = tempbuffer[i++];		//控制域 
	tempbuf.framedata.datalen = tempbuffer[i++];	//长度域

	for(j=0; j<tempbuf.framedata.datalen-2; j++,i++)
	{
		tempbuf.framedata.data[j] = tempbuffer[i];
		if(FRAMEHEADER == tempbuffer[i])
		{
			i++;
		}
	}
	
	tempbuf.framedata.data[j] = tempbuffer[i];
	if(FRAMEHEADER == tempbuffer[i])
	{
		tempbuf.framedata.data[j+1] = tempbuffer[i+2];
	}
	else
	{
		tempbuf.framedata.data[j+1] = tempbuffer[i+1];
	}

	u16 tempcrc = 0;
	u8 errortype = 0;

	if(0 != tempbuf.framectrl.bit.encrypt)
	{
		if(OK != Decoding((u8*)&tempbuf.framedata, (templen-3)))//解密长度减去头和控制域
		{
			errortype = 2;
			goto endhandle;
		}
	}
	
	switch(tempbuf.framectrl.bit.crctype)
	{
	case CRCCHECK:
		tempcrc = tempbuf.framedata.data[j]<<8 |tempbuf.framedata.data[j+1];//转换为小端模式
		if(tempcrc != crc_cal(tempbuffer, i))		//从帧头开始校验
		{
			errortype = 1;
		}
		break;
		
	case SUMCHECK:
		if(tempbuf.framedata.data[j] != SumCheck(&tempbuffer[3], (i-3)))		//从长度域开始
		{
			errortype = 1;
		}
		break;
		
	case BCCCHECK:
		if(tempbuf.framedata.data[j] != XorCheck(&tempbuffer[3], (i-3)))//从长度域开始
		{
			errortype = 1;
		}
		break;
		
	default:
		break;
	}
	
endhandle:
	if(0 == errortype)
	{
		framerevpara.errortype = 4;
		if(FifoInput(&framerevfifo, (u8*)&tempbuf, (tempbuf.framedata.datalen+4)))
		{
			framerevpara.offset += tempbuf.framedata.datalen+4;
		}
	}
	else
	{
		framerevpara.errortype = errortype;
	}
	
	FrameRevInit();
}

/******************************************
Fun: 写入发送数据到串口
Input:buf:写入的buf;bufloc:写入的位置;data:转义的数据 
Output:void
Return:true: 写入串口buf成功，false:失败
******************************************/
_Bool WrUartBuf(u8* src,  u8 len)
{
#ifdef OS
	if(0 == MicoUartSend(UART_FOR_APP, src, len))
	{
		return true;
	}
#else
	if(FifoInput(&modsendfifo, src, len))
	{
		return 1;
	}
#endif
	return 0;
}

/******************************************
Fun: 转义一个字节到BUF
Input:buf:写入的buf;bufloc:写入的位置;data:转义的数据 
Output:void
Return:void
******************************************/
void TransData(u8* buf, u8* bufloc, u8 data)
{
	if(FRAMEHEADER != data)
	{
		buf[(*bufloc)++] = data;
	}
	else
	{
		buf[(*bufloc)++] = data;
		buf[(*bufloc)++] = 0x55;
	}
}

/******************************************
Fun: 发送数据到设备
Input:void 
Output:void
Return:true:数据发送成功 false:失败
******************************************/
_Bool DevSendData(void)
{
	u8 i=0;
	u8 j=0;
	u8 tempbuf[250]={0};
	
	tempbuf[i++] = 0xF5;
	tempbuf[i++] = 0xF5;
	tempbuf[i++] = datadown.framectrl.byte;	//控制字
	tempbuf[i++] = datadown.framedata.datalen;		//长度
	
	for(j=0; j<(datadown.framedata.datalen-2); j++)			//减去校验两个字节
	{
		TransData(tempbuf, &i, datadown.framedata.data[j]);
	}
	
	if(CRCCHECK == datadown.framectrl.bit.crctype)
	{
		union_u16 temp;
		
		temp.val= crc_cal(tempbuf, i);				//从头开始校验
		TransData(tempbuf, &i, temp.u8adr[1]);
		TransData(tempbuf, &i, temp.u8adr[0]);
	}
	else if(SUMCHECK == datadown.framectrl.bit.crctype)
	{
		u8 temp = 0;
		
		temp = SumCheck(&tempbuf[3], (i-3));	//从长度开始校验
		TransData(tempbuf, &i, temp);
		TransData(tempbuf, &i, 0);
	}
	else if(BCCCHECK == datadown.framectrl.bit.crctype)
	{
		u8 temp = 0;
		
		temp= XorCheck(&tempbuf[3], (i-3));	//从长度开始校验
		TransData(tempbuf, &i, temp);
		TransData(tempbuf, &i, 0);
	}
	
	if(WrUartBuf(tempbuf,i))		//成功写入串口BUF
	{
		PrintfMem(tempbuf,i);
		return 1;
	}
	return 0;
}

/******************************************
Fun: 接收定时器时间到
Input: void
Output:void
Return:void
******************************************/
void DevRevTimerUp(void* arg)
{
	framerevtimer.state = 1;
}

/******************************************
Fun: 发送定时器时间到
Input: void
Output:void
Return:void
******************************************/
void DevSendTimerUp(void* arg)
{
	framesendtimer.state = 1;
}

/******************************************
Fun: 帧发送定时时间到处理
Input: void
Output:void
Return:void
******************************************/
static void DevSendTimerHandle()
{
	if(0 != framesendtimer.state)
	{
		framesendtimer.state = 0;		
		if(RESENDCNT != framesendpara.resendcnt)
		{
			framesendpara.resendcnt++;		
			DevSendData();	//重发
		}
		else
		{
			datadown.state = 0;
			
			APIStopTimer(&framesendtimer);
			
			FrameSendparaInit();	//初始化所有发送参数
		}
	}
}


/******************************************
Fun: 下行数据处理
Input: void
Output:void
Return:void
******************************************/
void DatadownHandle(void)
{
	if(2 == datadown.state)
	{
		u8 tempcmd = 0;
#ifdef OLD
		tempcmd = datadown.cmd;
#else
		tempcmd = datadown.framedata.data[0];
#endif	
		if(!DevSendData())	//发送数据
		{
			return;
		}
		
		switch(tempcmd)
		{
#ifdef OLD
		datadown.state = 3;
#else
		case CMDRSSI:
		case CMDCONFIRM:
		case CMDSTOPALARM:
		case CMDRETENCRYPARA:
		case CMDRETLINKSTE:
		case CMDRETWORKMODE:
		case CMDRETMODINFO:			
			datadown.state = 0;
			break;
			
		case CMDCONTROL:
		case CMDINFOREPORTCFG:
//		case CMDCHECKSTE:
		case CMDRDDEVID:
		case CMDRDDEVINFO:
			//			case CMDBPSTEST:				
		case CMDDEVRST:
		case CMDRDDEVPROFILE:
		case CMDWRDEVPROFILE:
		case CMDUPPACK:
		case CMDUPAPPCHK:
			datadown.state = 3;
#endif			
			APIReloadTimer(&framesendtimer, FRAMESENDCNT);
			APIStartTimer(&framesendtimer);
			
			break;
			
		default:
			break;
		}
	}
}

/******************************************
Fun: 串口一发送处理
Input: void
Output:void
Return:void
******************************************/
#ifndef OS
void Uart1DataHandle(void)
{
//	if(1 != Uart1SendState)		//FIFO数据通过串口发出
//	{
//		if(0 != modsendfifo.Datalen)
//		{
//			u16 len;
//			
//			Uart1SendState = 1;
//			len = FifoOutput(uartsendbuf, &modsendfifo, UARTSENDLEN);
//			HAL_UART_Transmit_IT(&huart1, uartsendbuf, len);
//		}
//	}
//	if(0 != modsendfifo.Datalen)
//	{
//		u8 c;
//		FifoOutput(&c, &modsendfifo, 1);
//		UART_PutChar (UART1, c); 
//		//HAL_UART_Transmit_IT(&huart1, uartsendbuf, len);
//	}
}
#endif

/******************************************
Fun: 帧处理回调函数
Input: datalen:cmd+data的长度;data:cmd与data的buf地址
Output:void
Return:0:数据没处理 1:数据已处理
******************************************/
__weak _Bool AppHandleCallback(u8* data, u8 datalen)
{
	/*用户自己填写*/
	PrintfMem((u8*)&dataup, (dataup.framedata.datalen+4));
	return 1;
}

/******************************************
Fun: 判断数据状态
Input: void
Output:void
Return:0:数据没准备好 1:数据已准备好
******************************************/
_Bool IsDataUpOk()
{
	if(2 == dataup.state)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/******************************************
Fun: 判断数据状态
Input: void
Output:void
Return:void
******************************************/
void InitDataUpState()
{
	dataup.state = 0;
}

/******************************************
Fun: 写入下行数据
Input: data:cmd与data的buf地址, datalen:cmd+data的长度;
Output:void
Return:0:写入失败 1:数据已写入
******************************************/
_Bool DataDownWr(u8* data, u8 datalen)
{
	if(0 == datadown.state)
	{
		if(datalen <= 128)
		{
			datadown.framedata.datalen = datalen+2;
			memcpy((u8*)&datadown.framedata.data, data, datalen);
			
			datadown.state = 2;
			return 1;
		}
	}
	return 0;
}

/******************************************
Fun: 帧处理主循环
Input: void
Output:void
Return:void
******************************************/
void DevFrameHandle()
{	
	if(IsDataUpOk())
	{
		if(AppHandleCallback((u8*)&dataup.framedata.data, (dataup.framedata.datalen-2)))
		{
			InitDataUpState();
		}
	}
	DevRevCmdHandle();

	DevRevTimerHandle();
	DevSendTimerHandle();
	
	
#if DEBUGFRAMEREV
	DevFrameErrHandle();
#endif
	
	DatadownHandle();
	SendCmdHandle();

#ifndef OS
	Uart1DataHandle();
#endif
}

#ifndef OS
/******************************************
Fun: 串口初始化
Input: void
Output:void
Return:void
******************************************/
void UartInit()
{
	//	HAL_UART_Receive_IT(&huart1, uart1revbyte, REVBYTES);
	//__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

/******************************************
Fun: 硬件初始化
Input: void
Output:void
Return:void
******************************************/
void DevHardInit()
{
	UartInit();
}
#endif

#ifdef TESTTIMER
TimerParaStu testtimer;
void TimerTest(void* arg)
{
//	u8 tempbuf[3] = {0x08,0xf5,0xf5};
//	DataDownWr(tempbuf,3);
	framesendpara.sendcmd.bit.versions = 1;
}
#endif
/******************************************
Fun: 参数初始化
Input: void
Output:void
Return:void
******************************************/
void DevParaInit()
{
#ifdef TESTTIMER
	APIInitTimer(&testtimer, 10000, (void*)TimerTest, NULL);			//测试定时器
	APIStartTimer(&testtimer);
#endif

#ifdef OLD
#else
	datadown.framehead[0] = 0xf5;		//帧头
	datadown.framehead[1] = 0xf5;		//帧头
	datadown.framectrl.byte = 0;		//帧控制字
#endif

	APIInitTimer(&framerevtimer, FRAMEREVCNT, (void*)DevRevTimerUp, NULL);			//绑定接收定时器
	APIInitTimer(&framesendtimer, FRAMESENDCNT, (void*)DevSendTimerUp, NULL);		//绑定发送定时器
	
	FifoInit(&framerevfifo, revfifobuf, REVFIFOLEN);								//初始化接收FIFO
	
#ifndef OS
	FifoInit(&modsendfifo, sendfifobuf, SENDFIFOLEN);							//初始化发送FIFO
#endif /* OS */
}
