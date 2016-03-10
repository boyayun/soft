#ifndef __DEVEICE_H
#define __DEVEICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
    
//#define KONGTIAO
#define DEBUGFRAMEREV		0		//运行模式 0:正常运行 1:DEBUG

/*定义命令字*/
/*通迅模块<->设备*/
#define CMDCONFIRM			0x05	//确认帧
/*通迅模块<->设备*/

/*通迅模块->控制中心/服务器*/
#define CMDCOMMERROR		0x1C	//模块与设备通信异常
/*通迅模块->控制中心/服务器*/

/*通迅模块->设备*/
#define CMDCONTROL			0x01	//控制帧
#define CMDINFOREPORTCFG	0x06	//信息汇报配置帧
#define CMDCHECKSTE			0x08	//查询设备状态帧
#define CMDSTOPALARM		0x09	//停止报警帧
#define CMDRDDEVID			0x10	//读取设备ID
#define CMDRDDEVINFO		0x12	//读取设备信息
#define CMDRETENCRYPARA		0x15	//返回加密参数
#define CMDBPSTEST			0x16	//波特率测试
#define CMDRETLINKSTE		0x18	//链路状态返回
#define CMDDEVRST			0x19	//强制设备复位
#define CMDRDDEVPROFILE		0x1A	//读取设备描述文件
#define CMDRSSI				0x30	//无线信号强度汇报
#define CMDRETWORKMODE		0x32	//返回工作模式
#define CMDWRDEVPROFILE 	0x80	//加载设备描述文件
#define CMDRETMODINFO		0x83	//返回通讯模块信息
#define CMDUPPACK			0xA0	//设备升级包发送
#define CMDUPAPPCHK			0xA2	//发送设备升级程序校验码
/*通迅模块->设备*/

/*设备->通迅模块*/
#define CMDSTE				0x02	//状态帧
#define CMDINVALID			0x03	//无效帧
#define CMDALARM			0x04	//异常/报警帧
#define CMDINFOREPORT		0x07	//信息汇报帧
#define CMDRETDEVID			0x11	//返回设备ID
#define CMDRETDEVINFO		0x13	//返回设备信息
#define CMDRDENCRYPARA		0x14	//读取加密参数
#define CMDRDLINKSTE		0x17	//链路状态查询
#define CMDRETDEVPROFILE	0x1B	//返回设备描述文件
#define CMDSETWORKMODE		0x31	//设置工作模式
#define CMDACKDEVPROFILE 	0x81	//设备描述文件接收确认
#define CMDRDMODINFO		0x82	//查询通讯模块信息
#define CMDACKUPPACK		0xA1	//设备升级包接收应答
#define CMDACKUPAPPCHK		0xA3	//设备升级程序校验码接收应答
/*设备->通迅模块*/
/*定义命令字*/

#define FRAMEDATALEN		131		//一帧数据最大131字节
#define FRAMEHEADER			0xF5	//帧头
#define CRCCHECK			0x00	//CRC校验
#define SUMCHECK			0x01	//累加和校验
#define BCCCHECK			0x02	//BCC校验

#define FRAMELEN			0x05	//帧固定长度

#define DEVIDLEN  36				//设备ID的长度

#define TEMPALARMVALUE	26			//高温报警值
#define ALARMCNTNOMAL 	200			//正常200ms报警
#define ALARMCNTHANDLED 5000		//正常5000ms报警

#define KEYSHAKECNT	20		//抖动时间，单位毫秒
#define KEYDOUBLECLICK	400		//双击时间，单位毫秒

#define REVHEAD	 	0		//正在接收头
#define REVCTRL  	1		//正在接收控制字
#define REVLEN 		2		//正在接收数据长度
#define REVDATA		3		//正在接收数据

typedef enum	
{
KeyStateUp = 0,			//抬起
KeyStateDowning,		//正在按下
KeyStateDown,			//正在抬起
KeyStateUping,			//按下
}EnumKeyState;

typedef enum	
{
KeyNumNOkey = 0,		//没有键 按下
KeyNumMeum,				//菜单键
KeyNumUp,				//上键
KeyNumDown,				//下键
KeyNumOK,				//确定键
KeyNumNull				//不识别
}EnumKeyNum;

typedef struct	
{
    EnumKeyState KeyParaState;	//按键状态
    u8 KeyParaShake;			//抖动状态
    TimerParaStu keytimer;
}StuKeyPara;

typedef struct	
{
    EnumKeyNum KeyValueNum;		//按键编号
    u8 KeyValueDown;			//按下有效
    u8 KeyValueUp;				//抬起有效
    u8 KeyValueLong;			//长按有效
}StuKeyValue;

extern StuKeyPara	keypara;
extern StuKeyValue keyvalue;

extern void KeyScan();

typedef struct
{
    TimerParaStu alarmtimer;		//最大定时66535ms
    u8 state;						//报警状态 0:无报警 1:正常报警 2:已确认 3:停止报警 4:已设置
}AlarmStuType;

typedef struct
{
    u8 softversions[4];
    u8 hardversions[4];
    u8 encrypt;
}DevInfoStuType;

#define DEVCFGFREMIN   100		//最小信息汇报间隔，单位:100ms
#define INFOREPORTCNT  100		//100ms
typedef struct
{
    u8 sw;
    u8 state;			//汇报时间到
    u16 timercnt;		//定时器计时时间
    u16 reportcnt;		//汇报时间
    TimerParaStu timer;
}InfoReportStuType;

typedef struct
{
#ifdef KONGTIAO
    u8 power;		//电源开关	0:关 1:开
	u8 temparature;	//温度
    u8 mode;		//模式选择  0:制冷 1:制热 2:除湿
    u8 wind;		//风量
//    u8 date[4];		//年、月、日
//    u8 time[3];		//时分秒
    u8 tempalarm;	//高温报警
#else
	u8 power;		//电源开关	0:关 1:开
    u8 mode;		//模式选择  0:制冷 1:制热 2:除湿
#endif
}DevStateStuType;
/*识别信息结构体*/
//typedef struct
//{
//	u8 DD_Version;				//描述文件标准的版本
//	u8 DD_Encode;				//描述文件类型
//	u8 DD_Devictype;			//描述文件适用的用户
//	u8 Manufacturer_Code[13];	//厂商代码
//	u8 DD_fileVer;				//描述文件版本
//	u8 Device_TypeID[2];		//家电的大类和小类
//	u8 Model_ID[3];				//家电型号代码
//	u8 DD_URL_Lenth[1];			//url连接地址的长度
//	u8 DD_URL[20];				//描述文件url连接地址
//	u8 Update;					//00：不具备升级能力	01：具备升级能力
//	u8 Synchronous;				//00：不具备同步时间能力	01：具备同步时间能力
//}DevInfoStuType;
/*识别信息结构体*/

/*开关类型结构*/
//typedef struct
//{
//	u8 Switch;		//开关量
//}ButtonStuType;
/*开关类型结构*/

/*开关类型结构*/
//typedef struct
//{
//	u8 Switch;		//数字量
//}ButtonStuType;
/*数字类型结构*/

/*设备描述文件*/
//typedef struct
//{
//	DevInfoStuType devinfo;
//}DevDescFileStuType;
/*设备描述文件*/

/*帧数据*/
typedef struct
{
    u8 	datalen;				//帧数据长度(3~131),数据最长128字节
    //	u8  cmdtype;				//帧类型
    u8  data[FRAMEDATALEN];		//帧数据数组
    // 	union_u16 checksum;			//校验和
}FrameDataStu;
/*帧数据*/

/*帧控制*/
typedef struct
{
    u8 versions : 2;	//协议版本 bit0~1 00:TCL智能家居基础通讯协议
    u8 reserved : 3;	//预留 bit2~4
    u8 crctype  : 2;	//校验类型 bit5~6 00:CRC; 01:累加和; 10:BCC
    u8 encrypt	: 1;	//是否加密 bit7 0:无加密
}FrameCtrlStu;

typedef union
{
    u8 byte;
    FrameCtrlStu bit;	
}FrameCtrlUni;
/*帧控制*/

/*数据接口结构体*/
typedef struct
{
    u8 framehead[2];
    FrameCtrlUni framectrl;	//帧控制域
    FrameDataStu framedata; //帧数据域
    //数据状态 0:无数据或数据已处理完 1:开始写入数据,内存已被占用
    //2:数据写入完成可以处理 3:数据正在进行处理(只有为0时才可写入)
    u8 state;
}DataInterfaceStu;		
/*数据接口结构体*/

/*帧接收参数*/
typedef struct
{
	u8 wordstate;	//字判断状态 0:初始状态; 1:正在判断;
	u8 datastate;	//数据状态 0:判断头; 1:正在接收控制字; 2:正在接收数据长度; 3:正在接收数据; 4:数据接收完成
	u16 offset; 	//写入数据的偏移
//	u8 len; 		//实际接收数据的长度
	u8 errortype;	//数据错误类型 0:初始值 1:校验错误 2:解密失败 3:错误数据 4:校验正确
}FrameRevParaStu;
/*帧接收参数*/

/*帧发送参数*/
#define RESENDCNT	3	//重发3次
typedef struct
{
    u8 len;			//发送数据的长度(控制字+数据长度+数据的长度)
    u8 resendcnt; 	//重发次数计数值
}FrameSendParaStu;
/*帧发送参数*/

/*帧定时参数*/
#define FRAMEREVCNT		100		//接收定时 单位:ms 不宜太大，绝对不能超过重发时间270MS
#define FRAMESENDCNT	270		//重发定时 单位:ms
/*帧定时参数*/

extern InfoReportStuType inforeportpara;
extern AlarmStuType alarmpara;
extern TimerParaStu framesendtimer;
extern TimerParaStu framerevtimer;
extern DataInterfaceStu dataup;				//设备数据上传接口
extern DataInterfaceStu datadown;			//下行数据接口

extern DevStateStuType devstate;			//设备状态

extern void DevHardInit();
extern void DevParaInit();
extern void DevFrameHandle(void);			//帧处理主循环
extern void RevDataWrFifo(u8* data, u8 len);

#ifdef __cplusplus
}
#endif

#endif /* d */
