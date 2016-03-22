//#include "stm32l1xx_hal.h"
//#include "usart.h"
#include "common.h"
#include "stdio.h"

#ifndef DEVEICE
#include "devproanaly.h"
#else
#include "deveice.h"
#endif

//#include "string.h"
//#include "stdio.h"
//
//void HardFault_Handler(void)
//{
////	u8 temp = 0;
////	
////	__ASM volatile ("BX  LR");
//}
//

#ifdef CRCLIST
#ifdef CRCLIST16
const u16 CRC16_CCITT_TABLE[256] =
{
	0x0, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 
	0x1081, 0x108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5, 
	0x3183, 0x200a, 0x1291, 0x318, 0x77a7, 0x662e, 0x54b5, 0x453c, 0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974, 
	0x4204, 0x538d, 0x6116, 0x709f, 0x420, 0x15a9, 0x2732, 0x36bb, 0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x528, 0x37b3, 0x263a, 0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72, 
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x630, 0x17b9, 0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x738, 0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70, 
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7, 0x840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff, 
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036, 0x18c1, 0x948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 0x2942, 0x38cb, 0xa50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134, 0x39c3, 0x284a, 0x1ad1, 0xb58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c, 
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 0x4a44, 0x5bcd, 0x6956, 0x78df, 0xc60, 0x1de9, 0x2f72, 0x3efb, 
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0xd68, 0x3ff3, 0x2e7a, 
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0xe70, 0x1ff9, 
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0xf78, 
};
#else
const u8 CRC16_CCITT_TABLE8L[256]=
{
	0x0, 0x89, 0x12, 0x9b, 0x24, 0xad, 0x36, 0xbf, 0x48, 0xc1, 0x5a, 0xd3, 0x6c, 0xe5, 0x7e, 0xf7, 	
	0x81, 0x8, 0x93, 0x1a, 0xa5, 0x2c, 0xb7, 0x3e, 0xc9, 0x40, 0xdb, 0x52, 0xed, 0x64, 0xff, 0x76,	
	0x2, 0x8b, 0x10, 0x99, 0x26, 0xaf, 0x34, 0xbd, 0x4a, 0xc3, 0x58, 0xd1, 0x6e, 0xe7, 0x7c, 0xf5, 	
	0x83, 0xa, 0x91, 0x18, 0xa7, 0x2e, 0xb5, 0x3c, 0xcb, 0x42, 0xd9, 0x50, 0xef, 0x66, 0xfd, 0x74, 	
	0x4, 0x8d, 0x16, 0x9f, 0x20, 0xa9, 0x32, 0xbb, 0x4c, 0xc5, 0x5e, 0xd7, 0x68, 0xe1, 0x7a, 0xf3, 	
	0x85, 0xc, 0x97, 0x1e, 0xa1, 0x28, 0xb3, 0x3a, 0xcd, 0x44, 0xdf, 0x56, 0xe9, 0x60, 0xfb, 0x72, 	
	0x6, 0x8f, 0x14, 0x9d, 0x22, 0xab, 0x30, 0xb9, 0x4e, 0xc7, 0x5c, 0xd5, 0x6a, 0xe3, 0x78, 0xf1, 	
	0x87, 0xe, 0x95, 0x1c, 0xa3, 0x2a, 0xb1, 0x38, 0xcf, 0x46, 0xdd, 0x54, 0xeb, 0x62, 0xf9, 0x70,	
	0x8, 0x81, 0x1a, 0x93, 0x2c, 0xa5, 0x3e, 0xb7, 0x40, 0xc9, 0x52, 0xdb, 0x64, 0xed, 0x76, 0xff, 
	0x89, 0x0, 0x9b, 0x12, 0xad, 0x24, 0xbf, 0x36, 0xc1, 0x48, 0xd3, 0x5a, 0xe5, 0x6c, 0xf7, 0x7e, 
	0xa, 0x83, 0x18, 0x91, 0x2e, 0xa7, 0x3c, 0xb5, 0x42, 0xcb, 0x50, 0xd9, 0x66, 0xef, 0x74, 0xfd,
	0x8b, 0x2, 0x99, 0x10, 0xaf, 0x26, 0xbd, 0x34, 0xc3, 0x4a, 0xd1, 0x58, 0xe7, 0x6e, 0xf5, 0x7c, 
	0xc, 0x85, 0x1e, 0x97, 0x28, 0xa1, 0x3a, 0xb3, 0x44, 0xcd, 0x56, 0xdf, 0x60, 0xe9, 0x72, 0xfb, 
	0x8d, 0x4, 0x9f, 0x16, 0xa9, 0x20, 0xbb, 0x32, 0xc5, 0x4c, 0xd7, 0x5e, 0xe1, 0x68, 0xf3, 0x7a, 
	0xe, 0x87, 0x1c, 0x95, 0x2a, 0xa3, 0x38, 0xb1, 0x46, 0xcf, 0x54, 0xdd, 0x62, 0xeb, 0x70, 0xf9, 
	0x8f, 0x6, 0x9d, 0x14, 0xab, 0x22, 0xb9, 0x30, 0xc7, 0x4e, 0xd5, 0x5c, 0xe3, 0x6a, 0xf1, 0x78
};

const u8 CRC16_CCITT_TABLE8H[256]=
{
	0x0, 0x11, 0x23, 0x32, 0x46, 0x57, 0x65, 0x74, 0x8c, 0x9d, 0xaf, 0xbe, 0xca, 0xdb, 0xe9, 0xf8, 
	0x10, 0x1, 0x33, 0x22, 0x56, 0x47, 0x75, 0x64, 0x9c, 0x8d, 0xbf, 0xae, 0xda, 0xcb, 0xf9, 0xe8, 
	0x21, 0x30, 0x2, 0x13, 0x67, 0x76, 0x44, 0x55, 0xad, 0xbc, 0x8e, 0x9f, 0xeb, 0xfa, 0xc8, 0xd9,
	0x31, 0x20, 0x12, 0x3, 0x77, 0x66, 0x54, 0x45, 0xbd, 0xac, 0x9e, 0x8f, 0xfb, 0xea, 0xd8, 0xc9, 
	0x42, 0x53, 0x61, 0x70, 0x4, 0x15, 0x27, 0x36, 0xce, 0xdf, 0xed, 0xfc, 0x88, 0x99, 0xab, 0xba, 
	0x52, 0x43, 0x71, 0x60, 0x14, 0x5, 0x37, 0x26, 0xde, 0xcf, 0xfd, 0xec, 0x98, 0x89, 0xbb, 0xaa, 
	0x63, 0x72, 0x40, 0x51, 0x25, 0x34, 0x6, 0x17, 0xef, 0xfe, 0xcc, 0xdd, 0xa9, 0xb8, 0x8a, 0x9b, 
	0x73, 0x62, 0x50, 0x41, 0x35, 0x24, 0x16, 0x7, 0xff, 0xee, 0xdc, 0xcd, 0xb9, 0xa8, 0x9a, 0x8b,
	0x84, 0x95, 0xa7, 0xb6, 0xc2, 0xd3, 0xe1, 0xf0, 0x8, 0x19, 0x2b, 0x3a, 0x4e, 0x5f, 0x6d, 0x7c, 
	0x94, 0x85, 0xb7, 0xa6, 0xd2, 0xc3, 0xf1, 0xe0, 0x18, 0x9, 0x3b, 0x2a, 0x5e, 0x4f, 0x7d, 0x6c, 
	0xa5, 0xb4, 0x86, 0x97, 0xe3, 0xf2, 0xc0, 0xd1, 0x29, 0x38, 0xa, 0x1b, 0x6f, 0x7e, 0x4c, 0x5d, 
	0xb5, 0xa4, 0x96, 0x87, 0xf3, 0xe2, 0xd0, 0xc1, 0x39, 0x28, 0x1a, 0xb, 0x7f, 0x6e, 0x5c, 0x4d, 
	0xc6, 0xd7, 0xe5, 0xf4, 0x80, 0x91, 0xa3, 0xb2, 0x4a, 0x5b, 0x69, 0x78, 0xc, 0x1d, 0x2f, 0x3e, 
	0xd6, 0xc7, 0xf5, 0xe4, 0x90, 0x81, 0xb3, 0xa2, 0x5a, 0x4b, 0x79, 0x68, 0x1c, 0xd, 0x3f, 0x2e,
	0xe7, 0xf6, 0xc4, 0xd5, 0xa1, 0xb0, 0x82, 0x93, 0x6b, 0x7a, 0x48, 0x59, 0x2d, 0x3c, 0xe, 0x1f,
	0xf7, 0xe6, 0xd4, 0xc5, 0xb1, 0xa0, 0x92, 0x83, 0x7b, 0x6a, 0x58, 0x49, 0x3d, 0x2c, 0x1e, 0xf

};

#endif
#endif

/******************************************
Fun: 主循环处理函数
Input: NULL
Output:NULL
Return:NULL
******************************************/
void MainHandle()
{
//	if(0 != secflag)
//	{
//		secflag = 0;
//	}
	FrameHandle();
}
/*******************************************************************************
函数功能:精确延时(毫秒)
*******************************************************************************/
u32 TimingDelay;
u8 secflag;
void DelayMs(u32 nTime)
{
    TimingDelay = nTime;
    while(0 != TimingDelay)
	{
		asm("nop");
	}
}

#ifndef OS
void StartTimer(TimerParaStu* timer)
{
	timer->sw = 1;
}

void ReloadTimer(TimerParaStu* timer, u32 cnt)
{
	timer->timevalue = cnt;
	timer->cnt = cnt;
}

void StopTimer(TimerParaStu* timer)
{
	timer->sw = 0;
}
#endif

void APIInitTimer(TimerParaStu* timer, u32 time_ms, void* function, void* arg)
{
#ifdef OS
	mico_init_timer(&timer->timer, time_ms, (timer_handler_t) function, arg );
#else
	
#endif
}

void APIReloadTimer(TimerParaStu* timer, u32 time_ms)
{
#ifdef OS
	mico_reload_timer(&timer->timer);
#else
	ReloadTimer(timer, time_ms);
#endif
}
	
void APIStartTimer(TimerParaStu* timer)
{
#ifdef OS
	mico_start_timer(&timer->timer);
#else
	StartTimer(timer);
#endif
}

void APIStopTimer(TimerParaStu* timer)
{
#ifdef OS
//	mico_deinit_timer( (mico_timer_t*) timer );
	mico_stop_timer(&timer->timer );
#else
	StopTimer(timer);
#endif
}
/*******************************************************************************
函数功能:系统1ms时钟设置
*******************************************************************************/
void HAL_SYSTICK_Callback()
{
	if(0 != TimingDelay)
	{
		TimingDelay--;		//精确延时(毫秒级)
	}
	
/*************秒事件处理***************/
	static u16 secondcnt = 1000;
	secondcnt--;
	if(0 == secondcnt)
	{
		secflag = 1;
		secondcnt = 1000;
	}
/*************秒事件处理***************/	

/*************接收超时***************/
#ifndef OS
if(0 != framerevtimer.sw)
{

	framerevtimer.cnt--;
	if(0 == framerevtimer.cnt)
	{
		framerevtimer.cnt = framerevtimer.timevalue;
		framerevtimer.state = 1;
	}
}
/*************接收超时***************/

/*************重发*******************/
	if(0 != framesendtimer.sw)
	{
		framesendtimer.cnt--;
		if(0 == framesendtimer.cnt)
		{
			framesendtimer.cnt = framesendtimer.timevalue;
			framesendtimer.state = 1;
		}
	}
/*************重发*******************/
	
#ifdef DEVEICE
/*************按键处理***************/
	if(0 != keypara.keytimer.sw)
	{
		keypara.keytimer.cnt--;
		if(0 == keypara.keytimer.cnt)
		{
			keypara.keytimer.cnt = keypara.keytimer.timevalue;
			keypara.keytimer.state = 1;
		}
	}
/*************按键处理***************/

/*************报警*******************/
	if(0 != alarmpara.alarmtimer.sw)
	{
		alarmpara.alarmtimer.cnt--;
		if(0 == alarmpara.alarmtimer.cnt)
		{
			alarmpara.alarmtimer.cnt = alarmpara.alarmtimer.timevalue;
			alarmpara.alarmtimer.state = 1;
		}
	}
/*************报警*******************/

/*************信息汇报***************/
	if(0 != inforeportpara.timer.sw)
	{
		inforeportpara.timer.cnt--;
		if(0== inforeportpara.timer.cnt)
		{
			inforeportpara.timer.cnt = inforeportpara.timer.timevalue;
			inforeportpara.timer.state = 1;
		}
	}
/*************信息汇报***************/
#endif
#endif
}

/******************************************
Fun: 数据加密
Input: data:
Output:void
Return:void
******************************************/
void Encoding(u8* data, u8 len)
{

}

/******************************************
Fun: 数据解密
Input: void
Output:void
Return: 解密结果 OK:成功; ERROR:失败
******************************************/
u8 Decoding(u8* data, u8 len)
{
	return OK;
}

/******************************************
Fun: CRC单字节校验
Input: data:校验的数据据;crc:上一个校验数据的值
Output:无
Return: CRC校验值
******************************************/
u16 crc16_ccitt(u8 data, u16 crc)
{
	u16 ccitt16 = 0x8408;

	int i;

	crc ^= data; /* 新的数据与将原来的余数（就是crc）相加（加法就是异或操作） */

	/* 求数据的CRC校验码 */
	for (i=0; i<8; i++)
	{
		if (crc & 0x01) /* 最高位为1,减去除数 */
		{
			crc >>= 1;
			crc ^= ccitt16;
		}
		else /* 最高位为0,不需要减去除数 */
		{
			crc >>= 1; /* 直接移位 */
		}
	}
	
	return crc;
}

/*生成CRC16校验表*/
#if 0
void init_crc16_ccitt_table16bit()
{
	int i;

	for (i=0; i<256; i++)
	{
		CRC16_CCITT_TABLE[i] = crc16_ccitt(i, 0);
	}
}

void init_crc16_ccitt_table8bit()
{
	int i;

	for (i=0; i<256; i++)
	{
		CRC16_CCITT_TABLE8H[i] = crc16_ccitt(i, 0)>>8;
		CRC16_CCITT_TABLE8L[i] = crc16_ccitt(i, 0);
	}
}
#endif
/*生成CRC16校验表*/

//CRC16/X25
#ifdef CRCLIST
/******************************************
Fun: CRC16校验(查表法)
Input: data:校验数据地址
	   len:校验数据长度
Output:NULL
Return:校验和
******************************************/
u16 crc_cal(u8 *data, u8 len)
{
	u8 i;
	u8 c = 0;
	u16 crc = 0xffff;

#ifdef CRCLIST16
	for(i=0; i<len; i++)
	{
		c = crc&0xff;
		crc >>= 8;
		crc ^= CRC16_CCITT_TABLE[data[i]^c];
	}
#else
	u8 crc_h = 0xFF;
	u8 crc_l = 0xFF;
	
	for(i=0; i<len; i++)
	{
		c = crc_l^data[i];
		crc_l = crc_h^CRC16_CCITT_TABLE8L[c];
		crc_h = CRC16_CCITT_TABLE8H[c];
	}
	crc = crc_h<<8 | crc_l;
#endif
	return ~crc;
}

#else
/******************************************
Fun: CRC16校验
Input: data:校验数据地址
	   len:校验数据长度
Output:NULL
Return:校验和
******************************************/
u16 crc_cal(u8 *data, u8 len)
{
	u16 crc = 0xFFFF;
	
	while(len--)
	{
		crc = crc16_ccitt(*data, crc);
		data++;
	}
	return ~crc;
}
#endif

/******************************************
Fun: 和校验
Input: data:校验数据地址
	   len:校验数据长度
Output:NULL
Return:校验和
******************************************/
u8 SumCheck(u8 *data, u8 len)
{
	u8 checksum = 0;
	
	while(len--)
	{
		checksum += *(data++);
	}
	
	return checksum;
}

/******************************************
Fun: 异或校验
Input: data:校验数据地址
	   len:校验数据长度
Output:NULL
Return:校验和
******************************************/
u8 XorCheck(u8 *data, u8 len)
{
	u8 checksum = 0;
	
	while(len--)
	{
		checksum ^= *(data++);
	}
	
	return checksum;
}
///*******************************************************************************
//函数功能:循环加减
//参数	type:循环方向 0:加(正方向) 1:减(反方向) 
//		currentnum:当前值地址
//		totalnum:总数
//*******************************************************************************/
//void AddSubRecycle(u8 type, u8 *curnum, u8 tonum)
//{
//	if(0 != tonum)
//	{
//		if(0 != type)		//减
//		{
//			*curnum = tonum - *curnum - 1;
//		}
//		
//		(*curnum)++;
//		*curnum %= tonum;
//		
//		if(0 != type)		//减
//		{
//			*curnum = tonum - *curnum - 1;
//		}
//	}
//}
//
///************************************************************************
//功能:  NegMemCpy
//** Description       :  拷贝源数据的反码到目标单元
//** Input Parameters  :
//               *des  : 目标地址指针
//               *src  : 源数据指针
//                len  : 数据长度
//** Output Parameters : none
//*************************************************************************/
//void NegMemCpy(u8 *des, u8 *src, u8 len)
//{
//    u8 i;
//    for (i=0; i<len; i++)
//    {
//        des[i] = 0xFF - src[i];
//    }
//}
//
///************************************************************************
//** Function Name     :  power10
//** Description       :  计算10的y次幂: 10^y
//** Input Parameters  :
//**                y  :  指数(最大值 = 9)
//** Output Parameters :
//**                   =  10^y
//** Modify History    :
//*************************************************************************/
//u32 power10 (u8 y)
//{
//    u32 res = 1;
//    u8 i;
//
//    for (i = 0; i < y; i++)
//    {
//        res *= 0x0A;
//    }
//    return (res);
//}
//
//u8 DePower(u32 data)
//{
//	u8 temp = 0;
//	
//	while(1)
//	{
//		if(data<10)
//		{
//			return temp;
//		}
//		data /= 10;
//		temp++;
//	}
//}
///************************************************************************
//** Function Name     :  Ascii2Hex
//** Description       :  Ascii字符转换为hex值
//** Input Parameters  :
//
//** Modify History    :
//************************************************************************/
//u8 Ascii2Hex(u8 ascii)
//{
//	if(('0' <= ascii) && ('9' >= ascii))
//	{
//		return ascii - '0';
//	}
//	
//	else if(('a'<= ascii) && ('f' >= ascii))
//	{
//		return ascii - 'a' + 10;
//	}
//	
//	else if(('A'<= ascii) && ('F' >= ascii))
//	{
//		return ascii - 'A' + 10;
//	}
//	
//	return 0;
//}
//
///************************************************************************
//** Function Name     :  Str2Hex
//** Description       :  Ascii字符集转换为hex值"3251"->3251
//** Input Parameters  :
//**             *str  :  Ascii字符集,不是字符串(没有字符串结束符)
//               len   :  Ascii字符集的长度
//** Output Parameters :  
//                        整型数
//** Modify History    :
//************************************************************************/
//u32 Str2Hex (u8 *str, u8 len)
//{
//    u8 i;
//    u32 res = 0;
//
//    for (i=0; i<len; i++)
//    {
//        res += (Ascii2Hex(str[i]) * power10(len-1-i));
//    }
//    return (res);
//}
//
///************************************************************************
//** Function Name     :  DecStr2Hex
//** Description       :  带小数位的Ascii字符集转换为hex值
//** Input Parameters  :
//**             *str  :  Ascii字符集,不是字符串(没有字符串结束符)
//               len   :  Ascii字符集的长度
//               fl    :  Ascii字符集中小数位数
//** Output Parameters :
//                        长型数
//** note
//                        例子
//                        str[] = "1.235" ; len=5; fl=2;
//                        结果 = 123
//
//                        str[] = "1.235" ; len=5; fl=1;
//                        结果 = 12
//
//                        str[] = "1.5" ; len=3; fl=3;
//                        结果 = 1500
//
//                        str[] = "15" ; len=2; fl=0;
//                        结果 = 15
//
//** Modify History    :
//*/
//u32 DecStr2Hex (u8 *str, u8 len, u8 fl)
//{
//    u8   i,j;
//    u8   il;    //整数位数据长度
//    u32  res;
//
//    //计算整数位长度
//    il = 0x00;
//    for (i=0; i<len; i++)
//    {
//        if (str[i] != '.')
//        {
//            il++;
//        }
//        else
//        {
//            break;
//        }
//    }
//    il += fl;     //整数长度+小数长度
//
//    res = 0x00;
//    j = 0x00;
//    il--;
//    for (i=0; i<len; i++)
//    {
//        if (str[i] != '.')
//        {
//            res += (Ascii2Hex(str[i]) * power10(il-j));
//            j++;
//        }
//    }
//
//    return (res);
//}
//
///*************************************************
//功能:将hex数据转换为bcd数据
//参数:
//	hex:hex数据
//	type:数据类型  u8:0 u16:1 u32:2
//返回值:bcd数据
//*************************************************/
//u32 HexToBcd(u32 hex, u8 type)
//{
//	u8 i;
//	u32 bcd = 0;
//	u8 len = 0;    
//
//	len <<= type;
//    for(i=0x00; i<len; i++)
//    {
//        bcd |= (hex % 10) << (4*i);
//		hex /= 10;
//    }
//    return bcd;
//}
//
///*************************************************
//功能:将BCD码转换为hex数据
//参数:
//	bcd:bcd数据
//	type:数据类型  u8:0  u16:1 u32:2
//返回值: hex数据
//*************************************************/
//u32 BcdToHex(u32 bcd, u8 type)
//{
//	u8 i;
//	u32 hex = 0;
//	u8 len = 2;
//
//	len <<= type;
//	for(i=0; i<len; i++)
//	{
//		hex += (bcd&0x0F)*power10(i);
//		bcd >>= 0x04;
//	}
//	return hex;
//}
//
///*************************************************
//功能:得到源数据bit位的值
//参数:
//	src:源数据
//	bit:源数据的位，0代表个位，依次类推
//	type:源数据的类型 0:hex  1:bcd
//返回值:源数据bit位的值
//*************************************************/
//u8 GetBitValue(u32 src, u8 bit, u8 type)
//{
//	if(0 == type)
//	{
//		return (src%(bit*power10(bit+1))/power10(bit));
//	}
//	else
//	{
//		return ((src >> (bit*4)) & 0x0F);
//	}
//}
//
///*************************************************
//功能:设置源数据bit位的值
//参数:
//	src:源数据的地址
//	bit:要设置的位，0代表个位，依次类推
//	data:源数据要设置的bit位的值
//	type:源数据的类型 0:hex  1:bcd
//返回值:无	
//*************************************************/
//void SetBitValue(u32 *src, u8 bit, u8 data, u8 type)
//{
//	if(0 == type)
//	{
//		u8 temp = 0;
//		
//		temp = GetBitValue((*src), bit, type);
//		*src -= temp*power10(bit);
//		*src += data*power10(bit);
//	}
//	else
//	{
//		*src &= ~(0x0000000F << (bit*4));
//		*src |= ((u32)data) << (bit*4);
//	}
//}
//
/*************************************************
功能:以16进制格式打印buf中的数
参数:
	buf:起始地址
	len:打印数据长度
返回值:无	
*************************************************/
void PrintfMem(u8 *buf, int len)
{
	int i = 0;
	printf("len = %d\r\n", len);
    printf("***************Mem begin******************\r\n");
	for(i = 0; i < len; i++)
	{
		printf("%02x ", buf[i] & 0xff);
		if(!((i+1) % 10)) printf("\r\n");
	}
	printf("\r\n***************Mem end********************\r\n");
}

/*************************************************
功能:循环队列初始化
参数:
	fifo:队列名
	src:队列起始地址
	fifolen:队列长度
返回值:无	
*************************************************/
void FifoInit(FifoTyp *fifo, u8 *src, u16 fifolen)
{
	fifo -> FifoAddr = src;
	fifo -> FifoLen = fifolen;
	fifo -> Datalen = 0;
	fifo -> FifoOffset = 0;
//	fifo -> State = 0;
}

/*************************************************
功能:循环队列写入
参数:
	fifo:队列名
	src:写入数据起始地址
	len:写入数据长度
返回值: TRUE:写入成功  FALSE:写入失败
*************************************************/
u8 FifoPushOneByte(FifoTyp *fifo, u8 data)
{
	u16 fifohead;	//FIFO的头地址
	u16 datalen = 0;
	u16 offset = 0;
	
	datalen = fifo->Datalen;
	offset = fifo->FifoOffset;
	if(fifo->Datalen < fifo->FifoLen)
	{
		fifohead = datalen + offset;
//		fifohead = fifo->Datalen + fifo->FifoOffset;
		fifohead %= fifo->FifoLen;
		fifo->FifoAddr[fifohead] = data;
		fifo->Datalen++;
		return TRUE;
	}

	else		//写入数据后超过FIFO长度
	{
#ifdef	DEBUGFIFO
		printf("写入数据后超过FIFO长度\r\n");
#endif
		return FALSE;
	}
}

/*************************************************
功能:循环队列写入
参数:
	fifo:队列名
	src:写入数据起始地址
	len:写入数据长度
返回值: TRUE:写入成功  FALSE:写入失败
*************************************************/
_Bool FifoInput(FifoTyp *fifo, u8 *src, u16 len)
{
	u16 cnt;			//写入FIFO后的数据长度
	u16 fifoheadstart; 	//FIFO的头地址
	u16 fifoheadend;	//写入数据后的FIFO头地址
	u16 temp;			

	u16 datalen = 0;
	u16 offset = 0;
	
	datalen = fifo->Datalen;
	offset = fifo->FifoOffset;
	
	cnt = fifo->Datalen + len;	

	if(cnt <= fifo->FifoLen)
	{
		fifoheadstart = datalen + offset;
//		fifoheadstart = fifo->Datalen + fifo->FifoOffset;
		fifoheadend = fifoheadstart + len;
		
		if((fifoheadstart >= fifo->FifoLen) || (fifoheadend <= fifo->FifoLen))		
		{
			fifoheadstart %= fifo->FifoLen;			
			memcpy(&fifo->FifoAddr[fifoheadstart], src, len);
		}
		else		
		{
			temp = fifo->FifoLen - fifoheadstart;
			memcpy(&fifo->FifoAddr[fifoheadstart], src, temp);
			
			src += temp;
			temp = len - temp;
			memcpy(&fifo->FifoAddr[0], src, temp);
		}
		
		fifo->Datalen = cnt;
		return 1;
	}
	else		//写入数据后超过FIFO长度
	{
#ifdef	DEBUGFIFO
		printf("写入数据后超过FIFO长度\r\n");
#endif
		return 0;
	}
}

/*************************************************
功能:循环队列读出
参数:
	des:数据读出的起始地址
	fifo:队列名
	len:读出数据长度
返回值:读出数据的长度
*************************************************/
u16 FifoOutput(u8 *des, FifoTyp *fifo, u16 len)
{
	u16 fifolen;
	u16 temp;
	u16 fifooutlen;	

	fifooutlen = (len <= fifo->Datalen) ? len:fifo->Datalen;
	fifolen = fifo->FifoLen - fifo->FifoOffset;
	
	if(fifooutlen <= fifolen)
	{
		memcpy(des, &fifo->FifoAddr[fifo->FifoOffset], fifooutlen);
		fifo->FifoOffset += fifooutlen;
	}
	else
	{
		temp = fifolen;
		memcpy(des, &fifo->FifoAddr[fifo->FifoOffset], temp);
		
		des += temp;
		temp = fifooutlen - fifolen;
		memcpy(des, &fifo->FifoAddr[0], temp);
		
		fifo->FifoOffset = temp;
	}

	fifo->Datalen -= fifooutlen;
	
	return fifooutlen;
}

/*************************************************
功能:循环队列复制头
参数:
	des:目的地址
	fifo:队列名
	offset:相对于FIFO尾的偏移
	len:数据长度
返回值:实际复制数据的长度
*************************************************/
u16 FifoCopy(u8 *des, FifoTyp *fifo, u16 offset, u16 len)
{
	u16 fifolen = 0;
	u16 temp = 0;
	u16 fifooutlen = 0;	
	u16 fifooffset = 0;

	if(offset <= fifo->Datalen)
	{
		fifooutlen = (len <= (fifo->Datalen - offset)) ? len:(fifo->Datalen - offset);
		fifooffset = (fifo->FifoOffset+offset)%fifo->FifoLen;
		if(0 != fifooutlen)
		{
			fifolen = fifo->FifoLen - fifooffset;

			if(fifooutlen <= fifolen)
			{
				memcpy(des, &fifo->FifoAddr[fifooffset], fifooutlen);
			}
			else
			{
				temp = fifolen;
				memcpy(des, &fifo->FifoAddr[fifooffset], temp);
				
				des += temp;
				temp = fifooutlen - fifolen;
				memcpy(des, &fifo->FifoAddr[0], temp);
			}
		}
	}

	return fifooutlen;
}

/*************************************************
功能:循环队列删除头
参数:
	fifo:队列名
	len:删除数据长度
返回值:删除数据的长度
*************************************************/
u16 FifoDeletHead(FifoTyp *fifo, u16 len)
{
	u16 fifodellen;	

	fifodellen = (len <= fifo->Datalen) ? len:fifo->Datalen;

	fifo->Datalen -= fifodellen;
	return fifodellen;
}

/*************************************************
功能:循环队列删除尾
参数:
	fifo:队列名
	len:删除数据长度
返回值:删除数据的长度
*************************************************/
u16 FifoDeletTail(FifoTyp *fifo, u16 len)
{
	u16 fifolen = 0;
	u16 fifodellen = 0;	

	fifodellen = (len <= fifo->Datalen) ? len:fifo->Datalen;
	fifolen = fifo->FifoLen - fifo->FifoOffset;
	
	if(fifodellen <= fifolen)
	{
		fifo->FifoOffset += fifodellen;						//尾往前移动fifodellen
	}
	else
	{
		fifo->FifoOffset -= fifo->FifoLen - fifodellen;		//尾实际往后移动
	}

	fifo->Datalen -= fifodellen;
	
	return fifodellen;
}

