#ifndef __UIPROGRESS_H__
#define __UIPROGRESS_H__

#pragma once

class UILIB_API CProgressUI : public CLabelUI
{
public:
	CProgressUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool IsHorizontal();
	void SetHorizontal(bool bHorizontal = true);
	int GetMinValue() const;
	void SetMinValue(int nMin);
	int GetMaxValue() const;
	void SetMaxValue(int nMax);
	int GetValue() const;
	void SetValue(int nValue);
	LPCTSTR GetFgImage() const;
	void SetFgImage(LPCTSTR pStrImage);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintStatusImage(HDC hDC);

protected:
	bool m_bHorizontal;
	int m_nMax;
	int m_nMin;
	int m_nValue;

	CStdString m_sFgImage;
	CStdString m_sFgImageModify;
};


#endif 