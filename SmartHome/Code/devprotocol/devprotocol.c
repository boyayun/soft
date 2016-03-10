#include "devproanaly.h"
#include "stdio.h"
#include "usart.h"
#include "common.h"



FrameRevParaStu framerevpara;		//帧接收相关参数
TimerParaStu framerevtimer; 		//帧接收定时器接口
DataInterfaceStu dataup;			//设备数据上传接口

FrameSendParaStu framesendpara;		//帧发送相关参数
TimerParaStu framesendtimer; 		//帧发送定时器接口
DataInterfaceStu datadown;			//下行数据到设备接口

/******************************************
Fun: 帧接收参数初始化
Input: void
Output:void
Return:void
******************************************/
static void FrameRevInit(void)
//static __INLINE void FrameRevInit(void)
{
	memset(&framerevpara, 0, (sizeof(FrameRevParaStu)-DEBUGFRAMEREV));	//errortype不能被还原
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
	memset(&framesendpara, 0, sizeof(FrameSendParaStu));	
}

/******************************************
Fun: 接收字节数据处理
Input: data:接收到的字节数据
Output:NULL
Return:NULL
******************************************/
static void DevByteDataHandle(u8 data)
{
	u8* p = (u8*)&dataup.framedata.data;
	
	if(1 == framerevpara.datastate)
	{
		dataup.framectrl.byte = data;
		framerevpara.datastate = 2;
	}
	else if(2 == framerevpara.datastate)
	{
		dataup.framedata.datalen = data;
		framerevpara.datastate = 3;
	}
	else
	{
		p[framerevpara.len++] = data;
		if(0 != dataup.framectrl.bit.encrypt)	
		{
			if(FRAMEDATALEN == framerevpara.len)	//加密情况下必须收满才能判断，命令字和校验3字节
			{
				StopTimer(&framerevtimer);
				framerevpara.datastate = 4; 
			}
		}
		else if(framerevpara.len == dataup.framedata.datalen)	//不加密只要长度一致就行
		{
			StopTimer(&framerevtimer);
			framerevpara.datastate = 4;
		}
	}
}

/******************************************
Fun: 接收字节数据字处理
Input: byte:接收到的数据
Output:NULL
Return:NULL
******************************************/
void DevByteWordHandle(u8 byte)
{
	if(0 == dataup.state)
	{
		dataup.state = 1;
		framerevpara.wrstate = 1;
		goto wrflag;
	}
	if((1 == dataup.state) && (1 == framerevpara.wrstate) && (4 != framerevpara.datastate))
	{
		goto wrflag;
	}
	return;
	
	wrflag:
	{
		StartTimer(&framerevtimer);
		
		if(0 == framerevpara.wordstate)
		{
			if(FRAMEHEADER == byte)
			{
				framerevpara.wordstate = 1;
			}
			
			else if(0 != framerevpara.datastate) 
			{
				DevByteDataHandle(byte);	//写入数据byte
			}

			else
			{
				dataup.state = 0;
				framerevpara.wrstate = 0;
				StopTimer(&framerevtimer);
			}
		}
		else
		{
			framerevpara.wordstate = 0;
			if(FRAMEHEADER == byte)
			{
				if((3 == framerevpara.datastate) && (0 != dataup.framectrl.bit.encrypt))
				{
					StopTimer(&framerevtimer);
					framerevpara.datastate = 4;
				}
				else
				{
					if(0 != framerevpara.datastate)//前一帧数据不完整，收到下一个帧头
					{
						#if DEBUGFRAMEREV
						framerevpara.errortype = 3;
						#endif
					}
					framerevpara.len = 0;
					framerevpara.datastate = 1;
				}
			}

			else if((0x55 == byte) && (0 != framerevpara.datastate))
			{
				DevByteDataHandle(0xF5);	//写入数据0xF5
			}
			
			else	//错误数据
			{
				dataup.state = 0;
				StopTimer(&framerevtimer);
				FrameRevInit();
#if DEBUGFRAMEREV
				framerevpara.errortype = 3;
#endif
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
			printf("数据校验错误");
			break;
			
		case 2:
			printf("数据解密错误");
			break;

		case 3:
			printf("错误帧");
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

		/*只有是加密数据且正在接收数据时且上一个收到的不是0xF5才会进行进一步处理*/
		if((3 == framerevpara.datastate) && (0 != framerevpara.wordstate) && (0 != dataup.framectrl.bit.encrypt))
		{
			framerevpara.datastate = 4;
		}
		else	//错误帧
		{
#if DEBUGFRAMEREV
			framerevpara.errortype = 3;
#endif
			dataup.state = 0;
			StopTimer(&framerevtimer);
			FrameRevInit();
		}
	}
}

/******************************************************
Fun:处理发给模块的命令
Input: cmd:发给模块的命令
Output:void
Return:void
******************************************************/
void ModCmdHandle(u8 cmd)
//__weak void ModCmdHandle(u8 cmd)
{
	switch(cmd)
	{
		case CMDSETWORKMODE:
			//设备模块工作模式
			break;
			
		case CMDRDMODINFO:
			//查询模块信息
			break;

		default:
			break;
	}
}

/******************************************
Fun: 接收设备数据命令解析
Input: cmd:命令
Output:void
Return:void
******************************************/
static void DevRevCmdHandle(u8 cmd)
{
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
			dataup.state = 2;
			datadown.state = 0;
			StopTimer(&framesendtimer);
			FrameSendparaInit();
			break;
			
		/*设备主动发给网关的命令*/
		case CMDALARM:
		case CMDINFOREPORT:
		case CMDRDENCRYPARA:
		case CMDRDLINKSTE:
			dataup.state = 2;
			break;

		/*设备主动发起给模块的命令，必须立即处理接收到的数据和命令*/				
		case CMDSETWORKMODE:				
		case CMDRDMODINFO:
			ModCmdHandle(cmd);
			dataup.state = 0;
			break;
			
		default:
			break;
	}
}


/******************************************
Fun: 完整帧数据处理
Input: void
Output:void
Return:void
******************************************/
static void DevFrameDataHandle(void)
{
	u16 tempcrc = 0;

	if(4 == framerevpara.datastate)
	{
		u8 errortype = 0;
		
		if(0 != dataup.framectrl.bit.encrypt)
		{
			if(OK == Decoding((u8*)&dataup.framedata, (framerevpara.len+1)))//解密从长度开始所以加1
			{
				if(framerevpara.len != dataup.framedata.datalen)
				{
					errortype = 3;
				}
			}
			else
			{
				errortype = 2;
			}
		}

		switch(dataup.framectrl.bit.crctype)
		{
			case CRCCHECK:
				tempcrc = dataup.framedata.data[dataup.framedata.datalen-2]<<8 |dataup.framedata.data[dataup.framedata.datalen-1];//转换为小端模式
				if(tempcrc != crc_calculate((u8*)&dataup, (dataup.framedata.datalen+2)))
				{
					errortype = 1;
				}
				break;

			case SUMCHECK:
				if(dataup.framedata.data[dataup.framedata.datalen-2] != SumCheck((u8*)&dataup, (dataup.framedata.datalen+2)))
				{
					errortype = 1;
				}
				
			case BCCCHECK:
				if(dataup.framedata.data[dataup.framedata.datalen-2] != XorCheck((u8*)&dataup, (dataup.framedata.datalen+2)))
				{
					errortype = 1;
				}
				break;

			default:
				break;
		}

		FrameRevInit();
		if(0 == errortype)
		{
#if DEBUGFRAMEREV
			printf("命令已正确解析");
//			printf("%02x ", dataup.framedata.data[0]);
			dataup.state = 0;
#endif
			DevRevCmdHandle(dataup.framedata.data[0]);
		}
		else
		{
			dataup.state = 0;
#if DEBUGFRAMEREV		
			framerevpara.errortype = errortype;
#endif
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
Fun: 发送数据到设备
Input:void 
Output:void
Return:void
******************************************/
static void DevSendData(void)
{
	u8 i;
	u8* p = (u8*)&datadown.framectrl;
	
	SendByteToDev(FRAMEHEADER);
	SendByteToDev(FRAMEHEADER);
	
	for(i=0; i<framesendpara.len; i++)
	{
		if(FRAMEHEADER != p[i])
		{
			SendByteToDev(p[i]);
		}
		else
		{
			SendByteToDev(FRAMEHEADER);
			SendByteToDev(0x55);
		}
	}
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
			SetTimer(&framesendtimer, FRAMESENDCNT);
		}
		else
		{
#if DEBUGFRAMEREV
			printf("通讯失败");
#endif			
			datadown.state = 0;
			StopTimer(&framesendtimer);
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
			case CMDCHECKSTE:
			case CMDRDDEVID:
			case CMDRDDEVINFO:
//			case CMDBPSTEST:				
			case CMDDEVRST:
			case CMDRDDEVPROFILE:
			case CMDWRDEVPROFILE:
			case CMDUPPACK:
			case CMDUPAPPCHK:
				datadown.state = 3;
				SetTimer(&framesendtimer, FRAMESENDCNT);
				StartTimer(&framesendtimer);
				break;

			default:
				break;
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
	DevFrameDataHandle();
	
#if DEBUGFRAMEREV
	DevFrameErrHandle();
#endif

	DatadownHandle();
	DevSendTimerHandle();
}

void DevParaInit()
{

}

void DevHardInit()
{

}