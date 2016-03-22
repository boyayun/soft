#ifndef __DEVPROANALY_H
#define __DEVPROANALY_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "common.h"
#define OLD		//�ϵ�Э��

#define DEBUGFRAMEREV		0		//����ģʽ 0:�������� 1:DEBUG
//#define TESTTIMER		//��ʱ������
		
/*����������*/
/*ͨѸģ��<->�豸*/
#define CMDCONFIRM			0x05	//ȷ��֡
/*ͨѸģ��<->�豸*/

/*ͨѸģ��->��������/������*/
#define CMDCOMMERROR		0x1C	//ģ�����豸ͨ���쳣
/*ͨѸģ��->��������/������*/

/*ͨѸģ��->�豸*/
#define CMDCONTROL			0x01	//����֡
#define CMDINFOREPORTCFG	0x06	//��Ϣ�㱨����֡
//#define CMDCHECKSTE			0x08	//��ѯ�豸״̬֡
#define CMDSTOPALARM		0x09	//ֹͣ����֡
#define CMDRDDEVID			0x10	//��ȡ�豸ID
#define CMDRDDEVINFO		0x12	//��ȡ�豸��Ϣ
#define CMDRETENCRYPARA		0x15	//���ؼ��ܲ���
#define CMDBPSTEST			0x16	//�����ʲ���
#define CMDRETLINKSTE		0x18	//��·״̬����
#define CMDDEVRST			0x19	//ǿ���豸��λ
#define CMDRDDEVPROFILE		0x1A	//��ȡ�豸�����ļ�
#define CMDRSSI				0x30	//�����ź�ǿ�Ȼ㱨
#define CMDRETWORKMODE		0x32	//���ع���ģʽ
#define CMDWRDEVPROFILE 	0x80	//�����豸�����ļ�
#define CMDRETMODINFO		0x83	//����ͨѶģ����Ϣ
#define CMDUPPACK			0xA0	//�豸����������
#define CMDUPAPPCHK			0xA2	//�����豸��������У����
/*ͨѸģ��->�豸*/

/*�豸->ͨѸģ��*/
#define CMDSTE				0x02	//״̬֡
#define CMDINVALID			0x03	//��Ч֡
#define CMDALARM			0x04	//�쳣/״̬֡
#define CMDINFOREPORT		0x07	//��Ϣ�㱨֡
#define CMDRETDEVID			0x11	//�����豸ID
#define CMDRETDEVINFO		0x13	//�����豸��Ϣ
#define CMDRDENCRYPARA		0x14	//��ȡ���ܲ���
#define CMDRDLINKSTE		0x17	//��·״̬��ѯ
#define CMDRETDEVPROFILE	0x1B	//�����豸�����ļ�
#define CMDSETWORKMODE		0x31	//���ù���ģʽ
#define CMDACKDEVPROFILE 	0x81	//�豸�����ļ�����ȷ��
#define CMDRDMODINFO		0x82	//��ѯͨѶģ����Ϣ
#define CMDACKUPPACK		0xA1	//�豸����������Ӧ��
#define CMDACKUPAPPCHK		0xA3	//�豸��������У�������Ӧ��
/*�豸->ͨѸģ��*/
/*����������*/

#define FRAMEDATALEN		131		//һ֡�������131�ֽ�
#define FRAMEHEADER			0xF5	//֡ͷ
#define CRCCHECK			0x00	//CRCУ��
#define SUMCHECK			0x01	//�ۼӺ�У��
#define BCCCHECK			0x02	//BCCУ��

#define FRAMELEN			0x05	//֡�̶�����

#define REVHEAD	 	0		//���ڽ���ͷ
#define REVCTRL  	1		//���ڽ��տ�����
#define REVLEN 		2		//���ڽ������ݳ���
#define REVDATA		3		//���ڽ�������


#pragma pack(1)
/*���ݽӿ�*/
/*֡����*/
typedef struct
{
	u8 versions : 2;	//Э��汾 bit0~1 00:TCL���ܼҾӻ���ͨѶЭ��
	u8 reserved : 3;	//Ԥ�� bit2~4
	u8 crctype  : 2;	//У������ bit5~6 00:CRC; 01:�ۼӺ�; 10:BCC
	u8 encrypt	: 1;	//�Ƿ���� bit7 0:�޼���
}FrameCtrlStu;

typedef union
{
	u8 byte;
	FrameCtrlStu bit;	
}FrameCtrlUni;
/*֡����*/

/*֡����*/
typedef struct
{
	u8 	datalen;				//֡���ݳ���(3~131),�����128�ֽ�
//	u8  cmdtype;				//֡����
	u8  data[FRAMEDATALEN];		//֡��������
// 	union_u16 checksum;			//У���
}FrameDataStu;
/*֡����*/

typedef struct
{
	u8 framehead[2];
	FrameCtrlUni framectrl;	//֡������
	FrameDataStu framedata; //֡������

	//����״̬ 0:�����ݻ������Ѵ����� 1:��ʼд������,�ڴ��ѱ�ռ��
	//2:����д����ɿ��Դ��� 3:�������ڽ��д���(ֻ��Ϊ0ʱ�ſ�д��)
	u8 state;
}DataInterfaceStu;		
/*���ݽӿ�*/

/*֡���ղ���*/
typedef struct
{
	u8 wordstate;	//���ж�״̬ 0:��ʼ״̬; 1:�����ж�;
	u8 datastate;  	//����״̬ 0:�ж�ͷ; 1:���ڽ��տ�����; 2:���ڽ������ݳ���; 3:���ڽ�������; 4:���ݽ������
	u16 offset;		//д�����ݵ�ƫ��
//	u8 len; 		//ʵ�ʽ������ݵĳ���
	u8 errortype;	//���ݴ������� 0:��ʼֵ 1:У����� 2:����ʧ�� 3:�������� 4:У����ȷ
}FrameRevParaStu;
/*֡���ղ���*/

/*֡���Ͳ���*/
/*֡����������*/
typedef struct
{
	u8 confirm  : 1;	//ȷ��֡
	u8 workmode : 1;	//����ģʽ
	u8 modinfo  : 1;	//ģ����Ϣ
	u8 linkste	: 1;	//��·״̬
	u8 versions : 4;	//
}FrameSendCmdStu;

typedef union
{
	u8 byte;
	FrameSendCmdStu bit;	
}FrameSendCmdUni;
/*֡����������*/

#define RESENDCNT	2	//�ط�3��
typedef struct
{
//	u8 len;			//�������ݵĳ���(������+���ݳ���+���ݵĳ���)
	u8 resendcnt; 	//�ط���������ֵ
	FrameSendCmdUni sendcmd;		//Ҫ���͵�������
}FrameSendParaStu;
/*֡���Ͳ���*/
#pragma pack()

#ifndef OS
extern TimerParaStu framerevtimer; 		//֡���ն�ʱ���ӿ�
extern TimerParaStu framesendtimer; 		//֡���Ͷ�ʱ���ӿ�
#endif

extern FifoTyp modsendfifo;				//ģ�����ݷ���FIFO
extern _Bool DataDownWr(u8* data, u8 datalen);
extern void FrameHandle(void);
extern void RevDataWrFifo(u8* data, u8 len);
extern void DevHardInit(void);
extern void DevParaInit(void);

#ifdef __cplusplus
	}
#endif
#endif /* d */
