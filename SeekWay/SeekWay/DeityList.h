#pragma  once

#include <string>
#include <vector>
using namespace std;

#ifdef _SW_DLL
#define SW_API __declspec(dllexport)
#else
#define SW_API __declspec(dllimport)
#endif






#ifndef IN
#define IN
#define OUT
#endif
