#pragma once

#include "afxwin.h"
// CMyButton �Ի���

class CMyButton : public CButton
{
	//DECLARE_DYNAMIC(CMyButton)
public:
	CMyButton();
	virtual ~CMyButton();
	//����Button Down�ı�����ɫ
	void SetDownColor(COLORREF color);
	//����Button Up�ı�����ɫ
	void SetUpColor(COLORREF color);
	BOOL Attach(const UINT nID, CWnd* pParent);
protected:
	//�������صĺ���
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
//	virtual BOOL OnEraseBkgnd(CDC* pDC);
public:
	//������ɫ�ֱ�Ϊ���֣�Button Down�ı�����ɫ��Button Up�ı�����ɫ
	COLORREF m_TextColor, m_DownColor,m_UpColor;
};
