
// commDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "comm.h"
#include "commDlg.h"
#include "afxdialogex.h"

#include "superSP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcommDlg 对话框

USHORT CRC16_CCITT_TABLE[256] =
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
UCHAR CRC16_CCITT_TABLE8H[256]= 
{
	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

	//0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

	//0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

	//0x00, 0xC1, 0x81, 0x40
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
UCHAR CRC16_CCITT_TABLE8L[256]=
{
	/*0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,

	0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,

	0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,

	0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,

	0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,

	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,

	0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,

	0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,

	0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,

	0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,

	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,

	0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,

	0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,

	0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,

	0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,

	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,

	0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,

	0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,

	0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,

	0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,

	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,

	0x41, 0x81, 0x80, 0x40*/
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
CcommDlg::CcommDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcommDlg::IDD, pParent)
{	
	m_color = 0;
	m_encryptmode = _T("");
	m_checkmode = _T("");
	m_provions = _T("");
	m_receivefeedback = _T("");
	m_handleresult = _T("");
	m_cmdexplain = _T("");
	m_comName = _T("COM5");
	m_bps = _T("9600");
	m_checkbit = 0;
	m_databit = 0;
	m_stopbit = 0;
	m_receivebytes = 0;
	m_sendbytes = 0;
	m_senddata = _T("");
	m_uKeepSendTime = 50;

	m_hexdisplay = 0;
	m_hexsend = 0;
	m_sendcrc = 0;
	m_stat = 0;
	m_bKeepSend = FALSE;
	m_sendcheckway = _T("CRC16");

	memset(handbuf,0,1024);
	handlen = 0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_ENCRYPTMODE, m_encryptmode);
	DDX_Text(pDX, IDC_EDIT_CHECKMODE, m_checkmode);
	DDX_Text(pDX, IDC_EDIT_PROVIONS, m_provions);
	DDX_Text(pDX, IDC_EDIT_RECEIVEFEEDBACK, m_receivefeedback);
	DDX_Text(pDX, IDC_EDIT_HANDLERESULT, m_handleresult);
	DDX_Text(pDX, IDC_EDIT_CMDEXPLAIN, m_cmdexplain);
	DDX_Control(pDX, IDC_EDIT_UARTNUMBER, m_sPort);
	DDX_CBString(pDX, IDC_EDIT_UARTNUMBER, m_comName);
	DDX_Control(pDX, IDC_EDIT_BPS, m_sBps);
	DDX_CBString(pDX, IDC_EDIT_BPS, m_bps);
	DDX_Text(pDX, IDC_EDIT_CHEAKBIT, m_checkbit);
	DDX_Text(pDX, IDC_EDIT_DATABIT, m_databit);
	DDX_Text(pDX, IDC_EDIT_STOPBIT, m_stopbit);
	DDX_Text(pDX, IDC_EDIT_RECEIVEBYTES, m_receivebytes);
	DDX_Control(pDX, IDC_EDIT_RECEIVEDATA, m_receivedata);
	DDX_Text(pDX, IDC_EDIT_SENDBYTES, m_sendbytes);
	DDX_Text(pDX, IDC_EDIT_SENDDATA, m_senddata);
	DDX_Text(pDX, IDC_EDIT_KEEPSENDTIME, m_uKeepSendTime);
	DDX_Text(pDX, IDC_EDIT_SENDCMD, m_sendcmd);

	DDX_Control(pDX, IDC_EDIT_CHECKWAY, m_checkway);
	DDX_CBString(pDX, IDC_EDIT_CHECKWAY, m_sendcheckway);

	DDX_Check(pDX, IDC_HEXDISPLAY, m_hexdisplay);
	DDX_Check(pDX, IDC_HEXSEND, m_hexsend);
	DDX_Check(pDX, IDC_SENDCRC, m_sendcrc);
	DDX_Check(pDX, IDC_KEEPSEND, m_bKeepSend);
}

BEGIN_MESSAGE_MAP(CcommDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_SEND, &CcommDlg::OnOk)
	ON_BN_CLICKED(ID_OPEN, &CcommDlg::OnBnClickedOk4)
	ON_BN_CLICKED(IDC_HEXDISPLAY, &CcommDlg::OnBnClickedHexdisplay)
	ON_BN_CLICKED(IDC_HEXSEND, &CcommDlg::OnBnClickedHexsend)
	ON_BN_CLICKED(ID_CLEARSEND, &CcommDlg::OnBnClickedClearsend)
	ON_BN_CLICKED(ID_CLEARRECIVE, &CcommDlg::OnBnClickedClearrecive)
	ON_MESSAGE(WM_UM_RECEIVE_CHAR, OnReceiveChar)
	ON_BN_CLICKED(IDC_KEEPSEND, &CcommDlg::OnBnClickedKeepsend)
	ON_BN_CLICKED(IDC_SENDCRC, &CcommDlg::OnBnClickedSendcrc)
	ON_BN_CLICKED(ID_SEND_STATE, &CcommDlg::OnBnClickedSendState)
	ON_BN_CLICKED(ID_SEND4, &CcommDlg::OnBnClickedSend4)
	ON_BN_CLICKED(ID_SEND6, &CcommDlg::OnBnClickedSend6)
	ON_BN_CLICKED(ID_SEND8, &CcommDlg::OnBnClickedSend8)
	ON_BN_CLICKED(ID_SEND10, &CcommDlg::OnBnClickedSend10)
	ON_BN_CLICKED(ID_SEND12, &CcommDlg::OnBnClickedSend12)
	ON_BN_CLICKED(ID_SEND14, &CcommDlg::OnBnClickedSend14)
	ON_BN_CLICKED(ID_SEND16, &CcommDlg::OnBnClickedSend16)
	ON_BN_CLICKED(ID_SEND3, &CcommDlg::OnBnClickedSend3)
	ON_BN_CLICKED(ID_SEND5, &CcommDlg::OnBnClickedSend5)
	ON_BN_CLICKED(ID_SEND7, &CcommDlg::OnBnClickedSend7)
	ON_BN_CLICKED(ID_SEND9, &CcommDlg::OnBnClickedSend9)
	ON_BN_CLICKED(ID_SEND11, &CcommDlg::OnBnClickedSend11)
	ON_BN_CLICKED(ID_SEND13, &CcommDlg::OnBnClickedSend13)
	ON_BN_CLICKED(ID_SEND15, &CcommDlg::OnBnClickedSend15)
	ON_BN_CLICKED(ID_SEND17, &CcommDlg::OnBnClickedSend17)
END_MESSAGE_MAP()

/******************************************
Fun: 异或校验
Input: data:校验数据地址
	   len:校验数据长度
Output:NULL
Return:校验和
******************************************/
UCHAR XorCheck(UCHAR *data, UCHAR len)
{
	UCHAR checksum = 0;
	
	while(len--)
	{
		checksum ^= *(data++);
	}
	
	return checksum;
}

/******************************************
Fun: 累加和校验
Input: data:校验数据地址
	   len:校验数据长度
Output:NULL
Return:校验和
******************************************/
UCHAR SumCheck(UCHAR *data, UCHAR len)
{
	UCHAR checksum = 0;
	
	while(len--)
	{
		checksum += *(data++);
	}
	
	return checksum;
}
// CcommDlg 消息处理程序
LRESULT CcommDlg::OnReceiveChar(WPARAM pPackage, LPARAM lParam)
{
	UpdateData();

	pPACKAGE pPack = (pPACKAGE)pPackage;
	ASSERT(pPack);
	ASSERT(pPack->pData);

	CString str;

	UpdateData();

	if ((handlen+pPack->iLen)<1024)
	{
		memcpy(&handbuf[handlen], &pPack->pData[0], pPack->iLen);
		handlen += pPack->iLen;
		SetTimer(emTIMER_RECEIVE, 100, NULL);
	}
	else
	{
		AfxMessageBox(_T("一包数据太多"));
	}

	if (m_hexdisplay)
	{
		for (UINT i=0; i<pPack->iLen; i++)
		{
			char hex[4];
			if (pPack->pData[i] > 0x0f)
			{
				sprintf(hex, _T("%x "), pPack->pData[i]);
			}
			else
			{
				sprintf(hex, _T("0%x "), pPack->pData[i]);
			}

			str.Insert(i*3, hex);
		}

		str.MakeUpper();
	}
	else
	{
		for (UINT i=0; i<pPack->iLen; i++)
		{
			str.Insert(i,pPack->pData[i]);
		}
	}

	// 更新接收控件

	m_storeText += str;
	m_receivedata.SetWindowText(m_storeText);


	int len = m_storeText.GetLength();
	m_receivedata.SetScrollPos(SB_VERT, 100);

	pPack->Clear();
	delete pPack;

	m_receivebytes = m_pSSP->GetReadedBytes();
	UpdateData(FALSE);
	return 0;
}

void CcommDlg::HandleCmd(UCHAR cmd)
{
	switch(cmd)
	{
	case 0x01:
		m_handleresult = "控制帧";
		m_cmdexplain = _T("通讯模块接收到上位机的指令，产生的设备控制帧，对设备进行控制");
		break;

	case 0x05:
		m_handleresult = _T("确认帧");
		m_cmdexplain = "表示数据正确接收的数据帧";
		break;

	case 0x06:
		m_handleresult = _T("汇报配置帧");
		m_cmdexplain = "启动或者关闭汇报，以及要求回报内容的控制帧（这部分功能必须结合设备描述文件来实现），\
设备接收到该帧以后执行汇报或者停止汇报，并且根据配置频率发送汇报信息；另外，有些设备有最小的的汇报间隔，如果\
设置频率的数值小于设备最小的频率，将按照设备最小频率执行。并且返回控制逻辑错误的无效帧";
		break;

	case 0x08:
		m_handleresult = _T("查询设备状态");
		m_cmdexplain = "通讯模块在需要主动获取设备状态的时候发送的数据帧；该帧与汇报操作的流程不同，该流程只操\
作一次，在设备关闭汇报或者没有汇报功能的设备的时候，可以使用此类帧数据";
		break;

	case 0x09:
		m_handleresult = _T("停止故障报警");
		break;

	case 0x10:
		m_handleresult = _T("读取设备ID");
		m_cmdexplain = "上位机收到设备的故障报警信息后，通过通讯模块向设备发送停止报警的命令";
		break;

	case 0x12:
		m_handleresult = _T("读取设备信息");
		m_cmdexplain = "设备接收到读取模块信息帧以后，设备将自身的基本信息返回给通讯模块";
		break;

	case 0x15:
		m_handleresult = _T("返回加密参数");
		m_cmdexplain = "";
		break;

	case 0x16:
		m_handleresult = _T("波特率测试帧");
		m_cmdexplain = "设备与通讯直接的通讯波特率缺省为9600bps，但是考虑到在个别的设备可能不采用以上的通讯\
频率，因此模块需要使用相应的操作来确定设备的波特率；模块在上电的时候首先使用缺省波特率发送测试帧，如果没有收\
到正确的回应则从1200bps开始逐步增加改变通讯波特率，知道正确收到回应；（该操作流程不建议使用）波特率顺序为：\
1200bps、2400bps、4800bps、9600bps、19200bps、38400bps、115200bps";
		break;

	case 0x18:
		m_handleresult = _T("链路状态返回");
		m_cmdexplain = "对于有些设备的带有自身的显示设备，如果需要在设备的显示中提供模块通讯链接的信息时，需\
要设备查询通讯模块的链路状态";
		break;

	case 0x19:
		m_handleresult = _T("强制设备复位");
		m_cmdexplain = "通讯模块在获知设备升级完成或者特殊情况下可以向设备发送强制复位的指令，迫使设备完成复位\
操作；（该指令谨慎使用）。设备一定在发出确认帧之后，进入复位模式";
		break;

	case 0x1A:
		m_handleresult = _T("读取设备描述文件");
		m_cmdexplain = "控制中心或是移动控制终端可以通过通讯模块读取保存在设备内部的设备描述文件，并且已经改设备描述文\
件实现对智能设备的控制。在联网的状态下，控制中心和移动控制终端也可以使用TypeID在注册的时候从服务器中获得该设备的设备描述文件";
		break;

	case 0x30:
		m_handleresult = _T("无线信号强度");
		m_cmdexplain = "对于有显示功能的设备，如果需要显示无线模块的信号强度的信息，则可以通过该指令获得相应的信息";
		break;

	case 0x32:
		m_handleresult = _T("工作模式应答");
		m_cmdexplain = "通讯模块接收到工作模式设置帧以后，一定在进入相应的工作模式以后，返回通讯模块工作模式应答帧，\
这样同时具有操作的检查功能";
		break;

	case 0x80:
		m_handleresult = _T("加载描述文件");
		m_cmdexplain = "设备描述文件最终需要写在设备中，有个别的设备如果需要后写入设备描述文件的情况下使用该命令帧";
		break;

	case 0x83:
		m_handleresult = _T("通讯模块信息返回");
		m_cmdexplain = "通讯模块在收到查询通讯模块信息的指令以后，返回给相应设备的数据指令帧";
		break;

	case 0xA0:
		m_handleresult = _T("设备升级下载");
		m_cmdexplain = "个别的设备允许远程升级，再这样的情况下，模块需要将接收到的数据发送给相应的设备，完成设备的软件升级操作";
		break;

	case 0xA2:
		m_handleresult = _T("发送设备升级校验");
		m_cmdexplain = "提供设备升级数据的设备，在发送升级数据以后，将发送一个升级文件的校验数据包。该数据\
包中带有升级文件的MD5校验码，被升级的设备接收到MD5校验码以后，与接收到的数据进行校验，正确的才能够进行设备的升级";
		break;

	default:
		m_handleresult = _T("不识别命令");
		m_cmdexplain = "协议中没有包含的命令或者命令不应该发给设备";
		break;
	}
}

void CcommDlg::Handflag(UCHAR flag, UCHAR cmd)
{
	switch(flag)
	{
	case 0:
		m_receivefeedback = _T("帧格式错误");
		break;

	case 1:
		m_receivefeedback = _T("帧解密错误");
		break;

	case 2:
		m_receivefeedback = _T("帧校验错误");
		break;

	case 3:
		m_receivefeedback = _T("帧未接收完整");
		break;

	case 4:
		m_receivefeedback = _T("数据太长");
		break;

	case 5:
		m_receivefeedback = _T("数据太短");
		break;

	case 6:
		m_receivefeedback = _T("帧解析正确");
		HandleCmd(cmd);
		break;
	}
}

void CcommDlg::CmdHandle(BYTE *revdata, UINT len)
{
	UINT i;
	UCHAR flag = 0;
	BOOL wordflag = 0;
	UCHAR datastate = 0;
	UCHAR templen = 0;
	UCHAR tempbuffer[250] = {0};
	UCHAR handarray[150] = {0};

	m_encryptmode = _T("");
	m_checkmode = _T("");
	m_provions = _T("");
	m_receivefeedback = _T("");
	m_handleresult = _T("");
	m_cmdexplain = _T("");

	for (i=0; i<len; i++)
	{
		if (0 == datastate)
		{
			if((0xF5 == revdata[i])&&(0xF5 == revdata[i+1]))
			{
				i++;
				datastate = 1;
				tempbuffer[templen]=0xf5;
				templen++;
				tempbuffer[templen]=0xf5;
				templen++;
			}
		}
		else if (1 == datastate)
		{
			if (0 == wordflag)
			{
				if (0xf5 == revdata[i])
				{
					wordflag = 1;
				}
				else
				{
					tempbuffer[templen++] = revdata[i];
				}
			}
			else
			{
				wordflag = 0;
				if (0xf5 == revdata[i])
				{
					goto handdata;
				}
				else if (0x55 == revdata[i])
				{
					tempbuffer[templen++] = 0xf5;
					tempbuffer[templen++] = 0x55;
				}
				else
				{
					Handflag(flag, tempbuffer[4]);
					return;
				}
			}
		}
	}

	handdata:
	{
		UCHAR i=0, j=0;
	
		for(;i<4;i++)
		{
			handarray[i] = tempbuffer[i];
		}
		for(j=0; j<handarray[3]-2; j++,i++)
		{
			handarray[j+4] = tempbuffer[i];
			if(0xf5 == tempbuffer[i])
			{
				i++;
			}
		}
	
		handarray[j+4] = tempbuffer[i];
		if(0xf5 == tempbuffer[i])
		{
			handarray[j+5] = tempbuffer[i+2];
		}
		else
		{
			handarray[j+5] = tempbuffer[i+1];
		}


		if(0 != datastate)
		{
			if(handarray[2]&0x80)
			{
				m_encryptmode = _T("加密数据");
				//解密
				UpdateData(FALSE);
				return;
			}
			else
			{
				m_encryptmode = _T("非加密数据");
			}

			if (0 == (handarray[2]&0x60))
			{
				m_checkmode = _T("CRC16校验");
			}
			else if (0x20 == (handarray[2]&0x60))
			{
				m_checkmode = _T("累加和校验");
			}
			else if (0x40 == (handarray[2]&0x60))
			{
				m_checkmode = _T("异或校验");
			}
			else
			{
				m_checkmode = _T("错误校验方式");
				UpdateData(FALSE);
				return;
			}

			if (0 == (handarray[2]&0x03))
			{
				m_provions = _T("SHBP协议");
			}
			else
			{
				m_provions = _T("错误协议");
				UpdateData(FALSE);
				return;
			}

			if ((len-4) < handarray[3])
			{
				flag = 3;
			}
			else if(131 < handarray[3])
			{
				flag = 4;
			}
			else if(3 > handarray[3])
			{
				flag = 5;
			}
			else
			{
				if (0 == (handarray[2]&0x60))
				{
					USHORT crcvelue = 0;
					USHORT crctemp = 0;
					crcvelue = crc_calculate(tempbuffer,i);
					crctemp = handarray[j+4]<<8 | handarray[j+5];
					if (crctemp == crcvelue)
					{
						flag = 6;
					}
					else
					{
						flag = 2;
					}
				}
				else if (0x20 == (handarray[2]&0x60))
				{
					if (handarray[j+4] == SumCheck(&tempbuffer[3],(i-3)))
					{
						flag = 6;
					}
					else
					{
						flag = 2;
					}
				}
				else if (0x40 == (handarray[2]&0x60))
				{
					if (handarray[j+4] == XorCheck(&tempbuffer[3],(i-3)))
					{
						flag = 6;
					}
					else
					{
						flag = 2;
					}
				}
			}
		}
	}
	Handflag(flag, handarray[4]);
	UpdateData(FALSE);
}

/************************************************************************
 校验多项式：0x8408
 数据方向：LSB（低位在前）
 初始值：0xFFFF
 异或值：0xFFFF
/************************************************************************/
USHORT CcommDlg::crc_calculate(UCHAR *data, UCHAR len)
{
	UCHAR j;
	USHORT crc = 0xFFFF;

	while(len--)
	{
		crc ^= *(data++);
		for(j=0; j<8; j++)
		{
			if(crc&0x01)
			{
				crc = (crc>>1)^0x8408;
			}
			else
			{
				crc >>= 0x01;
			} 
		}		
	}
	return ~crc;
}

USHORT CcommDlg::crc_lookuplist(UCHAR *data, UCHAR len)
{
	UCHAR i;
	UCHAR c = 0;
	USHORT crc = 0xffff;

	UCHAR crc_h = 0xFF;
	UCHAR crc_l = 0xFF;
	UCHAR indexcrc;
	//for(i=0; i<len; i++)
	//{
	//	c = crc&0xff;
	//	crc >>= 8;
	//	crc ^= CRC16_CCITT_TABLE[data[i]^c];
	//}
	for(i=0; i<len; i++)
	{
		indexcrc = crc_l^data[i];
		crc_l = crc_h^CRC16_CCITT_TABLE8H[indexcrc];
		crc_h = CRC16_CCITT_TABLE8L[indexcrc];
	}
	crc = crc_h<<8 | crc_l;
	return ~crc;
}

USHORT crc16_ccitt(UCHAR data, USHORT crc)
{
	USHORT ccitt16 = 0x8408;

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

void CcommDlg::init_crc16_ccitt_table()
{
	int i;

	for (i=0; i<256; i++)
	{
		CRC16_CCITT_TABLE[i] = crc16_ccitt(i, 0);
	}

	for (UINT i=0; i<256; i++)
	{
		char hex[9];
		sprintf(hex, _T("0x%x, "), CRC16_CCITT_TABLE[i]);
		m_senddata.Insert(i*8, hex);
	}

	UpdateData(FALSE);
}

void CcommDlg::init_crc16_ccitt_table8h()
{
	int i;

	for (i=0; i<256; i++)
	{
		CRC16_CCITT_TABLE8H[i] = crc16_ccitt(i, 0)>>8;
		CRC16_CCITT_TABLE8L[i] = crc16_ccitt(i, 0)&0xff;
	}

	for (UINT i=0; i<256; i++)
	{
		char hex[7];
		static CString temp;
		sprintf(hex, _T("0x%x, "), CRC16_CCITT_TABLE8H[i]);
		m_senddata.Insert(i*6, hex);
		sprintf(hex, _T("0x%x, "), CRC16_CCITT_TABLE8L[i]);
		temp.Insert(i*6, hex);
		m_receivedata.SetWindowText(temp);
	}

	//for (UINT i=0; i<256; i++)
	//{
	//	char hex[7];
	//	sprintf(hex, _T("0x%x, "), CRC16_CCITT_TABLE8L[i]);
	//	m_senddata.Insert((256+i*6), hex);
	//}
	UpdateData(FALSE);
}
BOOL CcommDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (!Init())
	{
		AfxMessageBox("初始化失败!");
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcommDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (SC_CLOSE == nID)
		{
			KillTimer(emTIMER_UPDATA);
			KillTimer(emTIMER_SEND);
			KillTimer(emTIMER_RECEIVE);
		}
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CcommDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{

	case emTIMER_UPDATA:
		{
	//		m_color=RGB(0,255,0);
			CDC *dc = GetDC();
			CBrush *pBrush=new CBrush(0x0000ff00);
			CPen *pen= new CPen(PS_SOLID,1,0x0000ff00);
			dc->SelectObject(pen);
			dc->SelectObject(pBrush);
			dc->Ellipse(520,208,540,228);//OnOk();
		}
		break;

	case emTIMER_SEND:
		OnOk();
		break;

	case emTIMER_RECEIVE:
		CmdHandle(handbuf, handlen);
		handlen = 0;
		KillTimer(emTIMER_RECEIVE);
		break;

	//case emTIMER_TRANSFER_CAL:
	//	{
	//		UINT curRecByte = m_pSSP->GetReadedBytes();
	//		UINT iLen =  curRecByte - m_preRecByte;
	//		CString transferSpeed;
	//		transferSpeed.Format(_T("%.2f KB/S"), iLen/1024.0f);
	//		SetDlgItemText(IDC_STATIC_TRANSFER_SPEED, transferSpeed);
	//		m_preRecByte = curRecByte;
	//	}
	//	break;

	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcommDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	m_color=RGB(0,0,0);
	CDC *dc = GetDC();
	CBrush *pBrush=new CBrush(m_color);
	CPen *pen= new CPen(PS_SOLID,1,m_color);
	dc->SelectObject(pen);
	dc->SelectObject(pBrush);
	dc->Ellipse(520,208,540,228);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcommDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 初始化
BOOL CcommDlg::Init()
{
	m_pSSP = new CSuperSerialPort();
	m_pSSP->Attach(this, WM_UM_RECEIVE_CHAR);
	m_preRecByte = m_pSSP->GetReadedBytes();

	// Init com data
	m_sPort.AddString(_T("COM1"));
	m_sPort.AddString(_T("COM2"));
	m_sPort.AddString(_T("COM3"));
	m_sPort.AddString(_T("COM4"));
	m_sPort.AddString(_T("COM5"));

	m_sBps.AddString(_T("9600"));
	m_sBps.AddString(_T("19200"));
	m_sBps.AddString(_T("38400"));
	m_sBps.AddString(_T("57600"));
	m_sBps.AddString(_T("115200"));

	m_checkbit = 0;
	m_databit  = 8;
	m_stopbit  = 1;

	m_checkway.AddString(_T("CRC16"));
	m_checkway.AddString(_T("异或"));
	m_checkway.AddString(_T("累加和"));

	DWORD pathLen = 256;
	TCHAR path[256] = {0};
	GetCurrentDirectory(pathLen, path);
	m_curPath.Format("%s\\", path);

	// 设置接收文本框的滚动范围
	m_receivedata.SetScrollRange(SB_VERT, 1, 100);

	UpdateData(FALSE);
	GetDlgItem(IDC_SENDCRC)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CHECKWAY)->EnableWindow(FALSE);

	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	//GetDlgItem(ID_OPEN)->ModifyStyle(0,BS_OWNERDRAW,0);
	//GetDlgItem(ID_SEND)->ModifyStyle(0,BS_OWNERDRAW,0);
	//GetDlgItem(ID_CLEARRECIVE)->ModifyStyle(0,BS_OWNERDRAW,0);
	//GetDlgItem(ID_CLEARSEND)->ModifyStyle(0,BS_OWNERDRAW,0);

	//绑定控件IDC_BUTTON1与类CMyButton，响应重载函数DrawItem()
	//m_Btn1.Attach(ID_OPEN,this);
	//m_Btn2.Attach(ID_SEND,this);
	//m_Btn3.Attach(ID_CLEARRECIVE,this);
	//m_Btn4.Attach(ID_CLEARSEND,this);

	//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
	//m_Btn1.SetDownColor(RGB(0,255,0));
	//m_Btn2.SetDownColor(RGB(255,0,0));
	//m_Btn3.SetDownColor(RGB(255,0,255));
	//m_Btn4.SetDownColor(RGB(0,0,0));

	//设置Button Up的背景色
	m_Btn1.SetDownColor(RGB(0,255,0));
	m_Btn2.SetDownColor(RGB(0,255,0));
	m_Btn3.SetDownColor(RGB(0,255,0));
	m_Btn4.SetDownColor(RGB(0,255,0));

	return true;
}

UCHAR CcommDlg::AsiicTOHex(UCHAR datafirst, UCHAR datasecond)
{
	UCHAR tempret = 0;
	if((0x30 <= datafirst)&&(0x39 >= datafirst))
	{
		datafirst -= 0x30;
	}
	else if ((0x41 <= datafirst)&&(0x46 >= datafirst))
	{
		datafirst -= 0x41;
		datafirst += 10;
	}
	else if((0x61 <= datafirst)&&(0x66 >= datafirst))
	{
		datafirst -= 0x61;
		datafirst += 10;
	}

	if((0x30 <= datasecond)&&(0x39 >= datasecond))
	{
		datasecond -= 0x30;
	}
	else if ((0x41 <= datasecond)&&(0x46 >= datasecond))
	{
		datasecond -= 0x41;
		datasecond += 10;
	}
	else if((0x61 <= datasecond)&&(0x66 >= datasecond))
	{
		datasecond -= 0x61;
		datasecond += 10;
	}

	tempret = datafirst*16 + datasecond;
	return tempret;
}

void CcommDlg::OnOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData();

	PACKAGE pack;
	
	UCHAR tempchar[1024] = {0}; 
	UCHAR temphex[1024] = {0};
	strcpy((char *)tempchar, 
		m_senddata.GetBuffer(m_senddata.GetLength()));
	UINT len = m_senddata.GetLength();
	UINT i = 0;
	UCHAR temp1 = 0;
	UCHAR temp2 = 0;

	if(m_hexsend)
	{
		while(len)
		{
			temphex[i] = AsiicTOHex(tempchar[i*3],tempchar[i*3+1]);
			i++;
			if(2<len)
			{
				len -= 3;
			}
			else if(2 == len)
			{
				len -= 2;
			}
		}
		pack.iLen = i;
		memcpy((char *)pack.pData, temphex, pack.iLen);
		
		if (m_sendcrc)
		{
			UINT j,k;
			tempchar[0] = 0xf5;
			tempchar[1] = 0xf5;
			for (j=2, k=2; j<i; j++,k++)
			{
				if (temphex[j] == 0xf5)
				{
					tempchar[k] = 0xf5;
					k++;
					tempchar[k] = 0x55;
				}
				else
				{
					tempchar[k] = temphex[j];
				}
			}

			if (0 == (tempchar[2]&0x60))
//			if ("CRC16" == m_sendcheckway)
			{
				USHORT crcvalue = 0;
				//crcvalue = crc_calculate(temphex, i);
				crcvalue = crc_lookuplist(tempchar, k);
				tempchar[k++] = crcvalue>>8;
				if(0xf5 == tempchar[k-1])
				{
					tempchar[k++] = 0x55;
				}
				tempchar[k++] = crcvalue&0xff;
				if(0xf5 == tempchar[k-1])
				{
					tempchar[k++] = 0x55;
				}
			}
			else if (0x20 == (tempchar[2]&0x60))
//			else if ("累加和" == m_sendcheckway)
			{
				UCHAR crcvalue = 0;
				crcvalue = SumCheck(&tempchar[3], (k-3));
				tempchar[k++] = crcvalue;
				if(0xf5 == tempchar[k-1])
				{
					tempchar[k++] = 0x55;
				}
				k++;
			}
			else if (0x40 == (tempchar[2]&0x60))
//			else if ("异或" == m_sendcheckway)
			{
				UCHAR crcvalue = 0;
				crcvalue = XorCheck(&tempchar[3], (k-3));
				tempchar[k++] = crcvalue;
				if(0xf5 == tempchar[k-1])
				{
					tempchar[k++] = 0x55;
				}
				k++;
			}
			pack.iLen = k;
			memcpy((char *)pack.pData, tempchar, pack.iLen);
		}
	}
	else
	{
		pack.iLen = m_senddata.GetLength();
		strcpy((char *)pack.pData, 
		m_senddata.GetBuffer(m_senddata.GetLength()));
	}

	// 发送包
	if (m_pSSP->SendData(&pack))
	{
		pack.Clear();
	}
	else
	{
		ShowErrorInfo();
	}
	m_sendbytes = (UINT)m_pSSP->GetWritedBytes();
	UpdateData(FALSE);
}

void CcommDlg::ShowErrorInfo()
{
	CString errInfo;
	DWORD   dwErrCode = m_pSSP->GetErrorCode();
	errInfo.Format("%s", 
		m_pSSP->FormatErrorCodeToString(dwErrCode));
	AfxMessageBox(errInfo);	
}

void CcommDlg::OnBnClickedOk4()
{
	// TODO: 在此添加控件通知处理程序代码

	if(!m_stat)		//打开串口
	{
		UpdateData();
//		init_crc16_ccitt_table8h();
		if (m_pSSP->Open(m_comName.GetBuffer(m_comName.GetLength()),
			atoi(m_bps),
			m_checkbit,
			m_databit,
			m_stopbit))
		{
			if (FALSE == m_pSSP->StartReceive(m_pSSP))
			{
				ShowErrorInfo();
				return;
			}

			SetTimer(emTIMER_UPDATA, 500, NULL);
			m_stat = !m_stat;
			OnBnClickedKeepsend();
			GetDlgItem(ID_OPEN)->SetWindowText("关闭串口");
			m_color=RGB(0,255,0);
		}
		else // 打开失败，获取错误信息
		{
			ShowErrorInfo();
			return;
		}
	}
	else
	{
		m_stat = !m_stat;
		m_pSSP->Close();
		GetDlgItem(ID_OPEN)->SetWindowText("打开串口");
		KillTimer(emTIMER_UPDATA);
		KillTimer(emTIMER_SEND);
		KillTimer(emTIMER_RECEIVE);
		m_color=RGB(0,0,0);
	}
	CDC *dc = GetDC();
	CBrush *pBrush=new CBrush(m_color);
	CPen *pen= new CPen(PS_SOLID,1,m_color);
	dc->SelectObject(pen);
	dc->SelectObject(pBrush);
	dc->Ellipse(520,208,540,228);
}

void CcommDlg::OnBnClickedHexdisplay()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
}


void CcommDlg::OnBnClickedHexsend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_hexsend)
	{
		GetDlgItem(IDC_SENDCRC)->EnableWindow(TRUE);
		//if (m_sendcrc)
		//{
		//	GetDlgItem(IDC_EDIT_CHECKWAY)->EnableWindow(TRUE);
		//}
	}
	else
	{
		GetDlgItem(IDC_SENDCRC)->EnableWindow(FALSE);
//		GetDlgItem(IDC_EDIT_CHECKWAY)->EnableWindow(FALSE);
	}
}

void CcommDlg::OnBnClickedClearsend()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata.Empty();
	m_pSSP->ClearSendBytes();
	m_sendbytes = 0;
	UpdateData(FALSE);
}


void CcommDlg::OnBnClickedClearrecive()
{
	// TODO: 在此添加控件通知处理程序代码
	m_receivedata.SetWindowText("");
	m_storeText.Empty();
	m_pSSP->ClearReceiveBytes();
	m_receivebytes = 0;
	UpdateData(FALSE);
}

void CcommDlg::OnBnClickedKeepsend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	if (m_bKeepSend)
	{
		if (m_uKeepSendTime > 49)
		{
			SetTimer(emTIMER_SEND, m_uKeepSendTime, NULL);
		}
		else
		{
			AfxMessageBox(_T("定时发送的时间间隔太小"));
		}
	}	
	else
	{
		KillTimer(emTIMER_SEND);
	}
}


void CcommDlg::OnBnClickedSendcrc()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	//if (m_sendcrc)
	//{
	//	GetDlgItem(IDC_EDIT_CHECKWAY)->EnableWindow(TRUE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_EDIT_CHECKWAY)->EnableWindow(FALSE);
	//}
}

void CcommDlg::ButtonSend()
{
	m_hexsend = 1;
	m_sendcrc = 1;
	GetDlgItem(IDC_SENDCRC)->EnableWindow();
	UpdateData(FALSE);
	OnOk();
}

void CcommDlg::OnBnClickedSendState()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 02";
	m_sendcmd = "显示设备当前状态的数据帧，可以通过此数据判断设备控制是否成功。当设备\
收到控制帧并且执行操作以后返回状态帧。还有当设备接收到查询设备状态帧时也返回状态帧";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 03";
	m_sendcmd = "当设备接收到报文格式正确但是不能解析的数据帧时或者当设备接收到\
不符合家电操作逻辑的数据帧时，设备向通讯模块返回无效帧";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 04";
	m_sendcmd = "具有报警功能的设备，在产生报警的情况下，发送的数据帧。当设备报警\
状态变化时（例如出现报警或报警解除时），立即将报警信息按故障报警帧格式打包发送给通讯模块";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend8()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 05";
	m_sendcmd = "表示数据正确接收的数据帧。设备和通讯模块都可能发送的数据帧";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend10()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 07";
	m_sendcmd = "设备根据“配置汇报帧”的内容发送汇报的数据帧";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend12()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 11";
	m_sendcmd = "设备接收都设备ID读取帧以后，设备将自身的设备ID信息返回给通讯模块。由通讯模块做进一步的处理";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend14()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 13";
	m_sendcmd = "设备接收到读取模块信息帧以后，设备将自身的基本信息返回给通讯模块；";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend16()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 14";
	m_sendcmd = "";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 17";
	m_sendcmd = "对于有些设备的带有自身的显示设备，如果需要在设备的显示中提供\
模块通讯链接的信息时，需要设备查询通讯模块的链路状态";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 1B";
	m_sendcmd = "设备描述文件下一包的包数：两个字节。模块在正确接收到下载的设备描述文件的一包以后，\
将当前包数加1，返回给设备描述文件的下载设备。下载设备接收到返回包数大于总包数的时候认为下载完成后，\
否则利用设备描述文件下一包的包数继续下载。";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 31";
	m_sendcmd = "有交互功能的设备可以通过该指令控制通讯模块的工作模式切换。通讯模块的工作模式分别包括\
设置、工作和测试三种工作模式，设置模式：模块可以进行网络的配置、组网等操作。工作模式：在网络链接或者组\
网完成以后，模块处于正常工作模式；测试模式：在生产测试的过程中，模块可以进入测试模块，只运行测试程序，方便提高生产效率";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend9()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 81";
	m_sendcmd = "设备描述文件下一包的包数：两个字节。模块在正确接收到下载的设备描述文件的一包以后，\
将当前包数加1，返回给设备描述文件的下载设备。下载设备接收到返回包数大于总包数的时候认为下载完成后，\
否则利用设备描述文件下一包的包数继续下载";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend11()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 82";
	m_sendcmd = "有显示功能的设备、控制中心或者生产设备可以查询模块的基本信息";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend13()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 84";
	m_sendcmd = "模块在已经绑定或者注册过以后，会存储一些信息。在接收到初始化模块的命令以后，\
将恢复到出厂的状态，只保留可以使用的基本信息";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend15()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 A1";
	m_sendcmd = "设备正确接收到升级数据以后，返回给模块，模块再返回给提供数据的原设备升级文件下一包的包数：\
两个字节。模块在正确接收到下载的升级数据包以后，将当前包数加1，返回给升级数据的原设备。下载设备接收到返回包\
数大于总包数的时候认为下载完成后，否则利用升级文件下一包的包数继续下载";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend17()
{
	// TODO: 在此添加控件通知处理程序代码
	m_senddata = "F5 F5 00 03 A3";
	m_sendcmd = "设备接收到校验数据以后，对接收的文件进行校验，并且根据校验的结果返回应答帧。";
	ButtonSend();
}
