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

#define REVFIFOLEN	1024			//����FIFO����
u8 revfifobuf[REVFIFOLEN];			//����FIFOBUFER
FifoTyp framerevfifo;				//���ݽ���FIFO

#define MODSENDFIFOLEN	1024		//����FIFO����
u8 modsendfifobuf[MODSENDFIFOLEN];	//���ݷ���FIFO
FifoTyp modsendfifo;				//ģ�����ݷ���FIFO

FrameRevParaStu framerevpara;		//֡������ز���
TimerParaStu framerevtimer; 		//֡���ն�ʱ���ӿ�
DataInterfaceStu dataup;			//�豸�����ϴ��ӿ�

FrameSendParaStu framesendpara;		//֡������ز���
TimerParaStu framesendtimer; 		//֡���Ͷ�ʱ���ӿ�
DataInterfaceStu datadown;			//�������ݵ��豸�ӿ�

/*�豸��ز���*/
DevStateStuType devstate;			//�豸״̬
u8 cmdsend;				//Ҫ���͵�����
u8 signal;				//�źŵȼ�
const u8 devid[DEVIDLEN] = "69245757160260316150723123456780124";
const DevInfoStuType devinfo = {{16, 01, 26}, {16, 01, 26}, 0};
const u8 testbuf[3] = {0x01, 0x00};
InfoReportStuType inforeportpara;		//��Ϣ�㱨����
AlarmStuType alarmpara;				//��������
/*�豸��ز���*/

void Uart2DataHandle(void)
{
	if(1 != Uart2SendState)		//FIFO����ͨ�����ڷ���
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
Fun: ֡���ղ�����ʼ��
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
Fun: ֡���Ͳ�����ʼ��
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
Fun: �����豸�����������
Input: cmd:����
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
	switch(funindex)		//���ܺ�
	{
		case 31:		//����
		devstate.power = dataup.framedata.data[i++];
		lcdpara.disnum = 2;
		break;

#ifndef KONGTIAO			
		case 5760:		//ģʽ
		devstate.mode = dataup.framedata.data[i++];
		lcdpara.disnum = 1;		
#else
        case 5601:
		devstate.mode = dataup.framedata.data[i++];
		lcdpara.disnum = 1;
		break;
			
//		case 38:		//����
//		devstate.date[0] = dataup.framedata.data[i++];
//		devstate.date[1] = dataup.framedata.data[i++];
//		devstate.date[2] = dataup.framedata.data[i++];
//        devstate.date[3] = dataup.framedata.data[i++];
//		lcdpara.disnum = 4;
//		SetRTCDate();
//		break;
//		
//		case 39:		//ʱ��
//		devstate.time[0] = dataup.framedata.data[i++];
//		devstate.time[1] = dataup.framedata.data[i++];
//		devstate.time[2] = dataup.framedata.data[i++];
//		lcdpara.disnum = 6;
//		SetRTCTime();
//		break;
		
		case 611:		//�¶�
		devstate.temparature = dataup.framedata.data[i++];
		lcdpara.disnum = 0;
		break;

		case 5602:		//����
		devstate.wind = dataup.framedata.data[i++];
		lcdpara.disnum = 3;
		break;
#endif
		default:
		break;
	}
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
					FormatSend(CMDSTE, (u8*)&devstate, sizeof(DevStateStuType));	//����״̬֡
					break;
				
				case CMDINFOREPORTCFG:
				{
					u16 temp = 0;
					temp = dataup.framedata.data[1]<<8 | dataup.framedata.data[2];
					if(temp >= DEVCFGFREMIN)
					{
						FormatSend(CMDCONFIRM, NULL, NULL);	//����ȷ��֡
					}
					else
					{
						FormatSend(CMDINVALID, NULL, NULL);	//������Ч֡
					}
				}
				break;
				
				case CMDRDDEVID:
				FormatSend(CMDRETDEVID, (u8*)devid, DEVIDLEN);		//�����豸ID֡
				break;
				
				case CMDRDDEVINFO:
				FormatSend(CMDRETDEVINFO, (u8*)&devinfo, sizeof(DevInfoStuType));	//�����豸��Ϣ֡
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
				//FormatSend(1, (u8*)&testbuf, 2);	//��Ϣ�㱨֡
				FormatSend(CMDINFOREPORT, (u8*)&devstate, sizeof(DevStateStuType));	//��Ϣ�㱨֡
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
Fun: �����豸�����������
Input: cmd:����
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
					inforeportpara.reportcnt = temp; //���û㱨Ƶ��
				}
				else
				{
					if(0 != inforeportpara.reportcnt)
					{
						inforeportpara.reportcnt = DEVCFGFREMIN;//���û㱨Ƶ��
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
			//				//��ʾ��·״̬
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
			signal = dataup.framedata.data[1];//��ʾ�źŵȼ�
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
			//				//��ʾģ����Ϣ
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
			printf("��ʶ�������\r\n");
			break;
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
Return:void
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
//		PrintfMem(tempbuf,i);
		return 1;
	}
	return 0;

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
		}
		else
		{
#if DEBUGFRAMEREV
			printf("û�յ��ظ�ָ��\r\n");
#endif			
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
			FormatSend(CMDALARM, (u8*)&devstate, sizeof(DevStateStuType));	//����״̬֡
			
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
		FormatSend(CMDALARM, (u8*)&devstate, sizeof(DevStateStuType));	//����״̬֡
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
	if(0 == keypara.KeyParaShake)			//������
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
				//					FifoInput(&Uart1SendFifo, "��ʼ����\r\n", sizeof"��ʼ����\r\n");		//test
				return;
			}
			break;
			
			case KeyStateDowning:
			if(KeyNumNOkey != keynum)
			{
				keypara.KeyParaState = KeyStateDown;
				keyvalue.KeyValueDown = TRUE;				//������Ч
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
				//					FifoInput(&Uart1SendFifo, "��ʼ����\r\n", sizeof"��ʼ����\r\n"); 	//test
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
				keyvalue.KeyValueUp = TRUE;						//̧����Ч
			}
			break;
			
			default:
			break;
		}
	}
	if(TRUE == keyvalue.KeyValueUp)
	{
		keyvalue.KeyValueUp = 0;
		if(0 != keyvalue.KeyValueNum)	//˫��
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
		else				//����
		{
//			cmdsend = CMDALARM;
			FormatSend(CMDALARM, (u8*)&devstate, sizeof(DevStateStuType));	//����״̬֡
			
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
Fun: ֡������ѭ��
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

	datadown.framehead[0] = 0xf5;		//֡ͷ
	datadown.framehead[1] = 0xf5;		//֡ͷ
	datadown.framectrl.byte = 0;		//֡������
	
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
