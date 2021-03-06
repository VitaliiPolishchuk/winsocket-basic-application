// OperationgSystemLab4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <windows.h>
#include <mutex>

std::mutex g_lock;


static const long MATRIX_SIZE = 6;
static const int THREADS_NUMBER = 4;

struct Matrix {
	float ** elements;

	void initialize_zero() {
		elements = new float*[MATRIX_SIZE];
		for (int i = 0; i < MATRIX_SIZE; ++i) {
			elements[i] = new float[MATRIX_SIZE];
			for (int j = 0; j < MATRIX_SIZE; ++j) {
				elements[i][j] = 0.0f;
			}
		}
	}

	void initialize_random() {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(-1e2, 1e2);
		auto random = std::bind(dist, mt);
		elements = new float*[MATRIX_SIZE];
		for (int i = 0; i < MATRIX_SIZE; ++i) {
			elements[i] = new float[MATRIX_SIZE];
			for (int j = 0; j < MATRIX_SIZE; ++j) {
				elements[i][j] = round(random());
			}
		}
	}

	void initialize_cin() {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(-1e2, -1e2);
		auto random = std::bind(dist, mt);
		elements = new float*[MATRIX_SIZE];
		for (int i = 0; i < MATRIX_SIZE; ++i) {
			elements[i] = new float[MATRIX_SIZE];
			for (int j = 0; j < MATRIX_SIZE; ++j) {
				std::cout << i << " " << j << " ";
				std::cin >> elements[i][j];
			}
		}
	}

	void print() {
		std::cout << std::endl;
		for (int i = 0; i < MATRIX_SIZE; ++i) {
			std::cout << "|\t";

			for (int j = 0; j < MATRIX_SIZE; ++j) {
				std::cout << elements[i][j] << "\t";
			}
			std::cout << "|" << std::endl;
		}
	}

};

void multiply(Matrix& r, const Matrix& m1, const Matrix& m2);
void multithreading_execution(Matrix& r, const Matrix& m1, const Matrix& m2);
void multiply_threading(Matrix& result, const int thread_number, const Matrix& m1, const Matrix& m2);

int main() {
	Matrix m1, m2, r;
	m1.initialize_random();
	m2.initialize_random();
	r.initialize_zero();
	std::cout << "\nFirst Matrix" << std::endl;
	m1.print();
	std::cout << "\nSecond Matrix" << std::endl;
	m2.print();
	multithreading_execution(r, m1, m2);
	std::cout << "\nResult " << std::endl;
	r.print();
	Sleep(100000);
	std::cout << "End of program" << std::endl;
}


void multiply(Matrix& r, const Matrix& m1, const Matrix& m2) {
	for (int i = 0; i < MATRIX_SIZE; ++i) {
		for (int j = 0; j < MATRIX_SIZE; ++j) {
			float result = 0.0f;
			for (int k = 0; k < MATRIX_SIZE; ++k) {
				const float e1 = m1.elements[i][k];
				const float e2 = m2.elements[k][j];
				result += e1 * e2;
			}
			r.elements[i][j] = result;
		}
	}
}


void multiply_threading(Matrix& result, const int thread_number, const Matrix& m1, const Matrix& m2) {
	// Calculate workload
	const int n_elements = (MATRIX_SIZE * MATRIX_SIZE);
	const int n_operations = n_elements / THREADS_NUMBER;
	const int rest_operations = n_elements % THREADS_NUMBER;

	int start_op, end_op;

	if (thread_number == 0) {
		// First thread does more job
		start_op = n_operations * thread_number;
		end_op = (n_operations * (thread_number + 1)) + rest_operations;
	}
	else {
		start_op = n_operations * thread_number + rest_operations;
		end_op = (n_operations * (thread_number + 1)) + rest_operations;
	}

	for (int op = start_op; op < end_op; ++op) {
		const int row = op % MATRIX_SIZE;
		const int col = op / MATRIX_SIZE;
		float r = 0.0f;
		for (int i = 0; i < MATRIX_SIZE; ++i) {
			const float e1 = m1.elements[row][i];
			const float e2 = m2.elements[i][col];
			r += e1 * e2;
		}

		result.elements[row][col] = r;
		g_lock.lock();
		std::cout << "row : " << row << " \tcolumn : " << col << "\tresult : " << result.elements[row][col] << "\t Thread number" << thread_number << std::endl;
		g_lock.unlock();
	}
}

void multithreading_execution(Matrix& r, const Matrix& m1, const Matrix& m2) {
	std::thread threads[THREADS_NUMBER];

	for (int i = 0; i < THREADS_NUMBER; ++i) {
		//std::cout << "Starting thread " << i << std::endl;
		threads[i] = std::thread(multiply_threading, std::ref(r), i, std::ref(m1), std::ref(m2));
	}

	//std::cout << "Calculating...." << std::endl;

	for (int i = 0; i < THREADS_NUMBER; ++i) {
		//std::cout << "Joining thread " << i << std::endl;
		threads[i].join();
	}
}