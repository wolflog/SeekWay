#ifndef __UISLIDER_H__
#define __UISLIDER_H__

#pragma once

class UILIB_API CSliderUI : public CProgressUI
{
public:
	CSliderUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetEnabled(bool bEnable = true);

	void SetThumbSize(SIZE szXY);
	RECT GetThumbRect() const;
	LPCTSTR GetThumbImage() const;
	void SetThumbImage(LPCTSTR pStrImage);
	LPCTSTR GetThumbHotImage() const;
	void SetThumbHotImage(LPCTSTR pStrImage);
	LPCTSTR GetThumbPushedImage() const;
	void SetThumbPushedImage(LPCTSTR pStrImage);

	void Event(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintStatusImage(HDC hDC);

protected:
	SIZE m_szThumb;
	UINT m_uButtonState;

	CStdString m_sThumbImage;
	CStdString m_sThumbHotImage;
	CStdString m_sThumbPushedImage;

	CStdString m_sImageModify;
};


#endif 