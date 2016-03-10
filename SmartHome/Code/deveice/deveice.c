#include "deveice.h"
#include "stdio.h"
#include "usart.h"
#include "common.h"
#include "lcddriver.h"
#include "apprtc.h"

u8 Uart2SendState;
#define MODSENDLEN		100
u8 modsendbuf[MODSENDLEN];

StuKeyPara keypara;
StuKeyValue keyvalue;

#define REVFIFOLEN	1024			//接收FIFO长度
u8 revfifobuf[REVFIFOLEN];			//接收FIFOBUFER
FifoTyp framerevfifo;				//数据接收FIFO

#define MODSENDFIFOLEN	1024		//发送FIFO长度
u8 modsendfifobuf[MODSENDFIFOLEN];	//数据发送FIFO
FifoTyp modsendfifo;				//模块数据发送FIFO

FrameRevParaStu framerevpara;		//帧接收相关参数
TimerParaStu framerevtimer; 		//帧接收定时器接口
DataInterfaceStu dataup;			//设备数据上传接口

FrameSendParaStu framesendpara;		//帧发送相关参数
TimerParaStu framesendtimer; 		//帧发送定时器接口
DataInterfaceStu datadown;			//下行数据到设备接口

/*设备相关参数*/
DevStateStuType devstate;			//设备状态
u8 cmdsend;				//要发送的命令
u8 signal;				//信号等级
const u8 devid[DEVIDLEN] = "69245757160260316150723123456780124";
const DevInfoStuType devinfo = {{16, 01, 26}, {16, 01, 26}, 0};
const u8 testbuf[3] = {0x01, 0x00};
InfoReportStuType inforeportpara;		//信息汇报参数
AlarmStuType alarmpara;				//报警参数
/*设备相关参数*/

void Uart2DataHandle(void)
{
	if(1 != Uart2SendState)		//FIFO数据通过串口发出
	{
		if(0 != modsendfifo.Datalen)
		{
			u16 len;
			
			Uart2SendState = 1;
			len = FifoOutput(modsendbuf, &modsendfifo, MODSENDLEN);
			HAL_UART_Transmit_IT(&huart1, modsendbuf, len);
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(&huart1 == huart)
	{
		Uart2SendState = 0;
	}
}

/******************************************
Fun: 帧接收参数初始化
Input: void
Output:void
Return:void
******************************************/
static void FrameRevInit(void)
//static __INLINE void FrameRevInit(void)
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
static void FrameSendparaInit(void)
//static __INLINE void FrameSendparaInit(void)
{
	framesendpara.len = 0;
	framesendpara.resendcnt = 0;;	
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
static void DevRevTimerHandle(void)
{
	if(0 != framerevtimer.state)
	{
		framerevtimer.state = 0;
		APIStopTimer(&framerevtimer);
		DevFrameDataHandle();
	}
}

void FormatSend(u8 cmd, u8* src, u8 len)
{
	datadown.framedata.datalen = len + 3;
	datadown.framedata.data[0] = cmd;
	if(0 != len)
	{
		memcpy(&datadown.framedata.data[1], src, len);
	}
	datadown.state = 2;
}

/******************************************
Fun: 接收设备数据命令解析
Input: cmd:命令
Output:void
Return:void
******************************************/
static void DevCtrlHandle()
{
	u16 funnum = 0;
	u16 funindex = 0;
    
	funnum = dataup.framedata.data[1]<<8 | dataup.framedata.data[2];
	funindex = dataup.framedata.data[3]<<8 | dataup.framedata.data[4];
    
    u8 i = 5;
	switch(funindex)		//功能号
	{
		case 31:		//开关
		devstate.power = dataup.framedata.data[i++];
		lcdpara.disnum = 2;
		break;

#ifndef KONGTIAO			
		case 5760:		//模式
		devstate.mode = dataup.framedata.data[i++];
		lcdpara.disnum = 1;		
#else
        case 5601:
		devstate.mode = dataup.framedata.data[i++];
		lcdpara.disnum = 1;
		break;
			
//		case 38:		//日期
//		devstate.date[0] = dataup.framedata.data[i++];
//		devstate.date[1] = dataup.framedata.data[i++];
//		devstate.date[2] = dataup.framedata.data[i++];
//        devstate.date[3] = dataup.framedata.data[i++];
//		lcdpara.disnum = 4;
//		SetRTCDate();
//		break;
//		
//		case 39:		//时间
//		devstate.time[0] = dataup.framedata.data[i++];
//		devstate.time[1] = dataup.framedata.data[i++];
//		devstate.time[2] = dataup.framedata.data[i++];
//		lcdpara.disnum = 6;
//		SetRTCTime();
//		break;
		
		case 611:		//温度
		devstate.temparature = dataup.framedata.data[i++];
		lcdpara.disnum = 0;
		break;

		case 5602:		//风量
		devstate.wind = dataup.framedata.data[i++];
		lcdpara.disnum = 3;
		break;
#endif
		default:
		break;
	}
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

void SendCmdHandle()
{	
	if(0 == datadown.state)
	{
		if(3 == dataup.state)
		{
			dataup.state = 0;
			
			switch(dataup.framedata.data[0])
			{
				case CMDCONTROL:
//				case CMDCHECKSTE:
					FormatSend(CMDSTE, (u8*)&devstate, sizeof(DevStateStuType));	//发送状态帧
					break;
				
				case CMDINFOREPORTCFG:
				{
					u16 temp = 0;
					temp = dataup.framedata.data[1]<<8 | dataup.framedata.data[2];
					if(temp >= DEVCFGFREMIN)
					{
						FormatSend(CMDCONFIRM, NULL, NULL);	//发送确认帧
					}
					else
					{
						FormatSend(CMDINVALID, NULL, NULL);	//发送无效帧
					}
				}
				break;
				
				case CMDRDDEVID:
				FormatSend(CMDRETDEVID, (u8*)devid, DEVIDLEN);		//发送设备ID帧
				break;
				
				case CMDRDDEVINFO:
				FormatSend(CMDRETDEVINFO, (u8*)&devinfo, sizeof(DevInfoStuType));	//发送设备信息帧
				break;
				
				default:
				break;
			}
		}
		else if(0 != cmdsend)
		{
			switch(cmdsend)
			{
				case CMDINFOREPORT:
				//FormatSend(1, (u8*)&testbuf, 2);	//信息汇报帧
				FormatSend(CMDINFOREPORT, (u8*)&devstate, sizeof(DevStateStuType));	//信息汇报帧
				break;
				
				case CMDRDLINKSTE:
				break;
				
				case CMDSETWORKMODE:
				break;
				
				case CMDRDMODINFO:
				break;
				
				default:
				break;
			}
			cmdsend = 0;
		}
	}
}

/******************************************
Fun: 接收设备数据命令解析
Input: cmd:命令
Output:void
Return:void
******************************************/
void DevRevCmdHandle()
{
	if(0 != framerevpara.offset)
	{
		u8 templen = 0;
		
		if(0 == dataup.state)
		{
			FifoCopy(&templen, &framerevfifo, 3, 1);
			templen += 4;
			FifoOutput((u8*)&dataup, &framerevfifo, templen);
            
			framerevpara.offset -= templen;
			dataup.state = 2;
		}
	}

	if(2 == dataup.state)
	{
		switch(dataup.framedata.data[0])
		{
			case CMDCONTROL:
			DevCtrlHandle();
			dataup.state = 3;
			break;
			
			case CMDCONFIRM:
			if(1 == alarmpara.state)
			{
				alarmpara.state = 2;
			}
			dataup.state = 0;
			datadown.state = 0;
			APIStopTimer(&framesendtimer);
			FrameSendparaInit();
			break;
			
			case CMDINFOREPORTCFG:
			{
				u16 temp = 0;
				dataup.state = 3;
				temp = dataup.framedata.data[1]<<8 | dataup.framedata.data[2];
				if(temp >= DEVCFGFREMIN)
				{
					inforeportpara.reportcnt = temp; //设置汇报频率
				}
				else
				{
					if(0 != inforeportpara.reportcnt)
					{
						inforeportpara.reportcnt = DEVCFGFREMIN;//设置汇报频率
					}
					else
					{
						inforeportpara.reportcnt = 0;
					}
				}
			}
			break;
			
			case CMDCHECKSTE:
			dataup.state = 3;
			break;
			
			case CMDSTOPALARM:
			dataup.state = 0;
			alarmpara.state = 3;
			break;
			
			case CMDRDDEVID:
			dataup.state = 3;
			break;
			
			case CMDRDDEVINFO:
			dataup.state = 3;
			break;
			
			//			case CMDRETENCRYPARA:
			//				dataup.state = 0;
			//				datadown.state = 0;
			//				APIStopTimer(&framesendtimer);
			//				FrameSendparaInit();
			//				break;
			
			//		case CMDBPSTEST:
			//			break;
			
			//			case CMDRETLINKSTE:
			//				//显示链路状态
			//				dataup.state = 0;
			//				datadown.state = 0;
			//				APIStopTimer(&framesendtimer);
			//				FrameSendparaInit();
			//				break;
			
			//		case CMDDEVRST:
			//			break;
			
			//		case CMDRDDEVPROFILE:
			//			break;
			
			case CMDRSSI:
			dataup.state = 0;
			signal = dataup.framedata.data[1];//显示信号等级
			break;
			
			//			case CMDRETWORKMODE:
			//				dataup.state = 0;
			//				datadown.state = 0;
			//				APIStopTimer(&framesendtimer);
			//				FrameSendparaInit();
			//				break;
			
			//		case CMDWRDEVPROFILE:
			//			break;
			
			//			case CMDRETMODINFO:
			//				//显示模块信息
			//				dataup.state = 0;
			//				datadown.state = 0;
			//				APIStopTimer(&framesendtimer);
			//				FrameSendparaInit();
			//				break;
			
			//		case CMDUPPACK:
			//			break;
			//			
			//		case CMDUPAPPCHK:
			//			break;
			
			default:
			dataup.state = 0;
			printf("不识别的命令\r\n");
			break;
		}
	}
}

/******************************************
Fun: 发送一个字节到设备
Input: data:发送的数据
Output:void
Return:void
******************************************/
void SendByteToDev(u8 data)
//__weak void SendByteToDev(u8 data)
{
#if DEBUGFRAMEREV
	printf("0x");
	printf("%02x", data);
#endif
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
Return:void
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
//		PrintfMem(tempbuf,i);
		return 1;
	}
	return 0;

}

/******************************************
Fun: 帧发送定时时间到处理
Input: void
Output:void
Return:void
******************************************/
static void DevSendTimerHandle(void)
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
#if DEBUGFRAMEREV
			printf("没收到回复指令\r\n");
#endif			
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
static void DatadownHandle(void)
{
	if(2 == datadown.state)
	{
		u8 tempcmd = 0;
		tempcmd = datadown.framedata.data[0];
		
		framesendpara.len = datadown.framedata.datalen + 2; //控制域和长度2字节
		
		if(0 != datadown.framectrl.bit.encrypt)
		{
			Encoding((u8*)&datadown.framedata, (datadown.framedata.datalen+1));		//数据加密
		}
		
		DevSendData();	//发送数据
		
		switch(tempcmd)
		{
			case CMDINFOREPORT:
			case CMDRDLINKSTE:
			case CMDSETWORKMODE:
			case CMDRDMODINFO:				
			datadown.state = 3;
			APIInitTimer(&framesendtimer, FRAMESENDCNT, NULL, NULL);
//			APIStartTimer(&framesendtimer);
			break;
			
			default:
			datadown.state = 0;
			break;
		}
	}
}

void AlarmHandle()
{
#ifdef KONGTIAO
	if(devstate.temparature >= TEMPALARMVALUE)
	{
		if(0 == alarmpara.state)
		{
			alarmpara.state = 1;
			devstate.tempalarm = 1;
			FormatSend(CMDALARM, (u8*)&devstate, sizeof(DevStateStuType));	//发送状态帧
			
			APIInitTimer(&alarmpara.alarmtimer, ALARMCNTNOMAL, NULL, NULL);
			APIStartTimer(&alarmpara.alarmtimer);
			
			lcdpara.disnum = 3;
		}
		
		if(2 == alarmpara.state)
		{
			alarmpara.state = 4;
			APIInitTimer(&alarmpara.alarmtimer, ALARMCNTHANDLED, NULL, NULL);
		}
		
		if(3 == alarmpara.state)
		{
			alarmpara.state = 4;
			APIStopTimer(&alarmpara.alarmtimer);
		}
	}
	else
	{
		if(0 != alarmpara.state)
		{
			alarmpara.state = 0;
			APIStopTimer(&alarmpara.alarmtimer);
			devstate.tempalarm = 0;
			lcdpara.disnum = 3;
		}
	}
#endif
}

void AlarmTimerHandle()
{
	if(1 == alarmpara.alarmtimer.state)
	{
		alarmpara.alarmtimer.state = 0;
		FormatSend(CMDALARM, (u8*)&devstate, sizeof(DevStateStuType));	//发送状态帧
	}
}

void KeyTimerHandle()
{
	if(1 == keypara.keytimer.state)
	{
		keypara.keytimer.state = 0;
		if(1 == keypara.KeyParaShake)
		{
			keypara.KeyParaShake = 0;
			APIStopTimer(&keypara.keytimer);
		}
		else
		{
			keyvalue.KeyValueNum = KeyNumNOkey;
			APIStopTimer(&keypara.keytimer);
			
			//			if(35 > devstate.temparature)
			//			{
			//				devstate.temparature++;
			//			}
			//			else
			//			{
			//				devstate.temparature = 25;
			//			}
			//			lcdpara.disnum = 0;
		}
	}
}

void KeyScan()
{
	if(0 == keypara.KeyParaShake)			//不消抖
	{
		u8 keynum = 0;
		keynum = (u8)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
		switch(keypara.KeyParaState)
		{
			case KeyStateUp:
			if(0 != keynum)
			{
				keypara.KeyParaState = KeyStateDowning;
				keypara.KeyParaShake = TRUE;
				APIInitTimer(&keypara.keytimer, KEYSHAKECNT, NULL, NULL);
				APIStartTimer(&keypara.keytimer);
				//					FifoInput(&Uart1SendFifo, "开始消抖\r\n", sizeof"开始消抖\r\n");		//test
				return;
			}
			break;
			
			case KeyStateDowning:
			if(KeyNumNOkey != keynum)
			{
				keypara.KeyParaState = KeyStateDown;
				keyvalue.KeyValueDown = TRUE;				//按下有效
			}
			else
			{
				keypara.KeyParaState = KeyStateUp;
			}
			break;
			
			case KeyStateDown:
			if (KeyNumNOkey != keynum)
			{
				
			}
			else
			{
				keypara.KeyParaState = KeyStateUping;
				keypara.KeyParaShake = TRUE;
				APIInitTimer(&keypara.keytimer, KEYSHAKECNT, NULL, NULL);
				APIStartTimer(&keypara.keytimer);
				//					FifoInput(&Uart1SendFifo, "开始消抖\r\n", sizeof"开始消抖\r\n"); 	//test
			}
			break;
			
			case KeyStateUping:
			if (KeyNumNOkey != keynum)
			{
				keypara.KeyParaState = KeyStateDown;
			}
			else
			{
				keypara.KeyParaState = KeyStateUp;
				keyvalue.KeyValueUp = TRUE;						//抬起有效
			}
			break;
			
			default:
			break;
		}
	}
	if(TRUE == keyvalue.KeyValueUp)
	{
		keyvalue.KeyValueUp = 0;
		if(0 != keyvalue.KeyValueNum)	//双击
		{
			keyvalue.KeyValueNum = KeyNumNOkey;
			APIStopTimer(&keypara.keytimer);
			
			//			if(16 < devstate.temparature)
			//			{
			//				devstate.temparature--;
			//			}
			//			else
			//			{
			//				devstate.temparature = 25;
			//			}
			//			lcdpara.disnum = 0;
		}
		else				//单击
		{
//			cmdsend = CMDALARM;
			FormatSend(CMDALARM, (u8*)&devstate, sizeof(DevStateStuType));	//发送状态帧
			
			keyvalue.KeyValueNum++;
			APIInitTimer(&keypara.keytimer, KEYDOUBLECLICK, NULL, NULL);
			APIStartTimer(&keypara.keytimer);
		}
	}
}

void InfoReportHandle()
{
	if((0 == inforeportpara.sw) && (0 != inforeportpara.reportcnt))
	{
		inforeportpara.sw = 1;
		inforeportpara.timercnt = inforeportpara.reportcnt;
		APIInitTimer(&inforeportpara.timer, INFOREPORTCNT, NULL, NULL);
		APIStartTimer(&inforeportpara.timer);
	}
	else if((0 != inforeportpara.sw) && (0 == inforeportpara.reportcnt))
	{
		inforeportpara.sw = 0;
		APIStopTimer(&inforeportpara.timer);
	}
}

void InfoReportTimerHandle()
{
	if(0 != inforeportpara.timer.state)
	{
		inforeportpara.timer.state = 0;
		if(0 == inforeportpara.reportcnt)
		{
			APIStopTimer(&inforeportpara.timer);
		}
		else
		{
			inforeportpara.timercnt--;
			if(0 == inforeportpara.timercnt)
			{
				inforeportpara.timercnt = inforeportpara.reportcnt;
				cmdsend = CMDINFOREPORT;
			}
		}
	}
}

/******************************************
Fun: 帧处理主循环
Input: void
Output:void
Return:void
******************************************/
void DevFrameHandle(void)
{	
	DevRevTimerHandle();
	DevRevCmdHandle();
	
#if DEBUGFRAMEREV
	DevFrameErrHandle();
#endif
	
	SendCmdHandle();
	DatadownHandle();
	DevSendTimerHandle();
	
	InfoReportHandle();
	InfoReportTimerHandle();
	
	AlarmTimerHandle();
	AlarmHandle();
	
	Uart2DataHandle();
	
	KeyTimerHandle();
	KeyScan();
}

void UartInit()
{
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

void DevHardInit()
{
	UartInit();
}

void DevParaInit()
{
	FifoInit(&modsendfifo, modsendfifobuf, MODSENDFIFOLEN);
	FifoInit(&framerevfifo, revfifobuf, REVFIFOLEN);

	datadown.framehead[0] = 0xf5;		//帧头
	datadown.framehead[1] = 0xf5;		//帧头
	datadown.framectrl.byte = 0;		//帧控制字
	
#ifdef KONGTIAO	
	devstate.power = 0;
	devstate.temparature = 25;
	devstate.mode = 1;
	devstate.wind = 1;
//	devstate.date[0] = 2016>>8;
//    devstate.date[1] = 2016&0xff;
//	devstate.date[2] = 1;
//	devstate.date[3] = 26;
//	devstate.time[0] = 9;
//	devstate.time[1] = 5;
//	devstate.time[2] = 35;
#else
	devstate.power = 0;
	devstate.mode = 1;
#endif

	dataup.framehead[0] = 0xf5;
	dataup.framehead[1] = 0xf5;
//	inforeportpara.reportcnt = DEVCFGFREMIN;
	
	AppRtcInit();
}
