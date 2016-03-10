#include "devproanaly.h"
#include "stdio.h"
#include "usart.h"
#include "common.h"



FrameRevParaStu framerevpara;		//֡������ز���
TimerParaStu framerevtimer; 		//֡���ն�ʱ���ӿ�
DataInterfaceStu dataup;			//�豸�����ϴ��ӿ�

FrameSendParaStu framesendpara;		//֡������ز���
TimerParaStu framesendtimer; 		//֡���Ͷ�ʱ���ӿ�
DataInterfaceStu datadown;			//�������ݵ��豸�ӿ�

/******************************************
Fun: ֡���ղ�����ʼ��
Input: void
Output:void
Return:void
******************************************/
static void FrameRevInit(void)
//static __INLINE void FrameRevInit(void)
{
	memset(&framerevpara, 0, (sizeof(FrameRevParaStu)-DEBUGFRAMEREV));	//errortype���ܱ���ԭ
}

/******************************************
Fun: ֡���Ͳ�����ʼ��
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
Fun: �����ֽ����ݴ���
Input: data:���յ����ֽ�����
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
			if(FRAMEDATALEN == framerevpara.len)	//��������±������������жϣ������ֺ�У��3�ֽ�
			{
				StopTimer(&framerevtimer);
				framerevpara.datastate = 4; 
			}
		}
		else if(framerevpara.len == dataup.framedata.datalen)	//������ֻҪ����һ�¾���
		{
			StopTimer(&framerevtimer);
			framerevpara.datastate = 4;
		}
	}
}

/******************************************
Fun: �����ֽ������ִ���
Input: byte:���յ�������
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
				DevByteDataHandle(byte);	//д������byte
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
					if(0 != framerevpara.datastate)//ǰһ֡���ݲ��������յ���һ��֡ͷ
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
				DevByteDataHandle(0xF5);	//д������0xF5
			}
			
			else	//��������
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
Fun: ֡���մ�����
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
			printf("����У�����");
			break;
			
		case 2:
			printf("���ݽ��ܴ���");
			break;

		case 3:
			printf("����֡");
			break;
			
		default:
			break;		
	}
	framerevpara.errortype = 0;
}
#endif

/******************************************
Fun: ֡���ն�ʱʱ�䵽����
Input: void
Output:void
Return:void
******************************************/
static void DevRevTimerHandle(void)
{
	if(0 != framerevtimer.state)
	{
		framerevtimer.state = 0;

		/*ֻ���Ǽ������������ڽ�������ʱ����һ���յ��Ĳ���0xF5�Ż���н�һ������*/
		if((3 == framerevpara.datastate) && (0 != framerevpara.wordstate) && (0 != dataup.framectrl.bit.encrypt))
		{
			framerevpara.datastate = 4;
		}
		else	//����֡
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
Fun:������ģ�������
Input: cmd:����ģ�������
Output:void
Return:void
******************************************************/
void ModCmdHandle(u8 cmd)
//__weak void ModCmdHandle(u8 cmd)
{
	switch(cmd)
	{
		case CMDSETWORKMODE:
			//�豸ģ�鹤��ģʽ
			break;
			
		case CMDRDMODINFO:
			//��ѯģ����Ϣ
			break;

		default:
			break;
	}
}

/******************************************
Fun: �����豸�����������
Input: cmd:����
Output:void
Return:void
******************************************/
static void DevRevCmdHandle(u8 cmd)
{
	switch(cmd)
	{
		/*�豸���ظ����ص�����*/
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
			
		/*�豸�����������ص�����*/
		case CMDALARM:
		case CMDINFOREPORT:
		case CMDRDENCRYPARA:
		case CMDRDLINKSTE:
			dataup.state = 2;
			break;

		/*�豸���������ģ��������������������յ������ݺ�����*/				
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
Fun: ����֡���ݴ���
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
			if(OK == Decoding((u8*)&dataup.framedata, (framerevpara.len+1)))//���ܴӳ��ȿ�ʼ���Լ�1
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
				tempcrc = dataup.framedata.data[dataup.framedata.datalen-2]<<8 |dataup.framedata.data[dataup.framedata.datalen-1];//ת��ΪС��ģʽ
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
			printf("��������ȷ����");
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
Fun: ����һ���ֽڵ��豸
Input: data:���͵�����
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
Fun: �������ݵ��豸
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
Fun: ֡���Ͷ�ʱʱ�䵽����
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
			DevSendData();	//�ط�
			SetTimer(&framesendtimer, FRAMESENDCNT);
		}
		else
		{
#if DEBUGFRAMEREV
			printf("ͨѶʧ��");
#endif			
			datadown.state = 0;
			StopTimer(&framesendtimer);
			FrameSendparaInit();	//��ʼ�����з��Ͳ���
		}
	}
}

/******************************************
Fun: �������ݴ���
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
		
		framesendpara.len = datadown.framedata.datalen + 2; //������ͳ���2�ֽ�
		
		if(0 != datadown.framectrl.bit.encrypt)
		{
			Encoding((u8*)&datadown.framedata, (datadown.framedata.datalen+1));		//���ݼ���
		}
		
		DevSendData();	//��������

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
Fun: ֡������ѭ��
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