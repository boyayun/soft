
// commDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "comm.h"
#include "commDlg.h"
#include "afxdialogex.h"

#include "superSP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CcommDlg �Ի���

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
Fun: ���У��
Input: data:У�����ݵ�ַ
	   len:У�����ݳ���
Output:NULL
Return:У���
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
Fun: �ۼӺ�У��
Input: data:У�����ݵ�ַ
	   len:У�����ݳ���
Output:NULL
Return:У���
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
// CcommDlg ��Ϣ�������
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
		AfxMessageBox(_T("һ������̫��"));
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

	// ���½��տؼ�

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
		m_handleresult = "����֡";
		m_cmdexplain = _T("ͨѶģ����յ���λ����ָ��������豸����֡�����豸���п���");
		break;

	case 0x05:
		m_handleresult = _T("ȷ��֡");
		m_cmdexplain = "��ʾ������ȷ���յ�����֡";
		break;

	case 0x06:
		m_handleresult = _T("�㱨����֡");
		m_cmdexplain = "�������߹رջ㱨���Լ�Ҫ��ر����ݵĿ���֡���ⲿ�ֹ��ܱ������豸�����ļ���ʵ�֣���\
�豸���յ���֡�Ժ�ִ�л㱨����ֹͣ�㱨�����Ҹ�������Ƶ�ʷ��ͻ㱨��Ϣ�����⣬��Щ�豸����С�ĵĻ㱨��������\
����Ƶ�ʵ���ֵС���豸��С��Ƶ�ʣ��������豸��СƵ��ִ�С����ҷ��ؿ����߼��������Ч֡";
		break;

	case 0x08:
		m_handleresult = _T("��ѯ�豸״̬");
		m_cmdexplain = "ͨѶģ������Ҫ������ȡ�豸״̬��ʱ���͵�����֡����֡��㱨���������̲�ͬ��������ֻ��\
��һ�Σ����豸�رջ㱨����û�л㱨���ܵ��豸��ʱ�򣬿���ʹ�ô���֡����";
		break;

	case 0x09:
		m_handleresult = _T("ֹͣ���ϱ���");
		break;

	case 0x10:
		m_handleresult = _T("��ȡ�豸ID");
		m_cmdexplain = "��λ���յ��豸�Ĺ��ϱ�����Ϣ��ͨ��ͨѶģ�����豸����ֹͣ����������";
		break;

	case 0x12:
		m_handleresult = _T("��ȡ�豸��Ϣ");
		m_cmdexplain = "�豸���յ���ȡģ����Ϣ֡�Ժ��豸������Ļ�����Ϣ���ظ�ͨѶģ��";
		break;

	case 0x15:
		m_handleresult = _T("���ؼ��ܲ���");
		m_cmdexplain = "";
		break;

	case 0x16:
		m_handleresult = _T("�����ʲ���֡");
		m_cmdexplain = "�豸��ͨѶֱ�ӵ�ͨѶ������ȱʡΪ9600bps�����ǿ��ǵ��ڸ�����豸���ܲ��������ϵ�ͨѶ\
Ƶ�ʣ����ģ����Ҫʹ����Ӧ�Ĳ�����ȷ���豸�Ĳ����ʣ�ģ�����ϵ��ʱ������ʹ��ȱʡ�����ʷ��Ͳ���֡�����û����\
����ȷ�Ļ�Ӧ���1200bps��ʼ�����Ӹı�ͨѶ�����ʣ�֪����ȷ�յ���Ӧ�����ò������̲�����ʹ�ã�������˳��Ϊ��\
1200bps��2400bps��4800bps��9600bps��19200bps��38400bps��115200bps";
		break;

	case 0x18:
		m_handleresult = _T("��·״̬����");
		m_cmdexplain = "������Щ�豸�Ĵ����������ʾ�豸�������Ҫ���豸����ʾ���ṩģ��ͨѶ���ӵ���Ϣʱ����\
Ҫ�豸��ѯͨѶģ�����·״̬";
		break;

	case 0x19:
		m_handleresult = _T("ǿ���豸��λ");
		m_cmdexplain = "ͨѶģ���ڻ�֪�豸������ɻ�����������¿������豸����ǿ�Ƹ�λ��ָ���ʹ�豸��ɸ�λ\
����������ָ�����ʹ�ã����豸һ���ڷ���ȷ��֮֡�󣬽��븴λģʽ";
		break;

	case 0x1A:
		m_handleresult = _T("��ȡ�豸�����ļ�");
		m_cmdexplain = "�������Ļ����ƶ������ն˿���ͨ��ͨѶģ���ȡ�������豸�ڲ����豸�����ļ��������Ѿ����豸������\
��ʵ�ֶ������豸�Ŀ��ơ���������״̬�£��������ĺ��ƶ������ն�Ҳ����ʹ��TypeID��ע���ʱ��ӷ������л�ø��豸���豸�����ļ�";
		break;

	case 0x30:
		m_handleresult = _T("�����ź�ǿ��");
		m_cmdexplain = "��������ʾ���ܵ��豸�������Ҫ��ʾ����ģ����ź�ǿ�ȵ���Ϣ�������ͨ����ָ������Ӧ����Ϣ";
		break;

	case 0x32:
		m_handleresult = _T("����ģʽӦ��");
		m_cmdexplain = "ͨѶģ����յ�����ģʽ����֡�Ժ�һ���ڽ�����Ӧ�Ĺ���ģʽ�Ժ󣬷���ͨѶģ�鹤��ģʽӦ��֡��\
����ͬʱ���в����ļ�鹦��";
		break;

	case 0x80:
		m_handleresult = _T("���������ļ�");
		m_cmdexplain = "�豸�����ļ�������Ҫд���豸�У��и�����豸�����Ҫ��д���豸�����ļ��������ʹ�ø�����֡";
		break;

	case 0x83:
		m_handleresult = _T("ͨѶģ����Ϣ����");
		m_cmdexplain = "ͨѶģ�����յ���ѯͨѶģ����Ϣ��ָ���Ժ󣬷��ظ���Ӧ�豸������ָ��֡";
		break;

	case 0xA0:
		m_handleresult = _T("�豸��������");
		m_cmdexplain = "������豸����Զ��������������������£�ģ����Ҫ�����յ������ݷ��͸���Ӧ���豸������豸�������������";
		break;

	case 0xA2:
		m_handleresult = _T("�����豸����У��");
		m_cmdexplain = "�ṩ�豸�������ݵ��豸���ڷ������������Ժ󣬽�����һ�������ļ���У�����ݰ���������\
���д��������ļ���MD5У���룬���������豸���յ�MD5У�����Ժ�����յ������ݽ���У�飬��ȷ�Ĳ��ܹ������豸������";
		break;

	default:
		m_handleresult = _T("��ʶ������");
		m_cmdexplain = "Э����û�а���������������Ӧ�÷����豸";
		break;
	}
}

void CcommDlg::Handflag(UCHAR flag, UCHAR cmd)
{
	switch(flag)
	{
	case 0:
		m_receivefeedback = _T("֡��ʽ����");
		break;

	case 1:
		m_receivefeedback = _T("֡���ܴ���");
		break;

	case 2:
		m_receivefeedback = _T("֡У�����");
		break;

	case 3:
		m_receivefeedback = _T("֡δ��������");
		break;

	case 4:
		m_receivefeedback = _T("����̫��");
		break;

	case 5:
		m_receivefeedback = _T("����̫��");
		break;

	case 6:
		m_receivefeedback = _T("֡������ȷ");
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
				m_encryptmode = _T("��������");
				//����
				UpdateData(FALSE);
				return;
			}
			else
			{
				m_encryptmode = _T("�Ǽ�������");
			}

			if (0 == (handarray[2]&0x60))
			{
				m_checkmode = _T("CRC16У��");
			}
			else if (0x20 == (handarray[2]&0x60))
			{
				m_checkmode = _T("�ۼӺ�У��");
			}
			else if (0x40 == (handarray[2]&0x60))
			{
				m_checkmode = _T("���У��");
			}
			else
			{
				m_checkmode = _T("����У�鷽ʽ");
				UpdateData(FALSE);
				return;
			}

			if (0 == (handarray[2]&0x03))
			{
				m_provions = _T("SHBPЭ��");
			}
			else
			{
				m_provions = _T("����Э��");
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
 У�����ʽ��0x8408
 ���ݷ���LSB����λ��ǰ��
 ��ʼֵ��0xFFFF
 ���ֵ��0xFFFF
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

	crc ^= data; /* �µ������뽫ԭ��������������crc����ӣ��ӷ������������� */

	/* �����ݵ�CRCУ���� */
	for (i=0; i<8; i++)
	{
		if (crc & 0x01) /* ���λΪ1,��ȥ���� */
		{
			crc >>= 1;
			crc ^= ccitt16;
		}
		else /* ���λΪ0,����Ҫ��ȥ���� */
		{
			crc >>= 1; /* ֱ����λ */
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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (!Init())
	{
		AfxMessageBox("��ʼ��ʧ��!");
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CcommDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CcommDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��ʼ��
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
	m_checkway.AddString(_T("���"));
	m_checkway.AddString(_T("�ۼӺ�"));

	DWORD pathLen = 256;
	TCHAR path[256] = {0};
	GetCurrentDirectory(pathLen, path);
	m_curPath.Format("%s\\", path);

	// ���ý����ı���Ĺ�����Χ
	m_receivedata.SetScrollRange(SB_VERT, 1, 100);

	UpdateData(FALSE);
	GetDlgItem(IDC_SENDCRC)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CHECKWAY)->EnableWindow(FALSE);

	//����ť�޸�ΪBS_OWNERDRAW���,����button�Ĳ����Ի�ģʽ
	//GetDlgItem(ID_OPEN)->ModifyStyle(0,BS_OWNERDRAW,0);
	//GetDlgItem(ID_SEND)->ModifyStyle(0,BS_OWNERDRAW,0);
	//GetDlgItem(ID_CLEARRECIVE)->ModifyStyle(0,BS_OWNERDRAW,0);
	//GetDlgItem(ID_CLEARSEND)->ModifyStyle(0,BS_OWNERDRAW,0);

	//�󶨿ؼ�IDC_BUTTON1����CMyButton����Ӧ���غ���DrawItem()
	//m_Btn1.Attach(ID_OPEN,this);
	//m_Btn2.Attach(ID_SEND,this);
	//m_Btn3.Attach(ID_CLEARRECIVE,this);
	//m_Btn4.Attach(ID_CLEARSEND,this);

	//����Button Down�ı���ɫ��SetDownColor()��SetUpnColor()��CMyButton���е���������
	//m_Btn1.SetDownColor(RGB(0,255,0));
	//m_Btn2.SetDownColor(RGB(255,0,0));
	//m_Btn3.SetDownColor(RGB(255,0,255));
	//m_Btn4.SetDownColor(RGB(0,0,0));

	//����Button Up�ı���ɫ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
//			else if ("�ۼӺ�" == m_sendcheckway)
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
//			else if ("���" == m_sendcheckway)
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

	// ���Ͱ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(!m_stat)		//�򿪴���
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
			GetDlgItem(ID_OPEN)->SetWindowText("�رմ���");
			m_color=RGB(0,255,0);
		}
		else // ��ʧ�ܣ���ȡ������Ϣ
		{
			ShowErrorInfo();
			return;
		}
	}
	else
	{
		m_stat = !m_stat;
		m_pSSP->Close();
		GetDlgItem(ID_OPEN)->SetWindowText("�򿪴���");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CcommDlg::OnBnClickedHexsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata.Empty();
	m_pSSP->ClearSendBytes();
	m_sendbytes = 0;
	UpdateData(FALSE);
}


void CcommDlg::OnBnClickedClearrecive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_receivedata.SetWindowText("");
	m_storeText.Empty();
	m_pSSP->ClearReceiveBytes();
	m_receivebytes = 0;
	UpdateData(FALSE);
}

void CcommDlg::OnBnClickedKeepsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	if (m_bKeepSend)
	{
		if (m_uKeepSendTime > 49)
		{
			SetTimer(emTIMER_SEND, m_uKeepSendTime, NULL);
		}
		else
		{
			AfxMessageBox(_T("��ʱ���͵�ʱ����̫С"));
		}
	}	
	else
	{
		KillTimer(emTIMER_SEND);
	}
}


void CcommDlg::OnBnClickedSendcrc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 02";
	m_sendcmd = "��ʾ�豸��ǰ״̬������֡������ͨ���������ж��豸�����Ƿ�ɹ������豸\
�յ�����֡����ִ�в����Ժ󷵻�״̬֡�����е��豸���յ���ѯ�豸״̬֡ʱҲ����״̬֡";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 03";
	m_sendcmd = "���豸���յ����ĸ�ʽ��ȷ���ǲ��ܽ���������֡ʱ���ߵ��豸���յ�\
�����ϼҵ�����߼�������֡ʱ���豸��ͨѶģ�鷵����Ч֡";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 04";
	m_sendcmd = "���б������ܵ��豸���ڲ�������������£����͵�����֡�����豸����\
״̬�仯ʱ��������ֱ����򱨾����ʱ����������������Ϣ�����ϱ���֡��ʽ������͸�ͨѶģ��";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 05";
	m_sendcmd = "��ʾ������ȷ���յ�����֡���豸��ͨѶģ�鶼���ܷ��͵�����֡";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 07";
	m_sendcmd = "�豸���ݡ����û㱨֡�������ݷ��ͻ㱨������֡";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 11";
	m_sendcmd = "�豸���ն��豸ID��ȡ֡�Ժ��豸��������豸ID��Ϣ���ظ�ͨѶģ�顣��ͨѶģ������һ���Ĵ���";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 13";
	m_sendcmd = "�豸���յ���ȡģ����Ϣ֡�Ժ��豸������Ļ�����Ϣ���ظ�ͨѶģ�飻";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 14";
	m_sendcmd = "";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 17";
	m_sendcmd = "������Щ�豸�Ĵ����������ʾ�豸�������Ҫ���豸����ʾ���ṩ\
ģ��ͨѶ���ӵ���Ϣʱ����Ҫ�豸��ѯͨѶģ�����·״̬";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 1B";
	m_sendcmd = "�豸�����ļ���һ���İ����������ֽڡ�ģ������ȷ���յ����ص��豸�����ļ���һ���Ժ�\
����ǰ������1�����ظ��豸�����ļ��������豸�������豸���յ����ذ��������ܰ�����ʱ����Ϊ������ɺ�\
���������豸�����ļ���һ���İ����������ء�";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 31";
	m_sendcmd = "�н������ܵ��豸����ͨ����ָ�����ͨѶģ��Ĺ���ģʽ�л���ͨѶģ��Ĺ���ģʽ�ֱ����\
���á������Ͳ������ֹ���ģʽ������ģʽ��ģ����Խ�����������á������Ȳ���������ģʽ�����������ӻ�����\
������Ժ�ģ�鴦����������ģʽ������ģʽ�����������ԵĹ����У�ģ����Խ������ģ�飬ֻ���в��Գ��򣬷����������Ч��";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 81";
	m_sendcmd = "�豸�����ļ���һ���İ����������ֽڡ�ģ������ȷ���յ����ص��豸�����ļ���һ���Ժ�\
����ǰ������1�����ظ��豸�����ļ��������豸�������豸���յ����ذ��������ܰ�����ʱ����Ϊ������ɺ�\
���������豸�����ļ���һ���İ�����������";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 82";
	m_sendcmd = "����ʾ���ܵ��豸���������Ļ��������豸���Բ�ѯģ��Ļ�����Ϣ";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 84";
	m_sendcmd = "ģ�����Ѿ��󶨻���ע����Ժ󣬻�洢һЩ��Ϣ���ڽ��յ���ʼ��ģ��������Ժ�\
���ָ���������״̬��ֻ��������ʹ�õĻ�����Ϣ";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 A1";
	m_sendcmd = "�豸��ȷ���յ����������Ժ󣬷��ظ�ģ�飬ģ���ٷ��ظ��ṩ���ݵ�ԭ�豸�����ļ���һ���İ�����\
�����ֽڡ�ģ������ȷ���յ����ص��������ݰ��Ժ󣬽���ǰ������1�����ظ��������ݵ�ԭ�豸�������豸���յ����ذ�\
�������ܰ�����ʱ����Ϊ������ɺ󣬷������������ļ���һ���İ�����������";
	ButtonSend();
}


void CcommDlg::OnBnClickedSend17()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_senddata = "F5 F5 00 03 A3";
	m_sendcmd = "�豸���յ�У�������Ժ󣬶Խ��յ��ļ�����У�飬���Ҹ���У��Ľ������Ӧ��֡��";
	ButtonSend();
}
