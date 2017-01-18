#include "YString.h"


unsigned int YString::_class_instance_num = 0;

YString::YString(void)
{
	++_class_instance_num;
}

YString::YString(const wchar_t *wc )
{
	++_class_instance_num;

	if (wc == 0)
	{
		mwc_char.clear();
		return;
	}

	mwc_char = wc;

}
YString::~YString(void)
{
	--_class_instance_num;
}
