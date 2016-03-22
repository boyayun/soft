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
#ifdef OLD
    #define FRAMESENDCNT	330			//重发定时 单位:ms
#else
    #define FRAMESENDCNT	270			//重发定时 单位:ms
#endif

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

#ifdef OLD
u8 laststate[4];
#endif

void FrameDataHandle();
void RevCmdHandle();
void SetDataDownState();
void RevTimerHandle();

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

#ifdef OLD
	#define HEADNUMER 4
#else
	#define HEADNUMER 2
#endif
	for(i=0; i<len; i++)
	{
		if(REVHEAD == framerevpara.datastate) 		//正在接收头
		{
			if(FRAMEHEADER == data[i])			//0XF5
			{
				framerevpara.wordstate++;
				if(HEADNUMER == framerevpara.wordstate)
				{
                    while(framerevpara.wordstate)
                    {
                        FifoPushOneByte(&framerevfifo, FRAMEHEADER);
                        framerevpara.wordstate--;
                    }
					framerevpara.datastate = REVDATA;
				}
			}
			else
			{
				framerevpara.wordstate = 0;
				APIStopTimer(&framerevtimer);
				return;
			}
		}
		else
		{
			if(FRAMEHEADER == data[i])			//0XF5
			{
				framerevpara.wordstate++;
				if(HEADNUMER == framerevpara.wordstate)
				{
					FrameDataHandle();

					while(framerevpara.wordstate)
                    {
                        FifoPushOneByte(&framerevfifo, FRAMEHEADER);
                        framerevpara.wordstate--;
                    }
				}
			}
#ifdef OLD
			else if((0 != framerevpara.wordstate) && (0x55 != data[i]))
			{
				framerevpara.errortype = 3;
				FifoDeletHead(&framerevfifo, (framerevfifo.Datalen-framerevpara.offset));
				APIStopTimer(&framerevtimer);
				FrameRevInit();
				return;
			}
#endif
			else
			{
				while(framerevpara.wordstate)
				{
					FifoPushOneByte(&framerevfifo, FRAMEHEADER);
                    framerevpara.wordstate--;
				}
				FifoPushOneByte(&framerevfifo, data[i]);
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
static void FrameErrHandle(void)
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
void RevTimerHandle()
{
	if(0 != framerevtimer.state)
	{
		framerevtimer.state = 0;
		APIStopTimer(&framerevtimer);
		FrameDataHandle();
	}
}

/******************************************
Fun: 协议转换
Input: new:新协议地址 old:旧协议地址
Output:void
Return:void
******************************************/
void OldToNew(u8* newbuf, u8* old)
{
	u8 i = 0;

	if(0 != old[8+15])
	{
	    newbuf[i++] = 0xF5;
	    newbuf[i++] = 0xF5;
	    newbuf[i++] = 0;
	    
		newbuf[i++] = 9;			//长度固定
		newbuf[i++] = CMDALARM;		//CMD
		newbuf[i++] = 0;			//报警两字节
		newbuf[i++] = old[8+15];

		if(FifoInput(&framerevfifo, newbuf, (newbuf[3]+4)))
        {
			framerevpara.offset += newbuf[3]+4;
		}
		i = 0;
	}

    newbuf[i++] = 0xF5;
    newbuf[i++] = 0xF5;
    newbuf[i++] = 0;

	newbuf[i++] = 18;			//长度固定
	newbuf[i++] = CMDSTE;		//CMD
	
	if(0x01 == old[8+3])		//开关
	{
		newbuf[i++] = 1;
	}
	else if(0x02 == old[8+3])	//关机
	{
		newbuf[i++] = 0;
	}

	if(0 == (0x01 & old[8+4]))	//自动
	{
		newbuf[i++] = 0;
	}
	else
	{
		newbuf[i++] = 1;
	}

	if(0 == (0x10 & old[8+5]))	//加湿
	{
		newbuf[i++] = 0;
	}
	else
	{
		newbuf[i++] = 1;
	}

	newbuf[i++] = old[8+6];		//风量
	newbuf[i++] = old[8+7];		//定时
	newbuf[i++] = old[8+8];		//空气质量
	newbuf[i++] = old[8+9];		//滤网更换
	
	newbuf[i++] = old[8+11];		//滤网信息H
	newbuf[i++] = old[8+10];		//滤网信息L
	newbuf[i++] = old[8+13];		//灰尘传感器H
	newbuf[i++] = old[8+12];		//灰尘传感器L

	newbuf[i++] = 0;				//温度H
	if(50 <= old[8+16])			//温度L
	{
		newbuf[i++] = old[8+16] - 50;		
	}
	else
	{
		newbuf[i++] = 50 - old[8+16];		
	}

	newbuf[i++] = 0;				//湿度H
	newbuf[i++] = old[8+17];		//湿度L
}

/******************************************
Fun: 接收设备数据命令解析
Input: void
Output:void
Return:void
******************************************/
void RevCmdHandle(void)
{
	if(0 != framerevpara.offset)
	{
		u8 debugflag = 0;
		static u8 cmdflag = 0;
		static u8 templen = 0;
		static u8 cmd = 0;

		if(0 == cmd)
		{
			FifoCopy(&cmd, &framerevfifo, 4, 1);
			FifoCopy(&templen, &framerevfifo, 3, 1);
            templen += 4;
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
	                if(3 == datadown.state)
	                {
	                    datadown.state = 0;					//停止重发
	                    APIStopTimer(&framesendtimer);
	                    FrameSendparaInit();
	                }
					cmdflag = 1;
					break;
					
					/*设备主动发给网关的命令*/
				case CMDALARM:
#ifdef OLD
                      cmdflag = 1;
                      break;
#endif                      
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
				FifoCopy((u8*)&dataup, &framerevfifo, 0, templen);
				dataup.state = 2;
				cmdflag = 2;
			}
		}
		if(2 == cmdflag)
		{
			framerevpara.offset -= templen;
			FifoDeletTail(&framerevfifo, templen);
			cmd = 0;
			templen = 0;
			cmdflag = 0;
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
void FrameDataHandle()
{
    DataInterfaceStu tempbuf = {0};
	u8 tempbuffer[250]={0};
	u8 templen = 0;
	u8 i=0, j=0;
	
	templen = framerevfifo.Datalen-framerevpara.offset;
	FifoCopy(tempbuffer, &framerevfifo, framerevpara.offset, templen);
	FifoDeletHead(&framerevfifo, templen);

#ifdef OLD
	u16 tempcrc = 0;
	u8 errortype = 0;

	tempcrc = tempbuffer[templen-2]<<8 | tempbuffer[templen-1];//转换为小端模式
	if(tempcrc != crc_cal(tempbuffer, (templen-2)))		//从帧头开始校验
	{
		errortype = 1;
	}
	else
	{
		memcpy(laststate, &tempbuffer[8+4], 4);
		OldToNew((u8*)&tempbuf, tempbuffer);
	}
#else
	tempbuf.framehead[i] = tempbuffer[i++];			//帧头
	tempbuf.framehead[i] = tempbuffer[i++];			//帧头
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
#endif

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
Input:firstsend:0:重发，1:第一次发送
Output:void
Return:true:数据发送成功 false:失败
******************************************/
_Bool SendData(u8 firstsend)
{
	u8 static i=0;
	u8 static tempbuf[250]={0};
	if(0 != firstsend)
	{
#ifdef OLD	
//		u16 funnum = 0;
		u16 funindex = 0;
	    
//		funnum = dataup.framedata.data[1]<<8 | dataup.framedata.data[2];
		funindex = datadown.framedata.data[3]<<8 | datadown.framedata.data[4];
		
		tempbuf[0] = 0xF5;
		tempbuf[1] = 0xF5;
		tempbuf[2] = 0xF5;
		tempbuf[3] = 0xF5;
		tempbuf[4] = 0;
		tempbuf[5] = 1;
		tempbuf[6] = 0x03;			//cmd
		tempbuf[7] = 11;			//len
		tempbuf[8+0] = 1;			//版本H
		tempbuf[8+1] = 0;			//版本L
        tempbuf[8+2] = 0;			//开关机
		tempbuf[8+6] = 11;			//网络状态

		memcpy(&tempbuf[11], laststate, 4);	//上次的状态
		
		if(11 == funindex)		//查询全部状态
		{
			tempbuf[6] = 0x04;			//cmd
			memset(&tempbuf[10], 0, 9);
		}
		
		else if(31 == funindex)
		{
			if(0 == datadown.framedata.data[5])
			{
				tempbuf[8+2] = 2;	//关机
			}
			else
			{
				tempbuf[8+2] = 1;	//开机
			}
		}
		else if(5005 == funindex)		//自动
		{
			tempbuf[8+3] = datadown.framedata.data[5];
		}
		else if(5022 == funindex)		//加湿
		{
			if(0 == datadown.framedata.data[5])
			{
				tempbuf[8+4] = 0;
			}
			else
			{
				tempbuf[8+4] = 0x10;
			}
		}
		else if(608 == funindex)		//风量
		{
			tempbuf[8+5] = datadown.framedata.data[5];
		}
		else if(5023 == funindex)		//定时
		{
			tempbuf[8+5] = datadown.framedata.data[5];
		}

		u16 tempcrc;
		tempcrc = crc_cal((u8*)&tempbuf, 19);				//从头开始校验
		tempbuf[8+11] = tempcrc >> 8;			//crcH
		tempbuf[8+12] = tempcrc & 0xff;			//crcL
        i = 21;
#else
		u8 j=0;

		i = 0;
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
#endif
	}
	if(WrUartBuf(tempbuf,i))		//成功写入串口BUF
	{
		PrintfMem(tempbuf,i);
		//LOG
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
void RevTimerUp(void* arg)
{
	framerevtimer.state = 1;
}

/******************************************
Fun: 发送定时器时间到
Input: void
Output:void
Return:void
******************************************/
void SendTimerUp(void* arg)
{
	framesendtimer.state = 1;
}

/******************************************
Fun: 帧发送定时时间到处理
Input: void
Output:void
Return:void
******************************************/
static void SendTimerHandle()
{
	if(0 != framesendtimer.state)
	{
		framesendtimer.state = 0;		
		if(RESENDCNT != framesendpara.resendcnt)
		{
			framesendpara.resendcnt++;		
			SendData(0);	//重发
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

		tempcmd = datadown.framedata.data[0];
		if(!SendData(1))	//发送数据
		{
			return;
		}
#ifdef OLD
        datadown.state = 0;
//        if(11 == datadown.framedata.data[4])
//        {
//            datadown.state = 3;
//            APIReloadTimer(&framesendtimer, FRAMESENDCNT);
//            APIStartTimer(&framesendtimer);
//        }
//        else
//        {
//            datadown.state = 0;
//            APIStopTimer(&framesendtimer);
//            
//            u8 tempbuf[5] = {0};
//            tempbuf[0] = CMDCONTROL;	//控制 
//            tempbuf[4] = 11;			//查询
//            DataDownWr(tempbuf, 5);		//发送查询指令
//        }
#else       
		switch(datadown.framedata.data[0])
		{
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
			APIReloadTimer(&framesendtimer, FRAMESENDCNT);
			APIStartTimer(&framesendtimer);
			break;
			
		default:
			break;
		}
#endif	
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
//Fun: 帧处理回调函数
//Input: datalen:cmd+data的长度;data:cmd与data的buf地址
//Output:void
//Return:0:数据没处理 1:数据已处理
//******************************************/
//__weak _Bool AppHandleCallback(u8* data, u8 datalen)
//{
//	/*用户自己填写*/
//	PrintfMem((u8*)&dataup, (dataup.framedata.datalen+4));
//	return 1;
//}

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
void FrameHandle()
{
	if(IsDataUpOk())
	{
		if(AppHandleCallback((u8*)&dataup.framedata.data, (dataup.framedata.datalen-2)))
		{
			InitDataUpState();
		}
	}
	RevCmdHandle();

	RevTimerHandle();
	SendTimerHandle();
	
	
#if DEBUGFRAMEREV
	FrameErrHandle();
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

TimerParaStu testtimer;
void TimerTest(void* arg)
{
	datadown.state = 0;
	u8 tempbuf[5] = {0};
	tempbuf[0] = CMDCONTROL;	//控制 
	tempbuf[4] = 11;			//查询
	DataDownWr(tempbuf, 5); 	//发送查询指令
}
/******************************************
Fun: 参数初始化
Input: void
Output:void
Return:void
******************************************/
void DevParaInit()
{
	APIInitTimer(&testtimer, 2000, (void*)TimerTest, NULL);			//测试定时器
	APIStartTimer(&testtimer);

	datadown.framehead[0] = 0xf5;		//帧头
	datadown.framehead[1] = 0xf5;		//帧头
	datadown.framectrl.byte = 0;		//帧控制字

	APIInitTimer(&framerevtimer, FRAMEREVCNT, (void*)RevTimerUp, NULL);			//绑定接收定时器
	APIInitTimer(&framesendtimer, FRAMESENDCNT, (void*)SendTimerUp, NULL);		//绑定发送定时器
	
	FifoInit(&framerevfifo, revfifobuf, REVFIFOLEN);								//初始化接收FIFO
	
#ifndef OS
	FifoInit(&modsendfifo, sendfifobuf, SENDFIFOLEN);							//初始化发送FIFO
#endif /* OS */
}
