#ifndef SW_DLL_B
#define SW_DLL_B

#ifdef _SW_DLL
#define SW_API __declspec(dllexport)
#else
#define SW_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif




#ifdef __cplusplus
};
#endif

#endif