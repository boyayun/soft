#include "stdafx.h"
#include "superSP.h"
#include <process.h>


//////////////////////////////////////////////////////////////////////////
// 静态成员

void CSuperSerialPort::SafeRelease(pPACKAGE pPack)
{
	if (NULL != pPack)
	{
		delete pPack;
		pPack = NULL;
	}
}


// 这两个函数被设计成友元目的是可以访问类的私有变量
ULONG WINAPI ReceiveThread(LPVOID pVoid)
{
	long threadStatus;
	DWORD dwMask, dwTrans;
	DWORD dwErrFlag;
	BOOL bExit = false;
	CSuperSerialPort* pCom = (CSuperSerialPort *)pVoid;

//	
//
	while (!bExit)
	{
		threadStatus = 
			WaitForMultipleObjects(CSuperSerialPort::emMAX_EVENT_NUMBER,
			pCom->m_hEvent,
			FALSE,
			INFINITE);
		threadStatus -= WAIT_OBJECT_0;

		if (threadStatus < 0 ||
			threadStatus >= CSuperSerialPort::emMAX_EVENT_NUMBER)
		{
			bExit = true;
			break;
		}

		// 进行事件处理
		switch (threadStatus)
		{
		case CSuperSerialPort::emTHREAD_EXIT:
			bExit = true;
			break;
//
		case CSuperSerialPort::emTHREAD_RUN:
			ClearCommError(pCom->GetComHandle(),
				&dwErrFlag,
				&pCom->m_comState);

			// 缓存队列中有数据
			if (pCom->m_comState.cbInQue)
			{
				pPACKAGE pPack = new PACKAGE;
				DWORD rdLen = pCom->m_comState.cbInQue;
				pPack->iLen   = rdLen;
				
				// 根据长度读取指定字节到包中
				if (ReadFile(pCom->GetComHandle(), 
					pPack->pData,
					pPack->iLen,
					&rdLen,
					&pCom->m_ovReader))
				{
					// 发送包到win消息队列
					pPack->iLen = rdLen;
                    EnterCriticalSection(&pCom->m_csRead);
					pCom->m_ReadCount += pPack->iLen;
					LeaveCriticalSection(&pCom->m_csRead);
					PostPackage(pCom, pPack);
					continue;
				}
			}

			dwMask = 0;
			
			if (!WaitCommEvent(pCom->GetComHandle(),
				&dwMask,
				&pCom->m_ovWaitEvent))
			{
				// GetLastError查找错误
				DWORD errorCode = GetLastError();
				if (errorCode == ERROR_IO_PENDING)
				{
					GetOverlappedResult(pCom->GetComHandle(),
						&pCom->m_ovWaitEvent,
						&dwTrans,
						TRUE);
				}
				else // 非交叠操作，输出错误信息并退出
				{
					LPVOID lpMsg;
					
					FormatMessage(
						FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						errorCode,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
						(LPTSTR)&lpMsg,
						0,
						NULL);
					
					TRACE((LPCTSTR)lpMsg);
					LocalFree(lpMsg);
					bExit = true;
				}
			}	
			
            break;


		default:
			break;
		}
	}
	
	return 0;
}



void PostPackage(CSuperSerialPort* pCom,  pPACKAGE pPack)
{
	if (NULL != pCom)
	{
		if (pCom->GetFileTransferFlag()) 
		{
			pCom->m_pFileWnd->PostMessage(pCom->m_dwDoFileMsg,
				(WPARAM)pPack);
		}
		else
		{
			vWINLIST::iterator it;
			for (it = pCom->m_winList.begin();
			it != pCom->m_winList.end();
			it++)
			{
				it->pWin->PostMessage(it->msg, (WPARAM)pPack);
			}
		}
	}
}




//////////////////////////////////////////////////////////////////////////
// 构造/析构

CSuperSerialPort::CSuperSerialPort()
: m_nextWinId(1),
  m_WriteCount(0),
  m_ReadCount(0),
  m_bIsOpen(false),
  m_bIsTransferFile(false)
{
	InitializeCriticalSection(&m_csWrite);
	InitializeCriticalSection(&m_csRead);
}

CSuperSerialPort::~CSuperSerialPort()
{

	CloseHandle(m_com.comHandle);	
}

//////////////////////////////////////////////////////////////////////////
// 操作控制

//
// Open 函数
//
// 参数: TCHAR* com    串口名
//       UINT rate     波特率
//       UINT parity   校验位
//       BYTE dataBits 数据位
//       UINT stopBits 停止位
//
// 返回值: true 成功, false 失败
//
// 注意: 如果返回失败,请调用GetErrorCode来查找原因
//

BOOL CSuperSerialPort::Open(TCHAR* com,
							UINT rate,
							UINT parity,
							BYTE dataBits,
							UINT stopBits)
{
	m_errorCode = emET_SUCCEDED;
	COMMTIMEOUTS cto = { MAXDWORD, 0, 0, 50, 2000};

	if (NULL == com)
	{
		m_errorCode = emET_COM_UNNAMED;
		return false;
	}

	strcpy(m_com.com, com);
	m_com.comRate   = rate;
	m_com.comParity = parity;
	m_com.dataBits  = dataBits;
	
	if (stopBits == 1) {
		stopBits = 0;
	} else if (stopBits == 1.5){
        stopBits = 1;
	} else {
		stopBits = 2;
	}

	m_com.stopBits  = stopBits;

	
	// 串口已经打开
	if (m_com.comHandle != INVALID_HANDLE_VALUE)
	{
		m_errorCode = emET_OPENED;
		return FALSE;
	}

	char temp[10] = {};
	char comnum = 0;
	strcpy(temp,m_com.com);
	if(0 == temp[4])
	{
		comnum = temp[3]-0x30;
	}
	else
	{
		comnum = (temp[3]-0x30)*10+temp[4]-0x30;
	}
	if(comnum<10)
	{
	sprintf(m_com.com, "COM%d", comnum);
	}
	else
	{
	sprintf(m_com.com, "\\\\.\\COM%d", comnum);
	}

	m_com.comHandle = CreateFile(m_com.com,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED| FILE_FLAG_OVERLAPPED,
		NULL);

	if (m_com.comHandle == INVALID_HANDLE_VALUE)
	{
		m_errorCode = emET_NO_OPEN;
		return false;
	}
	SetupComm(m_com.comHandle, 4096, 4096);
	SetCommMask(m_com.comHandle, EV_RXCHAR);
	SetCommTimeouts(m_com.comHandle, &cto);

	DCB  dcb;
	if(!GetCommState(m_com.comHandle, &dcb))
		return FALSE;
	//	memset(&dcb, 0, sizeof(dcb));
	//		dcb.DCBlength = sizeof(dcb);
	dcb.BaudRate  = m_com.comRate;
	dcb.Parity    = m_com.comParity;
	dcb.fParity   = m_com.comParity & 0; // 0&0 = 0, 1,2 & 0 = 1
	dcb.StopBits  = m_com.stopBits; // 0, 1, 2 = 1, 1.5, 2
	dcb.ByteSize  = m_com.dataBits;

	dcb.fOutxCtsFlow    = FALSE;
	//dcb.fOutxDsrFlow    = 0;
	dcb.fDtrControl     = 1;
	dcb.fTXContinueOnXoff = 0;
	dcb.fDsrSensitivity = 0;
	dcb.fRtsControl     = 1;
	dcb.fOutX           = FALSE;
	dcb.fInX            = FALSE;

	/* ----------------- misc parameters ----- */
//	dcb.fErrorChar      = 0;
	dcb.fBinary         = 0;
	dcb.fNull           = 0;
	//dcb.fAbortOnError   = 0;
	//dcb.wReserved       = 0;
	dcb.XonLim          = 0;
	dcb.XoffLim         = 0;
	dcb.XonChar         = 0x11;
	dcb.XoffChar        = 0x13;
	//dcb.EvtChar         = 0;
	if ( SetCommState(m_com.comHandle, &dcb)) 
	{
		m_bIsOpen = true;
		return true;
	}
	else
	{ 
		m_errorCode = emET_UNKNOW;
		return false;
	}
	
}


//
// Close 函数
//

void CSuperSerialPort::Close()
{
	CloseHandle(m_com.comHandle);
	SetEvent(m_hEvent[emTHREAD_EXIT]);
	ResetEvent(m_hEvent[emTHREAD_RUN]);
	m_bIsOpen = false;
	m_com.comHandle = INVALID_HANDLE_VALUE;

//	m_ReadCount  = 0;
//	m_WriteCount = 0;
}

void CSuperSerialPort::ClearSendBytes()
{
	m_WriteCount = 0;
}

void CSuperSerialPort::ClearReceiveBytes()
{
	m_ReadCount = 0;
}
//
// Attach 函数
//
// 参数: CWnd* pwin  窗口指针
//       DWORD msg   窗口消息
//
// 返回值: 返回一个附加到串口窗口列表的位置
//
// 注意: 窗口的附加Id(2^32)会用完,会导致程序崩溃
//
UINT CSuperSerialPort::Attach(CWnd* pwin, DWORD msg)
{
	if (NULL == pwin)
	{
		return 0;
	}

	WIN_ARG wa;
	wa.winId = m_nextWinId++;
	wa.msg   = msg;
	wa.pWin  = pwin;
	m_winList.push_back(wa);

	return wa.winId;
}


//
// Detach 函数
// 
// 参数: UINT winId  窗口附加时的Id
//

void CSuperSerialPort::Detach(UINT winId)
{
	if (winId == 0 || winId >= m_nextWinId)
	{
		return;
	}

	vWINLIST::iterator it;
	for(it = m_winList.begin();
	    it != m_winList.end();
		it++)
	{
		if (it->winId == winId)
		{
			m_winList.erase(it);
			break;
		}
	}
}



//
// SendData 函数
// 
// 参数: pPACKAGE pPack 要发送的数据包
//
// 返回值: true 发送成, false 发送失败
//

BOOL CSuperSerialPort::SendData(pPACKAGE pPack)
{
	DWORD dwErrFlag;
	COMSTAT comStat;
	m_errorCode = emET_SUCCEDED;

	if (m_com.comHandle == INVALID_HANDLE_VALUE)
	{
		m_errorCode = emET_NO_HANDLE;
		return false;
	}

	ClearCommError(m_com.comHandle, 
		&dwErrFlag,
		&comStat);

    BOOL bl;
	UINT nums = 0;
	UINT reserve = 0;
	
	// 数据过大
    if (pPack->iLen > pPack->emMAX_SIZE)
	{
		nums = pPack->iLen/pPack->emMAX_SIZE;
		reserve = pPack->iLen%pPack->emMAX_SIZE;
    
	
		// 分解传输
		for (UINT i= 0; i<nums; i++)
		{
			PACKAGE pk;
			pk.iLen = pk.emMAX_SIZE;
			memcpy(pk.pData, 
				pPack->pData+i*pPack->emMAX_SIZE,
				pPack->emMAX_SIZE*sizeof(BYTE));

			bl = WriteComm(&pk);
		}

		// 如果还有剩余的数据
		if (reserve != 0)
		{
			PACKAGE pk;
			pk.iLen = reserve;
			memcpy(pk.pData,
				pPack->pData+nums*pPack->emMAX_SIZE,
				pPack->emMAX_SIZE*sizeof(BYTE));
			bl = WriteComm(&pk);
		}
	}
	else
	{
		bl = WriteComm(pPack);
	}
	
	
	return bl;
} 

BOOL CSuperSerialPort::WriteComm(pPACKAGE pPack)
{
	DWORD wV;
    BOOL bl;

	bl = WriteFile(m_com.comHandle,
		pPack->pData,
		pPack->iLen,
		&pPack->iLen,//wV, 
		&m_ovWriter);
	

	if (!bl)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			bl = GetOverlappedResult(m_com.comHandle,
				&m_ovWriter,
				&pPack->iLen,//wV,
				TRUE);
		}
	}
	
	EnterCriticalSection(&m_csWrite);
	m_WriteCount += pPack->iLen; // 统计
	LeaveCriticalSection(&m_csWrite);

	return bl;
}

//
// StartReceive 函数
//
// 参数: LPVOID arg 传入对象,主要包含串口对象
//
// 返回值:  true 启动成功, false 启动失败
//
// 注意: 传入的参数必须包含串口对象, 否则不能接收数据
//

BOOL CSuperSerialPort::StartReceive(LPVOID arg)
{
	m_errorCode = emET_SUCCEDED;

	if (NULL == arg)
	{
		m_errorCode = emET_NO_HANDLE;
		return false;
	}

	// 创建事件
	m_hEvent[emTHREAD_EXIT] = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEvent[emTHREAD_RUN]  = CreateEvent(NULL, TRUE, TRUE, NULL);
	
	m_hReader               = CreateEvent(NULL, TRUE, FALSE, NULL);
	ZeroMemory(&m_ovReader, sizeof(m_ovReader));
	m_ovReader.hEvent       = m_hReader;

	m_hWriter               = CreateEvent(NULL, TRUE, FALSE, NULL);
	ZeroMemory(&m_ovWriter, sizeof(m_ovWriter));
	m_ovWriter.hEvent       = m_hWriter;
	
	m_hWait                 = CreateEvent(NULL, TRUE, FALSE, NULL);
	ZeroMemory(&m_ovWaitEvent, sizeof(m_ovWaitEvent));
	m_ovWaitEvent.hEvent    = m_hWait;
	
	
	m_hReceiveThread = CreateThread(NULL,
		0,
		ReceiveThread,
		arg,
		0,
		&m_threadId);
	if (m_hReceiveThread != NULL)
	{
		SetThreadPriority(m_hReceiveThread,
			THREAD_PRIORITY_BELOW_NORMAL);
	    return true;
	}
	else
	{
		return false;
	}
}


//////////////////////////////////////////////////////////////////////////
// 类属性

COMSTAT* CSuperSerialPort::GetComState()
{
	return &m_comState;
}

HANDLE CSuperSerialPort::GetComHandle()
{
	return m_com.comHandle; 
}

void CSuperSerialPort::SetInOutBuffer(DWORD inBuf, DWORD outBuf)
{
	SetupComm(m_com.comHandle, inBuf, outBuf);
}

DWORD CSuperSerialPort::GetErrorCode()
{
	return m_errorCode;
}

ULONG CSuperSerialPort::GetWritedBytes()
{
	ULONG tmp;
    EnterCriticalSection(&m_csWrite);
	tmp = m_WriteCount;
    LeaveCriticalSection(&m_csWrite);

	return tmp;
}

ULONG CSuperSerialPort::GetReadedBytes()
{
	ULONG  tmp;
	EnterCriticalSection(&m_csRead);
	tmp = m_ReadCount;
	LeaveCriticalSection(&m_csRead);

	return tmp;
}

#ifdef VERSION1_1 // v1.1版本被编译

BOOL CSuperSerialPort::GetFileTransferFlag() const
{
	return m_bIsTransferFile;
}

void CSuperSerialPort::OpenFileTransfer(CWnd* pwin,
									    DWORD msg)
{
	m_bIsTransferFile = true;
	m_pFileWnd        = pwin;
	m_dwDoFileMsg     = msg;
}

void CSuperSerialPort::CloseFileTransfer()
{
	m_bIsTransferFile = false;
	m_pFileWnd        = NULL;
	m_dwDoFileMsg     = 0;
}

#endif

//////////////////////////////////////////////////////////////////////////

// 注意该函数
const TCHAR* CSuperSerialPort::FormatErrorCodeToString(DWORD code)
{
	switch(code)
	{
	case emET_SUCCEDED:
		sprintf(m_errorInfo, _T("操作成功!"));
		break;

	case emET_NO_OPEN:
		sprintf(m_errorInfo, _T("串口没打开或被其他程序打开"));
		break;

	case emET_NO_HANDLE:
		sprintf(m_errorInfo, _T("串口没有句柄"));
		break;

	case emET_OPENED:
		sprintf(m_errorInfo, _T("串口已经打开过"));
		break;

	case emET_COM_UNNAMED:
		sprintf(m_errorInfo, _T("串口没命名"));
		break;
		
	default:
		sprintf(m_errorInfo,
			_T("未定义的错误,请调用系统的GetLastError来查找"));
		break;
	}

	return m_errorInfo;
}