// OperationSystemLab4Part2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>

std::mutex g_lock;

const int N = 100;

void threadFunctionWithoutLock(int &a)
{
	for (int i = 0; i < N; ++i) {
		std::cout << a++ << std::endl;
	}
}

void threadFunctionWithLock(int &a)
{
	for (int i = 0; i < N; ++i) {
		g_lock.lock();
		std::cout << a++ << std::endl;
		g_lock.unlock();
	}
}

int main()
{
	int a = 1;
	std::thread thr1(threadFunctionWithoutLock, std::ref(a));
	std::thread thr2(threadFunctionWithoutLock, std::ref(a));
	thr1.join();
	thr2.join();
	std::cout << a << std::endl;
	return 0;
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
