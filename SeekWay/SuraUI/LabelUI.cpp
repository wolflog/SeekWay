#include "StdAfx.h"
#include "LabelUI.h"


CLabelUI::CLabelUI() : m_uTextStyle(DT_VCENTER), m_dwTextColor(0), 
    m_dwDisabledTextColor(0), m_iFont(-1), m_bShowHtml(false)
{
    ::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
}

LPCTSTR CLabelUI::GetClass() const
{
    return _T("LabelUI");
}

LPVOID CLabelUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("Label")) == 0 ) return static_cast<CLabelUI*>(this);
    return CControlUI::GetInterface(pstrName);
}

void CLabelUI::SetTextStyle(UINT uStyle)
{
    m_uTextStyle = uStyle;
    Invalidate();
}

void CLabelUI::SetTextColor(DWORD dwTextColor)
{
    m_dwTextColor = dwTextColor;
}

void CLabelUI::SetDisabledTextColor(DWORD dwTextColor)
{
    m_dwDisabledTextColor = dwTextColor;
}

void CLabelUI::SetFont(int index)
{
    m_iFont = index;
}

RECT CLabelUI::GetTextPadding() const
{
    return m_rcTextPadding;
}

void CLabelUI::SetTextPadding(RECT rc)
{
    m_rcTextPadding = rc;
    Invalidate();
}

bool CLabelUI::IsShowHtml()
{
    return m_bShowHtml;
}

void CLabelUI::SetShowHtml(bool bShowHtml)
{
    if( m_bShowHtml == bShowHtml ) return;

    m_bShowHtml = bShowHtml;
    Invalidate();
}

SIZE CLabelUI::EstimateSize(SIZE szAvailable)
{
    if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo().tmHeight + 4);
    return CControlUI::EstimateSize(szAvailable);
}

void CLabelUI::Event(TEventUI& event)
{
    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        m_bFocused = true;
        return;
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        m_bFocused = false;
        return;
    }
    if( event.Type == UIEVENT_MOUSEENTER )
    {
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE )
    {
        return;
    }
    CControlUI::Event(event);
}

void CLabelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("align")) == 0 ) {
        if( _tcsstr(pstrValue, _T("left")) != NULL ) {
            m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
            m_uTextStyle |= DT_LEFT;
        }
        if( _tcsstr(pstrValue, _T("center")) != NULL ) {
            m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
            m_uTextStyle |= DT_CENTER;
        }
        if( _tcsstr(pstrValue, _T("right")) != NULL ) {
            m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
            m_uTextStyle |= DT_RIGHT;
        }
    }
    else if( _tcscmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetTextColor(clrColor);
    }
    else if( _tcscmp(pstrName, _T("disabledtextcolor")) == 0 ) {
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetDisabledTextColor(clrColor);
    }
    else if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
        RECT rcTextPadding = { 0 };
        LPTSTR pstr = NULL;
        rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
        SetTextPadding(rcTextPadding);
    }
    else if( _tcscmp(pstrName, _T("showhtml")) == 0 ) SetShowHtml(_tcscmp(pstrValue, _T("true")) == 0);
    else CControlUI::SetAttribute(pstrName, pstrValue);
}

void CLabelUI::PaintText(HDC hDC)
{
    if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
    if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

    if( m_sText.IsEmpty() ) return;
    int nLinks = 0;
    RECT rc = m_rcItem;
    rc.left += m_rcTextPadding.left;
    rc.right -= m_rcTextPadding.right;
    rc.top += m_rcTextPadding.top;
    rc.bottom -= m_rcTextPadding.bottom;
    if( IsEnabled() ) {
        if( m_bShowHtml )
            CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
            NULL, NULL, nLinks, DT_SINGLELINE | m_uTextStyle);
        else
            CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
            m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
    else {
        if( m_bShowHtml )
            CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
            NULL, NULL, nLinks, DT_SINGLELINE | m_uTextStyle);
        else
            CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
            m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
}
