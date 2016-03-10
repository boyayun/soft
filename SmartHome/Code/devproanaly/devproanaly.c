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

#define REVFIFOLEN	1024			//����FIFO����
u8 revfifobuf[REVFIFOLEN];			//����FIFOBUFER
FifoTyp framerevfifo;				//���ݽ���FIFO

#ifndef OS
	#define SENDFIFOLEN	1024			//����FIFO����
	u8 sendfifobuf[SENDFIFOLEN];		//����FIFOBUFFER
	FifoTyp modsendfifo;				//ģ�����ݷ���FIFO
	#define UARTSENDLEN		100			//���ڷ��ͳ���
	u8 uartsendbuf[UARTSENDLEN];		//���ڷ���BUFFER
#endif

FrameRevParaStu framerevpara;		//֡������ز���

#define FRAMEREVCNT		100			//���ն�ʱ ��λ:ms ����̫�󣬾��Բ��ܳ����ط�ʱ��270MS
#define FRAMESENDCNT	270			//�ط���ʱ ��λ:ms

TimerParaStu framesendtimer; 		//֡���Ͷ�ʱ���ӿ�
TimerParaStu framerevtimer; 		//֡���ն�ʱ���ӿ�

DataInterfaceStu dataup;			//�豸�����ϴ��ӿ�
FrameSendParaStu framesendpara;		//֡������ز���

DataInterfaceStu datadown;			//�������ݵ��豸�ӿ�

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
Fun: ���ڷ������жϴ���
Input: huart:���ں�
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
Fun: ֡���ղ�����ʼ��
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
Fun: ֡���Ͳ�����ʼ��
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
Fun: �����ֽ������ִ���
Input: data:���ݵ�ַ;len:����
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
		if(0 == framerevpara.wordstate)			//��һ�����ݲ���0xF5
		{
			if(FRAMEHEADER == data[i])			//0XF5
			{
				framerevpara.wordstate = 1;
			}
			else							
			{
				if(REVHEAD == framerevpara.datastate) 		//���ڽ���ͷ
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
			
			if(REVHEAD == framerevpara.datastate)		//����ͷ
			{
				if(FRAMEHEADER == data[i])				//0xF5
				{
					framerevpara.datastate = REVDATA;
					FifoPushOneByte(&framerevfifo, data[i]);				//����0xF5
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
					
					FifoPushOneByte(&framerevfifo, data[i]);				//����0xF5
					FifoPushOneByte(&framerevfifo, data[i]);
				}
				else if(0x55 == data[i])
				{
					FifoPushOneByte(&framerevfifo, 0xF5);			//�¼�2016-0301
					FifoPushOneByte(&framerevfifo, data[i]);
				}
				else				//��������
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
		printf("����У�����\r\n");
		break;
		
	case 2:
		printf("���ݽ��ܴ���\r\n");
		break;
		
	case 3:
		printf("����֡\r\n");
		break;
		
	case 4:
		printf("У����ȷ\r\n");
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
Fun: �����豸�����������
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
				datadown.state = 0;					//ֹͣ�ط�
				APIStopTimer(&framesendtimer);
				FrameSendparaInit();
				
				cmdflag = 1;
				break;
				
				/*�豸�����������ص�����*/
			case CMDALARM:
			case CMDINFOREPORT:
			case CMDRDENCRYPARA:
				framesendpara.sendcmd.bit.confirm = 1;
				cmdflag = 1;
				break;
				
				/*�豸���������ģ��������������������յ������ݺ�����*/				
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
Fun: ���������
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
Fun: ����֡���ݴ���
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
	
	tempbuf.framehead[0] = tempbuffer[i++];			//֡ͷ
	tempbuf.framehead[1] = tempbuffer[i++];			//֡ͷ
	tempbuf.framectrl.byte = tempbuffer[i++];		//������ 
	tempbuf.framedata.datalen = tempbuffer[i++];	//������

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
		if(OK != Decoding((u8*)&tempbuf.framedata, (templen-3)))//���ܳ��ȼ�ȥͷ�Ϳ�����
		{
			errortype = 2;
			goto endhandle;
		}
	}
	
	switch(tempbuf.framectrl.bit.crctype)
	{
	case CRCCHECK:
		tempcrc = tempbuf.framedata.data[j]<<8 |tempbuf.framedata.data[j+1];//ת��ΪС��ģʽ
		if(tempcrc != crc_cal(tempbuffer, i))		//��֡ͷ��ʼУ��
		{
			errortype = 1;
		}
		break;
		
	case SUMCHECK:
		if(tempbuf.framedata.data[j] != SumCheck(&tempbuffer[3], (i-3)))		//�ӳ�����ʼ
		{
			errortype = 1;
		}
		break;
		
	case BCCCHECK:
		if(tempbuf.framedata.data[j] != XorCheck(&tempbuffer[3], (i-3)))//�ӳ�����ʼ
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
Fun: д�뷢�����ݵ�����
Input:buf:д���buf;bufloc:д���λ��;data:ת������� 
Output:void
Return:true: д�봮��buf�ɹ���false:ʧ��
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
Fun: ת��һ���ֽڵ�BUF
Input:buf:д���buf;bufloc:д���λ��;data:ת������� 
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
Fun: �������ݵ��豸
Input:void 
Output:void
Return:true:���ݷ��ͳɹ� false:ʧ��
******************************************/
_Bool DevSendData(void)
{
	u8 i=0;
	u8 j=0;
	u8 tempbuf[250]={0};
	
	tempbuf[i++] = 0xF5;
	tempbuf[i++] = 0xF5;
	tempbuf[i++] = datadown.framectrl.byte;	//������
	tempbuf[i++] = datadown.framedata.datalen;		//����
	
	for(j=0; j<(datadown.framedata.datalen-2); j++)			//��ȥУ�������ֽ�
	{
		TransData(tempbuf, &i, datadown.framedata.data[j]);
	}
	
	if(CRCCHECK == datadown.framectrl.bit.crctype)
	{
		union_u16 temp;
		
		temp.val= crc_cal(tempbuf, i);				//��ͷ��ʼУ��
		TransData(tempbuf, &i, temp.u8adr[1]);
		TransData(tempbuf, &i, temp.u8adr[0]);
	}
	else if(SUMCHECK == datadown.framectrl.bit.crctype)
	{
		u8 temp = 0;
		
		temp = SumCheck(&tempbuf[3], (i-3));	//�ӳ��ȿ�ʼУ��
		TransData(tempbuf, &i, temp);
		TransData(tempbuf, &i, 0);
	}
	else if(BCCCHECK == datadown.framectrl.bit.crctype)
	{
		u8 temp = 0;
		
		temp= XorCheck(&tempbuf[3], (i-3));	//�ӳ��ȿ�ʼУ��
		TransData(tempbuf, &i, temp);
		TransData(tempbuf, &i, 0);
	}
	
	if(WrUartBuf(tempbuf,i))		//�ɹ�д�봮��BUF
	{
		PrintfMem(tempbuf,i);
		return 1;
	}
	return 0;
}

/******************************************
Fun: ���ն�ʱ��ʱ�䵽
Input: void
Output:void
Return:void
******************************************/
void DevRevTimerUp(void* arg)
{
	framerevtimer.state = 1;
}

/******************************************
Fun: ���Ͷ�ʱ��ʱ�䵽
Input: void
Output:void
Return:void
******************************************/
void DevSendTimerUp(void* arg)
{
	framesendtimer.state = 1;
}

/******************************************
Fun: ֡���Ͷ�ʱʱ�䵽����
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
			DevSendData();	//�ط�
		}
		else
		{
			datadown.state = 0;
			
			APIStopTimer(&framesendtimer);
			
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
		if(!DevSendData())	//��������
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
Fun: ����һ���ʹ���
Input: void
Output:void
Return:void
******************************************/
#ifndef OS
void Uart1DataHandle(void)
{
//	if(1 != Uart1SendState)		//FIFO����ͨ�����ڷ���
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
Fun: ֡����ص�����
Input: datalen:cmd+data�ĳ���;data:cmd��data��buf��ַ
Output:void
Return:0:����û���� 1:�����Ѵ���
******************************************/
__weak _Bool AppHandleCallback(u8* data, u8 datalen)
{
	/*�û��Լ���д*/
	PrintfMem((u8*)&dataup, (dataup.framedata.datalen+4));
	return 1;
}

/******************************************
Fun: �ж�����״̬
Input: void
Output:void
Return:0:����û׼���� 1:������׼����
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
Fun: �ж�����״̬
Input: void
Output:void
Return:void
******************************************/
void InitDataUpState()
{
	dataup.state = 0;
}

/******************************************
Fun: д����������
Input: data:cmd��data��buf��ַ, datalen:cmd+data�ĳ���;
Output:void
Return:0:д��ʧ�� 1:������д��
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
Fun: ֡������ѭ��
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
Fun: ���ڳ�ʼ��
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
Fun: Ӳ����ʼ��
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
Fun: ������ʼ��
Input: void
Output:void
Return:void
******************************************/
void DevParaInit()
{
#ifdef TESTTIMER
	APIInitTimer(&testtimer, 10000, (void*)TimerTest, NULL);			//���Զ�ʱ��
	APIStartTimer(&testtimer);
#endif

#ifdef OLD
#else
	datadown.framehead[0] = 0xf5;		//֡ͷ
	datadown.framehead[1] = 0xf5;		//֡ͷ
	datadown.framectrl.byte = 0;		//֡������
#endif

	APIInitTimer(&framerevtimer, FRAMEREVCNT, (void*)DevRevTimerUp, NULL);			//�󶨽��ն�ʱ��
	APIInitTimer(&framesendtimer, FRAMESENDCNT, (void*)DevSendTimerUp, NULL);		//�󶨷��Ͷ�ʱ��
	
	FifoInit(&framerevfifo, revfifobuf, REVFIFOLEN);								//��ʼ������FIFO
	
#ifndef OS
	FifoInit(&modsendfifo, sendfifobuf, SENDFIFOLEN);							//��ʼ������FIFO
#endif /* OS */
}
