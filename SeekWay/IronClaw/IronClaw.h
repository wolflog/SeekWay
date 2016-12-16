#ifndef SW_DLL_A
#define SW_DLL_A

//*************************
//			Ìú×¦
//*************************


#ifdef _SW_DLL
#define SW_API __declspec(dllexport)
#else
#define SW_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif


SW_API int get_max(int a,int b);

#ifdef __cplusplus
}
#endif

#endif