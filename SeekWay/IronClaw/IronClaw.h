#ifndef SW_DLL_A
#define SW_DLL_A

#ifdef _SW_DLL
#define SW_API __declspec(dllexport)
#else
#define SW_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

// export function
SW_API int GetMax(int a,int b);

#ifdef __cplusplus
}
#endif

#endif