
#ifdef SURAUI_EXPORTS
#define UILIB_API __declspec(dllexport)
#else
#define UILIB_API __declspec(dllimport)
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>

#include "UIBase.h"
#include "UIManager.h"
#include "UIRender.h"
#include "UIContainer.h"
#include "UIList.h"
#include "UICombo.h"
#include "UIComonControls.h"
#include "UIActiveX.h"

#include "UIMarkup.h"
#include "UIDlgBuilder.h"

