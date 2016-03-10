// 
//  串口类头文件 
//  版权 (C) 2006 LIBIN
//  保留所有权
// 
//  注意事项:
//  该文件定义了一个串口通讯的类, 类对象用在win32操作系统下,
//  该类对象与外部交流通过自定义窗体消息实现;该类对象实现了
//  单一串口向多窗体发送数据的能力.
//
//  版本关系:
//  2006-6  V1.0    实现交叠数据收发、窗体附加等功能
//  2006-7  V1.0.1  改进了由于I/O操作过于繁忙导致窗口事件不响应的问题
//
//  申明:
//  该代码可以随意修改和传播，原作者不承担一切责任。如有问题发送邮件到
//  <i_mlibin@163.com>
//

#ifndef _MOONZWU_SUPER_SERIAL_PORT
#define _MOONZWU_SUPER_SERIAL_PORT
#define VERSION1_1

#include <vector>
using namespace std;



typedef struct WIN_ARG
{
	UINT  winId;
	CWnd* pWin;
	DWORD msg;

	WIN_ARG()
	{
		winId = 0;
		pWin  = NULL;
		msg   = 0x00;
	}
}WINARG, *pWINARG;


typedef vector<WIN_ARG>   vWINLIST; // 窗口列表





// 传输数据包
typedef struct tagPackage
{
	enum emMaxSize
	{ emMAX_SIZE = 1024 };
	
	DWORD iLen;
	UINT iType;
	BYTE pData[emMAX_SIZE];

	tagPackage()
	{
		Clear();
	}

	void Clear()
	{
		iLen  = 0;
		iType = 0;
		ZeroMemory(pData, sizeof(*pData));		
	}
}PACKAGE, * pPACKAGE;



class CSuperSerialPort : public CObject
{
public:
	CSuperSerialPort();
	virtual ~CSuperSerialPort();

	// 
	BOOL Open(TCHAR* com,
		      UINT rate,
			  UINT parity,
			  BYTE dataBits,
			  UINT stopBits);

	// 
	BOOL IsOpen() { return m_bIsOpen; }
	
	//
	void Close();
	void ClearSendBytes();
	void ClearReceiveBytes();

	//
	UINT Attach(CWnd* pwin, DWORD msg);
	void Detach(UINT winId);


	//
	BOOL SendData(pPACKAGE pPack); // 发送数据
	BOOL StartReceive(LPVOID arg); // 启动接收, 参数为串口指针

	//
	COMSTAT* GetComState(); // 获取串口状态
	HANDLE   GetComHandle();// 获取串口句柄

	//
	ULONG    GetWritedBytes();
	ULONG    GetReadedBytes();

#ifdef VERSION1_1
	BOOL     GetFileTransferFlag() const;
	void     OpenFileTransfer(CWnd* pwnd, DWORD msg);
	void     CloseFileTransfer();
#endif	

	// 设置输入输出缓存区
	void     SetInOutBuffer(DWORD inBuf, DWORD outBuf);

	// 获取错误信息
	DWORD    GetErrorCode();
	const TCHAR*   FormatErrorCodeToString(DWORD code);

	
	static void SafeRelease(pPACKAGE pPack); // 释放包

	typedef struct COM_ARG
	{
		TCHAR  com[10];
		HANDLE comHandle;
		UINT   comRate;
		UINT   comParity;
		BYTE   dataBits;
		UINT   stopBits;

		COM_ARG()
		{
			sprintf(com, _T("COM1"));
			comHandle = INVALID_HANDLE_VALUE;
			comRate   = 9600;
			comParity = 0;
			dataBits  = 8;
			stopBits  = 1;
		}
		
	}COMARG, *pCOMARG;


	// 错误类型枚举
	enum emErrorType
	{
		emET_SUCCEDED = 0,
		emET_NO_OPEN,
		emET_NO_HANDLE,
		emET_OPENED,
		emET_COM_UNNAMED,
		emET_UNKNOW = 99
	};
	
	// 数据类型
	enum emDataType
	{
		emDATA_TYPE_TEXT=0, emDATA_TYPE_BINARY
	};

	// 线程事件数量
	enum emThreadEvent
	{ 
		emTHREAD_EXIT = 0,
		emTHREAD_RUN = 1,
		emMAX_EVENT_NUMBER = 2 
	};
	
protected:
    
	// 接收线程
    friend ULONG WINAPI ReceiveThread(LPVOID pVoid);
	friend void PostPackage(CSuperSerialPort* pCom, pPACKAGE pPack);

	BOOL WriteComm(pPACKAGE pPack);

private:
	
	COMARG   m_com;
	COMSTAT  m_comState;
	DWORD    m_errorCode;
	TCHAR    m_errorInfo[128];
	BOOL     m_bIsOpen;
	
#ifdef VERSION1_1
	BOOL     m_bIsTransferFile; // 传输文件标志
	CWnd*    m_pFileWnd;        // 文件接收窗体
	DWORD    m_dwDoFileMsg;     // 处理文件消息
#endif

	vWINLIST m_winList;

	/*------------------------------*/
	OVERLAPPED    m_ovReader;                          // Overlapped structure for ReadFile
    OVERLAPPED    m_ovWriter;                          // Overlapped structure for WriteFile
    OVERLAPPED    m_ovWaitEvent;                       // Overlapped structure for WaitCommEvent
	HANDLE        m_hWait;
	HANDLE        m_hReader;
	HANDLE        m_hWriter;
	
	
	UINT     m_nextWinId; // 返回附加的窗体Id

	ULONG    m_WriteCount; // 已发送字节数
	ULONG    m_ReadCount;  // 已接收字节数

	/*--------------------------------*/
	HANDLE   m_hEvent[emMAX_EVENT_NUMBER];    // 线程控制事件句柄
	DWORD     m_threadId;
	CRITICAL_SECTION m_csRead; // 读写字节统计的临界区
	CRITICAL_SECTION m_csWrite;
	HANDLE   m_hReceiveThread;
};
#endif