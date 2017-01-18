#ifndef __UIOPTION_H__
#define __UIOPTION_H__

#pragma once

class UILIB_API COptionUI : public CButtonUI
{
public:
	COptionUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool Activate();

	LPCTSTR GetSelectedImage();
	void SetSelectedImage(LPCTSTR pStrImage);

	bool IsGroup() const;
	void SetGroup(bool bGroup);
	bool IsSelected() const;
	void Selected(bool bSelected);

	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);

protected:
	bool m_bSelected;
	bool m_bGroup;

	CStdString m_sSelectedImage;
};


#endif 