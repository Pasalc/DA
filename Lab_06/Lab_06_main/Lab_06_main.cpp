// Lab_06_main.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include "MathLib.h"
#include <random>
#include <string>
#include <functional>
#include <assert.h>

int main()
{
	/*
	std::default_random_engine generator;
	std::uniform_int_distribution<uint64_t> distribution(0, _I32_MAX);
	auto num_roll=std::bind( distribution, generator);
	uint64_t first;
	uint64_t second;
	
	for (int i = 0; i < 10000; ++i) {
		first = num_roll();
		second = num_roll();
		BigNumber n1(std::to_string(first));
		BigNumber n2(std::to_string(second));
		assert(n1 + n2 == BigNumber(std::to_string(first + second)));
	}
	std::cout << "Sum is fine\n";
	for (int i = 0; i < 10000; ++i) {
		first = num_roll();
		second = num_roll();
		BigNumber n1(std::to_string(first));
		BigNumber n2(std::to_string(second));
		BigNumber n3 = n1-n2;
		assert(n1 > n2 == first > second);
		if (n1 > n2) {
			assert(n1 - n2 == BigNumber(std::to_string(first - second)));
		}
		else {
			assert(n3.HaveErrorCheck());
		}
	}
	std::cout << "Minus is fine\n";
	for (int i = 0; i < 10000; ++i) {
		first = num_roll();
		second = num_roll();
		BigNumber n1(std::to_string(first));
		BigNumber n2(std::to_string(second));
		assert(n1 * n2 == BigNumber(std::to_string(first * second)));
	}
	std::cout << "Multyplication is fine\n";
	for (int i = 0; i < 10000; ++i) {
		first = num_roll();
		second = num_roll();
		BigNumber n1(std::to_string(first));
		BigNumber n2(std::to_string(second));
		assert(n1 / n2 == BigNumber(std::to_string(first / second)));
	}
	std::cout << "Division is fine\n";
	for (int i = 0; i < 10000; ++i) {
		first = num_roll();
		second = num_roll();
		BigNumber n1(std::to_string(first));
		BigNumber n2(std::to_string(second));
		assert(n1 > n2 == (first > second));
		assert(n1 == n2 == (first == second));
		assert(n1 < n2 == (first < second));
	}
	std::cout << "Division is fine\n";*/

	while (std::cin) {

		std::string str1;
		std::string str2;
		std::string oper;
		std::getline(std::cin, str1);
		if (!std::cin) {
			break;
		}
		std::getline(std::cin, str2);
		std::getline(std::cin, oper);
		BigNumber n1(str1);
		BigNumber n2(str2);
		switch (oper.front())
		{
		case '+':
			std::cout << (n1 + n2) << "\n";
			break;
		case '-':
			std::cout << (n1 - n2) << "\n";
			break;
		case '*':
			std::cout << (n1 * n2) << "\n";
			break;
		case '/':
			std::cout << (n1 / n2) << "\n";
			break;
		case '^':
			std::cout << (n1 ^ n2) << "\n";
			break;
		case '=':
			std::cout << ((n1 == n2) ? "true" : "false") << "\n";
			break;
		case '>':
			std::cout << ((n1 > n2) ? "true" : "false") << "\n";
			break;
		case '<':
			std::cout << ((n1 < n2) ? "true" : "false") << "\n";
			break;
		}
	}
	return 0;
}

