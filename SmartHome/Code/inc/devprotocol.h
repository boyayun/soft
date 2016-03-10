#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

#define DEBUGFRAMEREV		1		//����ģʽ 0:�������� 1:DEBUG

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
#define CMDCHECKSTE			0x08	//��ѯ�豸״̬֡
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

#pragma pack(1)
/*֡����*/
typedef struct
{
	u8 	datalen;				//֡���ݳ���(3~131),�����128�ֽ�
//	u8  cmdtype;				//֡����
	u8  data[FRAMEDATALEN];		//֡��������
// 	union_u16 checksum;			//У���
}FrameDataStu;
/*֡����*/

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

/*���ݽӿڽṹ��*/
typedef struct
{
	u8 framehead[2];
	FrameCtrlUni framectrl;	//֡������
	FrameDataStu framedata; //֡������
	//����״̬ 0:�����ݻ������Ѵ����� 1:��ʼд������,�ڴ��ѱ�ռ��
	//2:����д����ɿ��Դ��� 3:�������ڽ��д���(ֻ��Ϊ0ʱ�ſ�д��)
	u8 state;
}DataInterfaceStu;		
/*���ݽӿڽṹ��*/

/*֡���ղ���*/
typedef struct
{
	u8 len; 		//ʵ�ʽ������ݵĳ���
	u8 wrstate;		//����д��״̬ 0:û��ʼ 1:����д��
	u8 wordstate;	//���ж�״̬ 0:��ʼ״̬; 1:�����ж�;
	u8 datastate;  	//����״̬ 0:�ж�ͷ; 1:���ڽ��տ�����; 2:���ڽ������ݳ���; 3:���ڽ�������; 4:���ݽ������
#if DEBUGFRAMEREV
	u8 errortype;	//���ݴ������� 0:��ʼֵ 1:У����� 2:����ʧ�� 3:�������� 4:֡������ 
#endif
}FrameRevParaStu;
/*֡���ղ���*/

/*֡���Ͳ���*/
#define RESENDCNT	3	//�ط�3��
typedef struct
{
	u8 len;			//�������ݵĳ���(������+���ݳ���+���ݵĳ���)
	u8 resendcnt; 	//�ط���������ֵ
}FrameSendParaStu;
/*֡���Ͳ���*/

/*֡��ʱ����*/
#define FRAMEREVCNT		100		//���ն�ʱ ��λ:ms ����̫�󣬾��Բ��ܳ����ط�ʱ��270MS
#define FRAMESENDCNT	270		//�ط���ʱ ��λ:ms
typedef struct
{	
	u16 cnt;			//��ʱʱ��	��λ:ms
	u8 sw;			//��ʱ������ 0:�� 1:ֻ��ʱһ�Σ�ʱ�䵽ʱ��ص���ʱ��
	u8 state;  		//��ʱ״̬ 0:��ʼ״̬ 1:��ʱʱ�䵽ʱֵΪ1
}FrameTimerParaStu;
/*֡��ʱ����*/
#pragma pack()

extern DataInterfaceStu dataup;				//�豸�����ϴ��ӿ�
extern DataInterfaceStu datadown;			//�������ݽӿ�

extern void DevFrameHandle(void);			//֡������ѭ��
extern void DevByteWordHandle(u8 byte);	//�豸�����ֽڴ����ж��н���

#if 0
typedef struct
{
	u8 wordstate : 2;	//��״̬ bit 0~1 00:��ʼ״̬; 01:�����ж�;
	u8 recstate  : 2;  	//����״̬ bit 2~3 00:�ж�ͷ; 01:���ڽ�������; 02:���ݽ������;
//	u8 datastate : 2;  	//����״̬ bit 4~5 00:����δ���ջ��Ѵ��� 01:�����ѽ���δ����	
}FrameStateStu;

typedef union
{
	FrameStateStu framestate;
	u8 frameste;
}FrameStateUnion;
#endif

#ifdef __cplusplus
}
#endif

#endif	//__PROTOCOL_H