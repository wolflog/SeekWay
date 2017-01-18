#include "StdAfx.h"
#include "ProgressUI.h"



CProgressUI::CProgressUI() : m_bHorizontal(true), m_nMin(0), m_nMax(100), m_nValue(0)
{
	m_uTextStyle = DT_SINGLELINE | DT_CENTER;
	SetFixedHeight(12);
}

LPCTSTR CProgressUI::GetClass() const
{
	return _T("ProgressUI");
}

LPVOID CProgressUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("Progress")) == 0 ) return static_cast<CProgressUI*>(this);
	return CLabelUI::GetInterface(pstrName);
}

bool CProgressUI::IsHorizontal()
{
	return m_bHorizontal;
}

void CProgressUI::SetHorizontal(bool bHorizontal)
{
	if( m_bHorizontal == bHorizontal ) return;

	m_bHorizontal = bHorizontal;
	Invalidate();
}

int CProgressUI::GetMinValue() const
{
	return m_nMin;
}

void CProgressUI::SetMinValue(int nMin)
{
	m_nMin = nMin;
	Invalidate();
}

int CProgressUI::GetMaxValue() const
{
	return m_nMax;
}

void CProgressUI::SetMaxValue(int nMax)
{
	m_nMax = nMax;
	Invalidate();
}

int CProgressUI::GetValue() const
{
	return m_nValue;
}

void CProgressUI::SetValue(int nValue)
{
	m_nValue = nValue;
	Invalidate();
}

LPCTSTR CProgressUI::GetFgImage() const
{
	return m_sFgImage;
}

void CProgressUI::SetFgImage(LPCTSTR pStrImage)
{
	if( m_sFgImage == pStrImage ) return;

	m_sFgImage = pStrImage;
	Invalidate();
}

void CProgressUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("fgimage")) == 0 ) SetFgImage(pstrValue);
	else if( _tcscmp(pstrName, _T("hor")) == 0 ) SetHorizontal(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("min")) == 0 ) SetMinValue(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("max")) == 0 ) SetMaxValue(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("value")) == 0 ) SetValue(_ttoi(pstrValue));
	else CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CProgressUI::PaintStatusImage(HDC hDC)
{
	if( m_nMax <= m_nMin ) m_nMax = m_nMin + 1;
	if( m_nValue > m_nMax ) m_nValue = m_nMax;
	if( m_nValue < m_nMin ) m_nValue = m_nMin;

	RECT rc = {0};
	if( m_bHorizontal ) {
		rc.right = (m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left) / (m_nMax - m_nMin);
		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	}
	else {
		rc.top = (m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
		rc.right = m_rcItem.right - m_rcItem.left;
		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	}

	if( !m_sFgImage.IsEmpty() ) {
		m_sFgImageModify.Empty();
		m_sFgImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rc.left, rc.top, rc.right, rc.bottom);

		if( !DrawImage(hDC, (LPCTSTR)m_sFgImage, (LPCTSTR)m_sFgImageModify) ) m_sFgImage.Empty();
		else return;
	}

	DWORD dwBorderColor = 0xFF4EA0D1;
	int nBorderSize = 1;
	CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, dwBorderColor);
}
