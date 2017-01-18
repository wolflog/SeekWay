#include "StdAfx.h"
#include "EditUI.h"

class CEditWnd : public CWindowWnd
{
public:
	CEditWnd();

	void Init(CEditUI* pOwner);

	LPCTSTR GetWindowClassName() const;
	LPCTSTR GetSuperClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	CEditUI* m_pOwner;
};

CEditWnd::CEditWnd() : m_pOwner(NULL)
{
}

void CEditWnd::Init(CEditUI* pOwner)
{
	CRect rcPos = pOwner->GetPos();
	RECT rcInset = pOwner->GetTextPadding();
	rcPos.left += rcInset.left;
	rcPos.top += rcInset.top;
	rcPos.right -= rcInset.right;
	rcPos.bottom -= rcInset.bottom;
	UINT uStyle = WS_CHILD | ES_AUTOHSCROLL;
	if( pOwner->IsPasswordMode() ) uStyle |= ES_PASSWORD;
	Create(pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
	SetWindowFont(m_hWnd, pOwner->GetManager()->GetDefaultFont(), TRUE);
	Edit_LimitText(m_hWnd, pOwner->GetMaxChar());
	if( pOwner->IsPasswordMode() ) Edit_SetPasswordChar(m_hWnd, pOwner->GetPasswordChar());
	Edit_SetText(m_hWnd, pOwner->GetText());
	Edit_SetModify(m_hWnd, FALSE);
	SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
	Edit_Enable(m_hWnd, pOwner->IsEnabled() == true);
	Edit_SetReadOnly(m_hWnd, pOwner->IsReadOnly() == true);
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	::SetFocus(m_hWnd);
	m_pOwner = pOwner;
}

LPCTSTR CEditWnd::GetWindowClassName() const
{
	return _T("EditWnd");
}

LPCTSTR CEditWnd::GetSuperClassName() const
{
	return WC_EDIT;
}

void CEditWnd::OnFinalMessage(HWND /*hWnd*/)
{
	// Clear reference and die
	m_pOwner->m_pWindow = NULL;
	delete this;
}

LRESULT CEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	if( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
	else if( uMsg == OCM_COMMAND && GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
	else bHandled = FALSE;
	if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	PostMessage(WM_CLOSE);
	return lRes;
}

LRESULT CEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( m_pOwner == NULL ) return 0;
	// Copy text back
	int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
	LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
	ASSERT(pstr);
	if( pstr == NULL ) return 0;
	::GetWindowText(m_hWnd, pstr, cchLen);
	m_pOwner->SetText(pstr);
	return 0;
}

/////////////////////////////////////////////////////////////
//////////////////

CEditUI::CEditUI() : m_pWindow(NULL), m_uMaxChar(255), m_bReadOnly(false), 
m_bPasswordMode(false), m_cPasswordChar(_T('*')), m_uButtonState(0)
{
	SetTextPadding(CRect(4, 4, 4, 4));
	SetBkColor(0xFFFFFFFF);
}

LPCTSTR CEditUI::GetClass() const
{
	return _T("EditUI");
}

LPVOID CEditUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("Edit")) == 0 ) return static_cast<CEditUI*>(this);
	return CLabelUI::GetInterface(pstrName);
}

UINT CEditUI::GetControlFlags() const
{
	if( !IsEnabled() ) return CControlUI::GetControlFlags();

	return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}

void CEditUI::Event(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->Event(event);
		else CLabelUI::Event(event);
		return;
	}

	if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		return;
	}
	if( event.Type == UIEVENT_WINDOWSIZE )
	{
		if( m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}
	if( event.Type == UIEVENT_SCROLLWHEEL )
	{
		if( m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}
	if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
	{
		if( m_pWindow ) return;
		m_pWindow = new CEditWnd();
		ASSERT(m_pWindow);
		m_pWindow->Init(this);
		Invalidate();
	}
	if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
	{
		Invalidate();
	}
	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK ) 
	{
		if( IsEnabled() ) {
			if( IsFocused() && m_pWindow == NULL )
			{
				m_pWindow = new CEditWnd();
				ASSERT(m_pWindow);
				m_pWindow->Init(this);
			}
			else if( m_pWindow != NULL )
			{
				POINT pt = event.ptMouse;
				pt.x -= m_rcItem.left + m_rcTextPadding.left;
				pt.y -= m_rcItem.top + m_rcTextPadding.top;
				::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
			}
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSEMOVE ) 
	{
		return;
	}
	if( event.Type == UIEVENT_BUTTONUP ) 
	{
		return;
	}
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	CLabelUI::Event(event);
}

void CEditUI::SetEnabled(bool bEnable)
{
	CControlUI::SetEnabled(bEnable);
	if( !IsEnabled() ) {
		m_uButtonState = 0;
	}
}

void CEditUI::SetText(LPCTSTR pstrText)
{
	m_sText = pstrText;
	if( m_pManager != NULL ) m_pManager->SendNotify(this, _T("textchanged"));
	Invalidate();
}

void CEditUI::SetMaxChar(UINT uMax)
{
	m_uMaxChar = uMax;
	if( m_pWindow != NULL ) Edit_LimitText(*m_pWindow, m_uMaxChar);
}

UINT CEditUI::GetMaxChar()
{
	return m_uMaxChar;
}

void CEditUI::SetReadOnly(bool bReadOnly)
{
	if( m_bReadOnly == bReadOnly ) return;

	m_bReadOnly = bReadOnly;
	if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
	Invalidate();
}

bool CEditUI::IsReadOnly() const
{
	return m_bReadOnly;
}

void CEditUI::SetPasswordMode(bool bPasswordMode)
{
	if( m_bPasswordMode == bPasswordMode ) return;
	m_bPasswordMode = bPasswordMode;
	Invalidate();
}

bool CEditUI::IsPasswordMode() const
{
	return m_bPasswordMode;
}

void CEditUI::SetPasswordChar(TCHAR cPasswordChar)
{
	if( m_cPasswordChar == cPasswordChar ) return;
	m_cPasswordChar = cPasswordChar;
	if( m_pWindow != NULL ) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
	Invalidate();
}

TCHAR CEditUI::GetPasswordChar() const
{
	return m_cPasswordChar;
}

LPCTSTR CEditUI::GetNormalImage()
{
	return m_sNormalImage;
}

void CEditUI::SetNormalImage(LPCTSTR pStrImage)
{
	m_sNormalImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditUI::GetHotImage()
{
	return m_sHotImage;
}

void CEditUI::SetHotImage(LPCTSTR pStrImage)
{
	m_sHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditUI::GetFocusedImage()
{
	return m_sFocusedImage;
}

void CEditUI::SetFocusedImage(LPCTSTR pStrImage)
{
	m_sFocusedImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditUI::GetDisabledImage()
{
	return m_sDisabledImage;
}

void CEditUI::SetDisabledImage(LPCTSTR pStrImage)
{
	m_sDisabledImage = pStrImage;
	Invalidate();
}

void CEditUI::SetVisible(bool bVisible)
{
	CControlUI::SetVisible(bVisible);
	if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
}

void CEditUI::SetInternVisible(bool bVisible)
{
	if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
}

SIZE CEditUI::EstimateSize(SIZE szAvailable)
{
	if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo().tmHeight + 6);
	return CControlUI::EstimateSize(szAvailable);
}

void CEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("readonly")) == 0 ) SetReadOnly(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("password")) == 0 ) SetPasswordMode(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
	else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
	else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
	else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
	else CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CEditUI::PaintStatusImage(HDC hDC)
{
	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
		if( !m_sDisabledImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
		if( !m_sFocusedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if( !m_sHotImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
			else return;
		}
	}

	if( !m_sNormalImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
		else return;
	}

	DWORD dwBorderColor = 0xFF4EA0D1;
	int nBorderSize = 1;
	if( (m_uButtonState & UISTATE_HOT) != 0 || (m_uButtonState & UISTATE_FOCUSED) != 0) {
		dwBorderColor = 0xFF85E4FF;
		nBorderSize = 2;
	}
	CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, dwBorderColor);
}

void CEditUI::PaintText(HDC hDC)
{
	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	if( m_sText.IsEmpty() ) return;

	CStdString sText = m_sText;
	if( m_bPasswordMode ) {
		sText.Empty();
		LPCTSTR p = m_sText.GetData();
		while( *p != _T('\0') ) {
			sText += m_cPasswordChar;
			p = ::CharNext(p);
		}
	}

	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;
	if( IsEnabled() ) {
		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, \
			m_iFont, DT_SINGLELINE | m_uTextStyle);
	}
	else {
		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
			m_iFont, DT_SINGLELINE | m_uTextStyle);
	}
}