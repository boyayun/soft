#ifndef __COMMON_H
#define __COMMON_H

#define OS		//����ϵͳ

#ifdef OS
#include "MICO.h"
#else
#define FALSE 0
#define TRUE  1
#endif

//#include "SppProtocol.h"
//#include "mico_tcl.h"
//#include "mico_tcl_ota.h"
//#include "geniot.h"

//#include "stm32f1xx_hal.h"
#include "string.h"

typedef signed          char s8;
typedef signed short     int s16;
typedef signed           int s32;

typedef unsigned        char u8;
typedef unsigned short   int u16;
typedef unsigned         int u32;

#define __INLINE  inline

#define DEVEICE

#define OK 0
//#define ERROR  1
#define TMIEOUT  2

#define INIT	0	//��ʼ״̬
#define DOING	1	//����ִ��
#define DONE	2	//ִ�����

#define CRCLIST		//CRCУ���
//#define CRCLIST16	//CRC16λУ���

//#define DEBUGFIFO

//#ifndef __weak
//  #define __weak   __attribute__((weak))
//#endif /* __weak */

//#ifndef __packed
//  #define __packed __attribute__((__packed__))
//#endif /* __packed */

/*�������͵Ĵ�С��ת��*/
#define T_U32(x) 			((((x)&0xff)<<24)|(((x)&0xff00)<<8)|(((x)&0xff0000)>>8)|(((x)&0xff000000)>>24))
#define T_U16(x) 			((((x)&0xff)<<8)|(((x)&0xff00)>>8))
				
/*ָ��ָ���������͵Ĵ�С��ת��*/
#define T_U32_PTR(x) 		(((u8)x[0]<<24)|((u8)x[1]<<16)|((u8)x[2]<<8)|(u8)x[3])
#define T_U16_PTR(x) 		(((u8)x[0]<<8)|(u8)x[1])
	
//�������ͱ�������
typedef union
{
	u16 val;
	u8 	u8adr[2];
}union_u16;

typedef union
{
    u32 val;
	u16 u16adr[2];
    u8 	u8adr[4];
}union_u32;

typedef struct
{
	u8 curnum;
	u8 totnum;
}NumStu;

typedef struct
{
	u32 ID : 29;		//0~~28
	u32 AcqWay : 1;		//29
	u32 IDType : 1;		//30
	u32 channel : 1;	//31
}BitStu;

typedef struct
{
#ifdef OS
	mico_timer_t timer;
#else
	u8 sw;			//��ʱ������ 0:�� 1:ֻ��ʱһ�Σ�ʱ�䵽ʱ��ص���ʱ��
	u16 timevalue;	//��ʱʱ��	��λ:ms
	u16 cnt;		//����ֵ	��λ:ms
#endif
	u8 state;  		//��ʱ״̬ 0:��ʼ״̬ 1:��ʱʱ�䵽ʱֵΪ1
}TimerParaStu;

typedef struct
{
	u8  *FifoAddr;	//FIFO����
	u16 FifoLen;	//FIFO����
	volatile u16 Datalen;	//FIFO���ݳ���
	volatile u16 FifoOffset;	//FIFO��ַƫ��
//	u8  State;		//״̬  0:����; 1:д������̫��; 2:FIFO��������̫��;
}FifoTyp;

extern void HAL_SYSTICK_Callback();
extern u8 secflag;
extern u16 FifoCopy(u8 *des, FifoTyp *fifo, u16 offset, u16 len);
extern u16 FifoDeletHead(FifoTyp *fifo, u16 len);
extern u16 FifoDeletTail(FifoTyp *fifo, u16 len);
extern u8 FifoPushOneByte(FifoTyp *fifo, u8 data);

extern void APIInitTimer(TimerParaStu* timertimer, u32 time_ms, void* function, void* arg);
extern void APIReloadTimer(TimerParaStu* timer, u32 time_ms);
extern void APIStartTimer(TimerParaStu* timer);
extern void APIStopTimer(TimerParaStu* timer);

extern void MainHandle(void);
extern void Encoding(u8* data, u8 len);
extern u8 Decoding(u8* data, u8 len);
extern u16 crc_cal(u8* data, u8 len);
extern u8 SumCheck(u8 *data, u8 len);
extern u8 XorCheck(u8 *data, u8 len);
extern u32 TimingDelay;
extern void PrintfMem(u8 *buf, int len);
extern void BitReplace(u32* src, u8 bit, u8* data, u8 state, u8 type);
extern u32 HEXFormat2BCDFormat32(u32 hex_data);
extern u32 BCDFormat2HEXFormat32(u32 bcd_data);
extern u16 CalAbs16(s16 val);
extern u32 CalAbs32(s32 val);
extern u32 CalAbs(s32 val);
extern u32 CalSubbAbs(u32 a, u32 b);
extern u8 idigitTst(u8 val);
extern u16 Max16(u16 a, u16 b);
extern u16 Min16(u16 a, u16 b);
extern void AddSubRecycle(u8 type, u8* currentnum, u8 totalnum);
extern void NegMemCpy(u8 *des, u8 *src, u8 len);
extern void DelayMs(u32 nTime);
extern u32  power10 (u8 y);
extern u32  Str2Hex (u8 *str, u8 len);
extern u32  DecStr2Hex (u8 *str, u8 len, u8 fl);
extern void FifoInit(FifoTyp *fifo, u8 *src, u16 fifolen);
extern _Bool FifoInput(FifoTyp *fifo, u8 *src, u16 len);
extern u16 FifoOutput(u8 *des, FifoTyp *fifo, u16 len);

#endif	//common.h
