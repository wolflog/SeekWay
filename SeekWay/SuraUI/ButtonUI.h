#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#pragma once

class UILIB_API CButtonUI : public CLabelUI
{
public:
	CButtonUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	bool Activate();
	void SetEnabled(bool bEnable = true);
	void Event(TEventUI& event);

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetPushedImage();
	void SetPushedImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);

	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);

protected:
	UINT m_uButtonState;

	CStdString m_sNormalImage;
	CStdString m_sHotImage;
	CStdString m_sPushedImage;
	CStdString m_sFocusedImage;
	CStdString m_sDisabledImage;
};


#endif 