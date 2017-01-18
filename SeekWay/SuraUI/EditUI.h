#ifndef __UIEDIT_H__
#define __UIEDIT_H__

#pragma once

class CEditWnd;

class UILIB_API CEditUI : public CLabelUI
{
	friend CEditWnd;
public:
	CEditUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	void SetEnabled(bool bEnable = true);
	void SetText(LPCTSTR pstrText);
	void SetMaxChar(UINT uMax);
	UINT GetMaxChar();
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly() const;
	void SetPasswordMode(bool bPasswordMode);
	bool IsPasswordMode() const;
	void SetPasswordChar(TCHAR cPasswordChar);
	TCHAR GetPasswordChar() const;

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);

	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
	SIZE EstimateSize(SIZE szAvailable);
	void Event(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);

protected:
	CEditWnd* m_pWindow;

	UINT m_uMaxChar;
	bool m_bReadOnly;
	bool m_bPasswordMode;
	TCHAR m_cPasswordChar;
	UINT m_uButtonState;
	CStdString m_sNormalImage;
	CStdString m_sHotImage;
	CStdString m_sFocusedImage;
	CStdString m_sDisabledImage;
};


#endif 