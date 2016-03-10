#ifndef __DEVEICE_H
#define __DEVEICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
    
//#define KONGTIAO
#define DEBUGFRAMEREV		0		//����ģʽ 0:�������� 1:DEBUG

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
#define CMDALARM			0x04	//�쳣/����֡
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

#define DEVIDLEN  36				//�豸ID�ĳ���

#define TEMPALARMVALUE	26			//���±���ֵ
#define ALARMCNTNOMAL 	200			//����200ms����
#define ALARMCNTHANDLED 5000		//����5000ms����

#define KEYSHAKECNT	20		//����ʱ�䣬��λ����
#define KEYDOUBLECLICK	400		//˫��ʱ�䣬��λ����

#define REVHEAD	 	0		//���ڽ���ͷ
#define REVCTRL  	1		//���ڽ��տ�����
#define REVLEN 		2		//���ڽ������ݳ���
#define REVDATA		3		//���ڽ�������

typedef enum	
{
KeyStateUp = 0,			//̧��
KeyStateDowning,		//���ڰ���
KeyStateDown,			//����̧��
KeyStateUping,			//����
}EnumKeyState;

typedef enum	
{
KeyNumNOkey = 0,		//û�м� ����
KeyNumMeum,				//�˵���
KeyNumUp,				//�ϼ�
KeyNumDown,				//�¼�
KeyNumOK,				//ȷ����
KeyNumNull				//��ʶ��
}EnumKeyNum;

typedef struct	
{
    EnumKeyState KeyParaState;	//����״̬
    u8 KeyParaShake;			//����״̬
    TimerParaStu keytimer;
}StuKeyPara;

typedef struct	
{
    EnumKeyNum KeyValueNum;		//�������
    u8 KeyValueDown;			//������Ч
    u8 KeyValueUp;				//̧����Ч
    u8 KeyValueLong;			//������Ч
}StuKeyValue;

extern StuKeyPara	keypara;
extern StuKeyValue keyvalue;

extern void KeyScan();

typedef struct
{
    TimerParaStu alarmtimer;		//���ʱ66535ms
    u8 state;						//����״̬ 0:�ޱ��� 1:�������� 2:��ȷ�� 3:ֹͣ���� 4:������
}AlarmStuType;

typedef struct
{
    u8 softversions[4];
    u8 hardversions[4];
    u8 encrypt;
}DevInfoStuType;

#define DEVCFGFREMIN   100		//��С��Ϣ�㱨�������λ:100ms
#define INFOREPORTCNT  100		//100ms
typedef struct
{
    u8 sw;
    u8 state;			//�㱨ʱ�䵽
    u16 timercnt;		//��ʱ����ʱʱ��
    u16 reportcnt;		//�㱨ʱ��
    TimerParaStu timer;
}InfoReportStuType;

typedef struct
{
#ifdef KONGTIAO
    u8 power;		//��Դ����	0:�� 1:��
	u8 temparature;	//�¶�
    u8 mode;		//ģʽѡ��  0:���� 1:���� 2:��ʪ
    u8 wind;		//����
//    u8 date[4];		//�ꡢ�¡���
//    u8 time[3];		//ʱ����
    u8 tempalarm;	//���±���
#else
	u8 power;		//��Դ����	0:�� 1:��
    u8 mode;		//ģʽѡ��  0:���� 1:���� 2:��ʪ
#endif
}DevStateStuType;
/*ʶ����Ϣ�ṹ��*/
//typedef struct
//{
//	u8 DD_Version;				//�����ļ���׼�İ汾
//	u8 DD_Encode;				//�����ļ�����
//	u8 DD_Devictype;			//�����ļ����õ��û�
//	u8 Manufacturer_Code[13];	//���̴���
//	u8 DD_fileVer;				//�����ļ��汾
//	u8 Device_TypeID[2];		//�ҵ�Ĵ����С��
//	u8 Model_ID[3];				//�ҵ��ͺŴ���
//	u8 DD_URL_Lenth[1];			//url���ӵ�ַ�ĳ���
//	u8 DD_URL[20];				//�����ļ�url���ӵ�ַ
//	u8 Update;					//00�����߱���������	01���߱���������
//	u8 Synchronous;				//00�����߱�ͬ��ʱ������	01���߱�ͬ��ʱ������
//}DevInfoStuType;
/*ʶ����Ϣ�ṹ��*/

/*�������ͽṹ*/
//typedef struct
//{
//	u8 Switch;		//������
//}ButtonStuType;
/*�������ͽṹ*/

/*�������ͽṹ*/
//typedef struct
//{
//	u8 Switch;		//������
//}ButtonStuType;
/*�������ͽṹ*/

/*�豸�����ļ�*/
//typedef struct
//{
//	DevInfoStuType devinfo;
//}DevDescFileStuType;
/*�豸�����ļ�*/

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
	u8 wordstate;	//���ж�״̬ 0:��ʼ״̬; 1:�����ж�;
	u8 datastate;	//����״̬ 0:�ж�ͷ; 1:���ڽ��տ�����; 2:���ڽ������ݳ���; 3:���ڽ�������; 4:���ݽ������
	u16 offset; 	//д�����ݵ�ƫ��
//	u8 len; 		//ʵ�ʽ������ݵĳ���
	u8 errortype;	//���ݴ������� 0:��ʼֵ 1:У����� 2:����ʧ�� 3:�������� 4:У����ȷ
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
/*֡��ʱ����*/

extern InfoReportStuType inforeportpara;
extern AlarmStuType alarmpara;
extern TimerParaStu framesendtimer;
extern TimerParaStu framerevtimer;
extern DataInterfaceStu dataup;				//�豸�����ϴ��ӿ�
extern DataInterfaceStu datadown;			//�������ݽӿ�

extern DevStateStuType devstate;			//�豸״̬

extern void DevHardInit();
extern void DevParaInit();
extern void DevFrameHandle(void);			//֡������ѭ��
extern void RevDataWrFifo(u8* data, u8 len);

#ifdef __cplusplus
}
#endif

#endif /* d */
