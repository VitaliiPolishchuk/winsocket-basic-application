// ReadMemory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <string>

DWORD pid;
DWORD scrap = 0x1871B4DC; //address
int myScrap; //value to read

int main()
{
	HWND hWnd = FindWindowA(0, ("FTL: Faster Than Light"));

	GetWindowThreadProcessId(hWnd, &pid);
	HANDLE pHAndle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
	ReadProcessMemory(pHAndle, (LPVOID)scrap, &myScrap, sizeof(myScrap), 0);
	std::cout << myScrap << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
