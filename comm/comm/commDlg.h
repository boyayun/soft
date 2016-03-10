
// commDlg.h : 头文件
//
#include "mdctext.h"
#include  "CMyButton.h"
#pragma once

class CSuperSerialPort;

// USER MESSAGE
#define  WM_UM_RECEIVE_CHAR WM_USER+1


// CcommDlg 对话框
class CcommDlg : public CDialogEx
{
// 构造
public:
	CcommDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COMM_DIALOG };
	int m_color;

	CString m_encryptmode;
	CString m_checkmode;
	CString m_provions;
	CString m_receivefeedback;
	CString m_handleresult;
	CString m_cmdexplain;
	CComboBox m_sPort;
	CString m_comName;
	CComboBox m_sBps;
	CString m_bps;
	UINT m_checkbit;
	BYTE m_databit;
	UINT m_stopbit;
	UINT m_receivebytes;
	CMdcText m_receivedata;
	UINT m_sendbytes;
	CString m_senddata;
	UINT m_uKeepSendTime;
	CString m_sendcmd;

	BOOL m_hexdisplay;
	BOOL m_hexsend;
	BOOL m_sendcrc;
	BOOL m_stat;
	BOOL m_bKeepSend;
	CComboBox m_checkway;
	CString m_sendcheckway;

	CMyButton m_Btn1;
	CMyButton m_Btn2;
	CMyButton m_Btn3;
	CMyButton m_Btn4;

	UCHAR handbuf[1024];
	UINT handlen;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CSuperSerialPort* m_pSSP;
	CString m_storeText;

	enum emTIMER_TYPE 
	{
		emTIMER_SEND = 1, 
		emTIMER_RECEIVE = 2,
		emTIMER_USE_CAL = 3,
		emTIMER_TRANSFER_CAL = 4,
		emTIMER_UPDATA = 5
	};

	// 当前路径
	CString m_curPath;
	UINT    m_preRecByte;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnReceiveChar(WPARAM pPackage, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnOk();
	afx_msg void OnBnClickedOk4();
	afx_msg void OnBnClickedHexdisplay();
	afx_msg void OnBnClickedHexsend();

	BOOL Init();
	void ShowErrorInfo();
	UCHAR AsiicTOHex(UCHAR datafirst, UCHAR datasecond);
	void CmdHandle(BYTE *revdata, UINT len);
	void HandleCmd(UCHAR cmd);
	void Handflag(UCHAR flag, UCHAR cmd);
	USHORT crc_calculate(UCHAR *data, UCHAR len);
	USHORT crc_lookuplist(UCHAR *data, UCHAR len);
	void init_crc16_ccitt_table();
	void init_crc16_ccitt_table8h();
	void ButtonSend();
	afx_msg void OnBnClickedClearsend();
	afx_msg void OnBnClickedClearrecive();
	afx_msg void OnBnClickedKeepsend();
	afx_msg void OnBnClickedSendcrc();
//	afx_msg void OnBnClickedSendState();
	afx_msg void OnDoubleclickedSendState();
	afx_msg void OnBnClickedSendState();
	afx_msg void OnBnClickedSend4();
	afx_msg void OnBnClickedSend6();
	afx_msg void OnBnClickedSend8();
	afx_msg void OnBnClickedSend10();
	afx_msg void OnBnClickedSend12();
	afx_msg void OnBnClickedSend14();
	afx_msg void OnBnClickedSend16();
	afx_msg void OnBnClickedSend3();
	afx_msg void OnBnClickedSend5();
	afx_msg void OnBnClickedSend7();
	afx_msg void OnBnClickedSend9();
	afx_msg void OnBnClickedSend11();
	afx_msg void OnBnClickedSend13();
	afx_msg void OnBnClickedSend15();
	afx_msg void OnBnClickedSend17();
};
