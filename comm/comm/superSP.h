// 
//  ������ͷ�ļ� 
//  ��Ȩ (C) 2006 LIBIN
//  ��������Ȩ
// 
//  ע������:
//  ���ļ�������һ������ͨѶ����, ���������win32����ϵͳ��,
//  ����������ⲿ����ͨ���Զ��崰����Ϣʵ��;�������ʵ����
//  ��һ������ര�巢�����ݵ�����.
//
//  �汾��ϵ:
//  2006-6  V1.0    ʵ�ֽ��������շ������帽�ӵȹ���
//  2006-7  V1.0.1  �Ľ�������I/O�������ڷ�æ���´����¼�����Ӧ������
//
//  ����:
//  �ô�����������޸ĺʹ�����ԭ���߲��е�һ�����Ρ��������ⷢ���ʼ���
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


typedef vector<WIN_ARG>   vWINLIST; // �����б�





// �������ݰ�
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
	BOOL SendData(pPACKAGE pPack); // ��������
	BOOL StartReceive(LPVOID arg); // ��������, ����Ϊ����ָ��

	//
	COMSTAT* GetComState(); // ��ȡ����״̬
	HANDLE   GetComHandle();// ��ȡ���ھ��

	//
	ULONG    GetWritedBytes();
	ULONG    GetReadedBytes();

#ifdef VERSION1_1
	BOOL     GetFileTransferFlag() const;
	void     OpenFileTransfer(CWnd* pwnd, DWORD msg);
	void     CloseFileTransfer();
#endif	

	// �����������������
	void     SetInOutBuffer(DWORD inBuf, DWORD outBuf);

	// ��ȡ������Ϣ
	DWORD    GetErrorCode();
	const TCHAR*   FormatErrorCodeToString(DWORD code);

	
	static void SafeRelease(pPACKAGE pPack); // �ͷŰ�

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


	// ��������ö��
	enum emErrorType
	{
		emET_SUCCEDED = 0,
		emET_NO_OPEN,
		emET_NO_HANDLE,
		emET_OPENED,
		emET_COM_UNNAMED,
		emET_UNKNOW = 99
	};
	
	// ��������
	enum emDataType
	{
		emDATA_TYPE_TEXT=0, emDATA_TYPE_BINARY
	};

	// �߳��¼�����
	enum emThreadEvent
	{ 
		emTHREAD_EXIT = 0,
		emTHREAD_RUN = 1,
		emMAX_EVENT_NUMBER = 2 
	};
	
protected:
    
	// �����߳�
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
	BOOL     m_bIsTransferFile; // �����ļ���־
	CWnd*    m_pFileWnd;        // �ļ����մ���
	DWORD    m_dwDoFileMsg;     // �����ļ���Ϣ
#endif

	vWINLIST m_winList;

	/*------------------------------*/
	OVERLAPPED    m_ovReader;                          // Overlapped structure for ReadFile
    OVERLAPPED    m_ovWriter;                          // Overlapped structure for WriteFile
    OVERLAPPED    m_ovWaitEvent;                       // Overlapped structure for WaitCommEvent
	HANDLE        m_hWait;
	HANDLE        m_hReader;
	HANDLE        m_hWriter;
	
	
	UINT     m_nextWinId; // ���ظ��ӵĴ���Id

	ULONG    m_WriteCount; // �ѷ����ֽ���
	ULONG    m_ReadCount;  // �ѽ����ֽ���

	/*--------------------------------*/
	HANDLE   m_hEvent[emMAX_EVENT_NUMBER];    // �߳̿����¼����
	DWORD     m_threadId;
	CRITICAL_SECTION m_csRead; // ��д�ֽ�ͳ�Ƶ��ٽ���
	CRITICAL_SECTION m_csWrite;
	HANDLE   m_hReceiveThread;
};
#endif