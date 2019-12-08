#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdint.h>
#ifdef LAB06_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif

using namespace std;

extern "C"{
class MATHLIBRARY_API BigNumber {
private:
	vector<int64_t> *_chunks;
	bool error;
	//Настроиваемые параметры хранения большого числа
	//Количество символов в строке, которые станут одной "чанкой"
	static const int64_t BASE = 2;
	//Зависит от BASE (BASE10 = 10^BASE), используется при нормализации
	static const int64_t BASE10 = 100;
	//Служебные функции
	int64_t cmp(BigNumber &a);
	BigNumber Plus(BigNumber &a);
	BigNumber Minus(BigNumber &a);
	void Multiply(BigNumber &a, BigNumber& res);
	void Divide(BigNumber &a, BigNumber& res);
	void ZeroRemover();
	void NormalizationChunks(size_t size=0, bool left=false);
	void Resize(const size_t &newsize);
	bool isZero();
	
	size_t ChunksBaseCount();
	void Shift(int64_t shift=1);
	void AddToRes(std::vector<int64_t> *res, const uint64_t &div, size_t &pos);
	void AddToRes(std::vector<int64_t> *res, const uint64_t &div, size_t &&pos);
	uint64_t MaxDiv(std::vector<int64_t> &num);
public:
	bool HaveErrorCheck();
	BigNumber operator + (BigNumber &num);
	BigNumber operator - (BigNumber &num);
	BigNumber operator * (BigNumber &num);
	BigNumber operator / (BigNumber &num);
	BigNumber operator ^ (BigNumber &num);
	bool operator == (BigNumber &num);
	bool operator < (BigNumber &num);
	bool operator > (BigNumber &num);
	BigNumber& operator-- ();
	//Перегрузка оператора << для вывода в поток
	friend MATHLIBRARY_API std::ostream & operator << (std::ostream &os, BigNumber &num);

	int GetBASE();
	//Конструктор, строку конвертирует в большое число
	BigNumber(std::string str);
	//Конструктор без аргументов - "пустое" положительное число
	BigNumber();
};

} 