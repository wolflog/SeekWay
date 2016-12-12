#include <iostream>
#include <Windows.h>
using namespace std;

typedef int (*PFMAX)(int a,int b);

int main()
{
	HINSTANCE dll_test = LoadLibrary(L"IronClaw.dll");

	DWORD ret = GetLastError();

	cout << "last error " << ret << endl;

	PFMAX maxnum = (PFMAX)GetProcAddress(dll_test,"get_max");

	ret = GetLastError();

	cout << "last error " << ret << endl;

	int x = 2,y = 3;

	cout << maxnum(x,y) << endl;

	system("pause");
	return 0;
}