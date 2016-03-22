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
#ifdef OLD
    #define FRAMESENDCNT	330			//�ط���ʱ ��λ:ms
#else
    #define FRAMESENDCNT	270			//�ط���ʱ ��λ:ms
#endif

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

#ifdef OLD
u8 laststate[4];
#endif

void FrameDataHandle();
void RevCmdHandle();
void SetDataDownState();
void RevTimerHandle();

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

#ifdef OLD
	#define HEADNUMER 4
#else
	#define HEADNUMER 2
#endif
	for(i=0; i<len; i++)
	{
		if(REVHEAD == framerevpara.datastate) 		//���ڽ���ͷ
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
Fun: ֡���մ�����
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
Fun: Э��ת��
Input: new:��Э���ַ old:��Э���ַ
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
	    
		newbuf[i++] = 9;			//���ȹ̶�
		newbuf[i++] = CMDALARM;		//CMD
		newbuf[i++] = 0;			//�������ֽ�
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

	newbuf[i++] = 18;			//���ȹ̶�
	newbuf[i++] = CMDSTE;		//CMD
	
	if(0x01 == old[8+3])		//����
	{
		newbuf[i++] = 1;
	}
	else if(0x02 == old[8+3])	//�ػ�
	{
		newbuf[i++] = 0;
	}

	if(0 == (0x01 & old[8+4]))	//�Զ�
	{
		newbuf[i++] = 0;
	}
	else
	{
		newbuf[i++] = 1;
	}

	if(0 == (0x10 & old[8+5]))	//��ʪ
	{
		newbuf[i++] = 0;
	}
	else
	{
		newbuf[i++] = 1;
	}

	newbuf[i++] = old[8+6];		//����
	newbuf[i++] = old[8+7];		//��ʱ
	newbuf[i++] = old[8+8];		//��������
	newbuf[i++] = old[8+9];		//��������
	
	newbuf[i++] = old[8+11];		//������ϢH
	newbuf[i++] = old[8+10];		//������ϢL
	newbuf[i++] = old[8+13];		//�ҳ�������H
	newbuf[i++] = old[8+12];		//�ҳ�������L

	newbuf[i++] = 0;				//�¶�H
	if(50 <= old[8+16])			//�¶�L
	{
		newbuf[i++] = old[8+16] - 50;		
	}
	else
	{
		newbuf[i++] = 50 - old[8+16];		
	}

	newbuf[i++] = 0;				//ʪ��H
	newbuf[i++] = old[8+17];		//ʪ��L
}

/******************************************
Fun: �����豸�����������
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
	                if(3 == datadown.state)
	                {
	                    datadown.state = 0;					//ֹͣ�ط�
	                    APIStopTimer(&framesendtimer);
	                    FrameSendparaInit();
	                }
					cmdflag = 1;
					break;
					
					/*�豸�����������ص�����*/
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

	tempcrc = tempbuffer[templen-2]<<8 | tempbuffer[templen-1];//ת��ΪС��ģʽ
	if(tempcrc != crc_cal(tempbuffer, (templen-2)))		//��֡ͷ��ʼУ��
	{
		errortype = 1;
	}
	else
	{
		memcpy(laststate, &tempbuffer[8+4], 4);
		OldToNew((u8*)&tempbuf, tempbuffer);
	}
#else
	tempbuf.framehead[i] = tempbuffer[i++];			//֡ͷ
	tempbuf.framehead[i] = tempbuffer[i++];			//֡ͷ
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
Input:firstsend:0:�ط���1:��һ�η���
Output:void
Return:true:���ݷ��ͳɹ� false:ʧ��
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
		tempbuf[8+0] = 1;			//�汾H
		tempbuf[8+1] = 0;			//�汾L
        tempbuf[8+2] = 0;			//���ػ�
		tempbuf[8+6] = 11;			//����״̬

		memcpy(&tempbuf[11], laststate, 4);	//�ϴε�״̬
		
		if(11 == funindex)		//��ѯȫ��״̬
		{
			tempbuf[6] = 0x04;			//cmd
			memset(&tempbuf[10], 0, 9);
		}
		
		else if(31 == funindex)
		{
			if(0 == datadown.framedata.data[5])
			{
				tempbuf[8+2] = 2;	//�ػ�
			}
			else
			{
				tempbuf[8+2] = 1;	//����
			}
		}
		else if(5005 == funindex)		//�Զ�
		{
			tempbuf[8+3] = datadown.framedata.data[5];
		}
		else if(5022 == funindex)		//��ʪ
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
		else if(608 == funindex)		//����
		{
			tempbuf[8+5] = datadown.framedata.data[5];
		}
		else if(5023 == funindex)		//��ʱ
		{
			tempbuf[8+5] = datadown.framedata.data[5];
		}

		u16 tempcrc;
		tempcrc = crc_cal((u8*)&tempbuf, 19);				//��ͷ��ʼУ��
		tempbuf[8+11] = tempcrc >> 8;			//crcH
		tempbuf[8+12] = tempcrc & 0xff;			//crcL
        i = 21;
#else
		u8 j=0;

		i = 0;
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
#endif
	}
	if(WrUartBuf(tempbuf,i))		//�ɹ�д�봮��BUF
	{
		PrintfMem(tempbuf,i);
		//LOG
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
void RevTimerUp(void* arg)
{
	framerevtimer.state = 1;
}

/******************************************
Fun: ���Ͷ�ʱ��ʱ�䵽
Input: void
Output:void
Return:void
******************************************/
void SendTimerUp(void* arg)
{
	framesendtimer.state = 1;
}

/******************************************
Fun: ֡���Ͷ�ʱʱ�䵽����
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
			SendData(0);	//�ط�
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

		tempcmd = datadown.framedata.data[0];
		if(!SendData(1))	//��������
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
//            tempbuf[0] = CMDCONTROL;	//���� 
//            tempbuf[4] = 11;			//��ѯ
//            DataDownWr(tempbuf, 5);		//���Ͳ�ѯָ��
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
//Fun: ֡����ص�����
//Input: datalen:cmd+data�ĳ���;data:cmd��data��buf��ַ
//Output:void
//Return:0:����û���� 1:�����Ѵ���
//******************************************/
//__weak _Bool AppHandleCallback(u8* data, u8 datalen)
//{
//	/*�û��Լ���д*/
//	PrintfMem((u8*)&dataup, (dataup.framedata.datalen+4));
//	return 1;
//}

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

TimerParaStu testtimer;
void TimerTest(void* arg)
{
	datadown.state = 0;
	u8 tempbuf[5] = {0};
	tempbuf[0] = CMDCONTROL;	//���� 
	tempbuf[4] = 11;			//��ѯ
	DataDownWr(tempbuf, 5); 	//���Ͳ�ѯָ��
}
/******************************************
Fun: ������ʼ��
Input: void
Output:void
Return:void
******************************************/
void DevParaInit()
{
	APIInitTimer(&testtimer, 2000, (void*)TimerTest, NULL);			//���Զ�ʱ��
	APIStartTimer(&testtimer);

	datadown.framehead[0] = 0xf5;		//֡ͷ
	datadown.framehead[1] = 0xf5;		//֡ͷ
	datadown.framectrl.byte = 0;		//֡������

	APIInitTimer(&framerevtimer, FRAMEREVCNT, (void*)RevTimerUp, NULL);			//�󶨽��ն�ʱ��
	APIInitTimer(&framesendtimer, FRAMESENDCNT, (void*)SendTimerUp, NULL);		//�󶨷��Ͷ�ʱ��
	
	FifoInit(&framerevfifo, revfifobuf, REVFIFOLEN);								//��ʼ������FIFO
	
#ifndef OS
	FifoInit(&modsendfifo, sendfifobuf, SENDFIFOLEN);							//��ʼ������FIFO
#endif /* OS */
}
