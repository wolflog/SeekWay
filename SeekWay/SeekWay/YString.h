#pragma once

#include <wchar.h>
#include "DeityList.h"

class YString
{
public:
	YString(void);
	YString(const wchar_t *wc);
	~YString(void);

	wstring mwc_char;

	// 类实例数量
	static unsigned int _class_instance_num;
};
