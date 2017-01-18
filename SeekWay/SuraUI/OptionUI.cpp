#include "StdAfx.h"
#include "OptionUI.h"



COptionUI::COptionUI() : m_bGroup(false), m_bSelected(false)
{
}

LPCTSTR COptionUI::GetClass() const
{
	return _T("OptionUI");
}

LPVOID COptionUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("Option")) == 0 ) return static_cast<COptionUI*>(this);
	return CButtonUI::GetInterface(pstrName);
}

bool COptionUI::IsGroup() const
{
	return m_bGroup;
}

void COptionUI::SetGroup(bool bGroup)
{
	m_bGroup = bGroup;
	if(m_bGroup && m_bSelected) {
		m_bSelected = false;
		Invalidate();
	}
}

bool COptionUI::IsSelected() const
{
	return m_bSelected;
}

void COptionUI::Selected(bool bSelected)
{
	if( m_bSelected == bSelected ) return;
	m_bSelected = bSelected;
	if( m_bSelected ) m_uButtonState |= UISTATE_SELECTED;
	else m_uButtonState &= ~UISTATE_SELECTED;

	if(m_bGroup) {
		if(m_bSelected)  {
			if( GetParent() ) {
				IContainerUI* pParent = (IContainerUI*)(GetParent()->GetInterface(_T("IContainer")));
				if(pParent) {
					for( int it = 0; it < pParent->GetCount(); it++ ) {
						COptionUI* pControl = static_cast<COptionUI*>(pParent->GetItemAt(it)->GetInterface(_T("Option")));
						if( pControl && pControl != this && pControl->IsGroup() && pControl->IsVisible() && pControl->IsEnabled() ) {
							pControl->Selected(false);
						}
					}
				}
			}

			if( m_pManager != NULL ) m_pManager->SendNotify(this, _T("selectchanged"));
		}
	}
	else {
		if( m_pManager != NULL ) m_pManager->SendNotify(this, _T("selectchanged"));
	}

	Invalidate();
}

bool COptionUI::Activate()
{
	if( !CControlUI::Activate() ) return false;
	if( m_bGroup ) Selected(true);
	else Selected(!m_bSelected);

	return true;
}

LPCTSTR COptionUI::GetSelectedImage()
{
	return m_sSelectedImage;
}

void COptionUI::SetSelectedImage(LPCTSTR pStrImage)
{
	m_sSelectedImage = pStrImage;
	Invalidate();
}

SIZE COptionUI::EstimateSize(SIZE szAvailable)
{
	if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo().tmHeight + 8);
	return CControlUI::EstimateSize(szAvailable);
}

void COptionUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("group")) == 0 ) SetGroup(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("selected")) == 0 ) Selected(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("selectedimage")) == 0 ) SetSelectedImage(pstrValue);
	else CButtonUI::SetAttribute(pstrName, pstrValue);
}

void COptionUI::PaintStatusImage(HDC hDC)
{
	m_uButtonState &= ~UISTATE_PUSHED;

	if( (m_uButtonState & UISTATE_SELECTED) != 0 ) {
		if( !m_sSelectedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sSelectedImage) ) m_sSelectedImage.Empty();
			else return;
		}
	}

	return CButtonUI::PaintStatusImage(hDC);
}
