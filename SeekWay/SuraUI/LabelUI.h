#ifndef __UILABEL_H__
#define __UILABEL_H__

#pragma once

class UILIB_API CLabelUI : public CControlUI
{
public:
    CLabelUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetTextStyle(UINT uStyle);
    void SetTextColor(DWORD dwTextColor);
    void SetDisabledTextColor(DWORD dwTextColor);
    void SetFont(int index);
    RECT GetTextPadding() const;
    void SetTextPadding(RECT rc);
    bool IsShowHtml();
    void SetShowHtml(bool bShowHtml = true);

    SIZE EstimateSize(SIZE szAvailable);
    void Event(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void PaintText(HDC hDC);

protected:
    DWORD m_dwTextColor;
    DWORD m_dwDisabledTextColor;
    int m_iFont;
    UINT m_uTextStyle;
    RECT m_rcTextPadding;
    bool m_bShowHtml;
};

#endif // __UICOMMONCONTROLS_H__

