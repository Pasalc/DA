// Lab_06.cpp : Определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "MathLib.h"

#define NOT_SAME_SIZE 1
using namespace std;
//Деление для отрицательных чисел
int64_t my_div(const int64_t& num, int64_t diver) {
	if ((num < 0) && (num % diver))
		return num / diver - 1;
	else//excessively
		return num / diver;
}
//Взятие по модулю для отрицательных чисел
int64_t my_mod(const int64_t& num, int64_t diver) {
	if ((num < 0) && (num % diver))
		return num % diver + diver;
	else//excessively
		return num % diver;//return 0;
}

int BigNumber::GetBASE() {
	return this->BASE;
}
//Конструктор, строку конвертирует в большое число
BigNumber::BigNumber(std::string str){
	int i = str.size() - BASE;
	_chunks = new vector<int64_t>();

	//Записываем с конца по BASE символов строки в чанки
	while(i >= BASE - 1) {
		_chunks->push_back(stoi(str.substr(i, BASE)));
		i -= BASE;
	}
	//Дошли до начала строки, тут ищем знак и записываем последнюю чанку
	
	// if (i>=0) {//i>-1
	_chunks->push_back(stoi(str.substr(0, i + BASE)));
}
//Конструктор без аргументов - "пустое" положительное число
BigNumber::BigNumber() {
	_chunks = new vector<int64_t>();
}
//Изменение размера массива с чанками
void BigNumber::Resize(const size_t& newSize) {
	_chunks->resize(newSize,0);
}
/*
* Функция нормализует большое число так, чтобы
* во всех чанках лежали BASE-разрядные числа
*/
void BigNumber::NormalizationChunks(size_t size, bool left) {
	int64_t over = 0; //"Лишнее", которое будет кочевать в следующие чанки
	vector<int64_t>& chunks = *_chunks;

	//size = size==0?chunks.size():size;	//??Do we Need it
	//Нормализовать все чанки
	if (size == 0) {						//??If yes then how this suppose to work??
		size = chunks.size();				//??And why we have this here
		if (size != 0) {
			for (size_t i = 0; i < size - 1; ++i) {
				chunks[i] += over; //Прибавляем привесок от прошлых чанок
				over = my_div(chunks[i], BASE10); //Считаем перевес в текущей чанке
				chunks[i] = my_mod(chunks[i], BASE10); //Выравниваем чанку по базе
			}
			//Прибавляем перевес к последней чанке
			chunks[size - 1] += over;
			//Обрабатываем перевес в последней чанке
			if (chunks[size - 1] / BASE10) {
				over = my_div(chunks[size - 1], BASE10);
				chunks[size - 1] = my_mod(chunks[size - 1], BASE10);
				chunks.push_back(over); //Создаем нову чанку с остатками
			}
		}
	}
	//Нормализовать size чанков(необходимые количество чанков должно быть выделено)
	else {
		if (!left) {
			for (size_t i = 0; i < size || over>0; ++i) {
				chunks[i] += over; //Прибавляем привесок от прошлых чанок
				over = my_div(chunks[i], BASE10); //Считаем перевес в текущей чанке
				chunks[i] = my_mod(chunks[i], BASE10); //Выравниваем чанку по базе
			}
		}
		else {
			for (size_t i = chunks.size() - size; i < chunks.size()-1; ++i) {
				chunks[i] += over; //Прибавляем привесок от прошлых чанок
				over = my_div(chunks[i], BASE10); //Считаем перевес в текущей чанке
				chunks[i] = my_mod(chunks[i], BASE10); //Выравниваем чанку по базе
			}
			chunks[chunks.size() - 1] += over;
		}
	}
	return;
}

//Проверяет числона правильность
bool BigNumber::HaveErrorCheck() {
	vector<int64_t>& chunks = *_chunks;
	//Если в последней чанке отрицательное число
	if (chunks.size() == 0 || chunks[chunks.size() - 1] < 0) {// chunks.size()==0||chunks[chunks.size() - 1] < 0
		error = true;
		return true;
	}
	else {
		error = false;
		return false;
	}
}

//Убирает незначащие нули
void BigNumber::ZeroRemover() {
	vector<int64_t>& chunks = *_chunks;
	//Убираем из числа нулевые чанки
	int i = chunks.size() - 1;
	while (chunks[i] == 0) {
		if (i == 0) {
			return; //true
		}
		chunks.pop_back();
		--i;
	}
	return;//true
}
//Количество десятичных разрядов
size_t BigNumber::ChunksBaseCount() {
	size_t count = BASE;
	for (uint64_t div = BASE10; count > 1;--count) {
		div /= 10;
		if ((_chunks->back() / div) > 0) {
			break;
		}
	}
	return count + (_chunks->size()-1)*BASE;
}
//Сдвиг по десятичным разрядам(влево или вправо)
void BigNumber::Shift(int64_t shift) {
	if (shift != 0) {
		uint64_t over = 0;
		std::vector<int64_t>& chunks = *_chunks;

		if (shift > 0) {
			for (uint64_t countChunk = 0; countChunk < chunks.size() - 1; ++countChunk) {
				chunks[countChunk] *= (int64_t)pow(10, shift);
				chunks[countChunk] += over;
				over = my_div(chunks[countChunk], BASE10);
				chunks[countChunk] = my_mod(chunks[countChunk], BASE10);
			}
			chunks[chunks.size() - 1] *= (int64_t)pow(10, shift);
			chunks[chunks.size() - 1] += over;
		}
		else {//right
			for (uint64_t countChunk = 0; countChunk < chunks.size(); ++countChunk) {
				chunks[countChunk] *= (int64_t)pow(10, BASE + shift);
				chunks[countChunk] += over;
				over = my_div(chunks[countChunk], BASE10);
				chunks[countChunk] = my_mod(chunks[countChunk], BASE10);
			}
			if (over != 0) { chunks.push_back(over); }
		}
	}
}
//Set radix of 'res' on 'pos' position to 'div' ('res' must be set on zero first)
void BigNumber::AddToRes(std::vector<int64_t>* res, const uint64_t& div, size_t& pos) {
	(*res)[pos / BASE] += div * (int64_t)pow(10,pos%BASE);
}
//Set radix of 'res' on 'pos' position to 'div' ('res' must be set on zero first)
void BigNumber::AddToRes(std::vector<int64_t>* res, const uint64_t& div, size_t&& pos) {
	(*res)[pos / BASE] += div * (int64_t)pow(10, pos%BASE);
}
//Find the most possible division
uint64_t BigNumber::MaxDiv(std::vector<int64_t>& num) {
	std::vector<int64_t>& chunks = *_chunks;
	size_t div = chunks.back() / num.back();
	for (int64_t end = chunks.size() - 1, numEnd = num.size() - 1; numEnd >= 0; --end, --numEnd) {
		chunks[end] -= num[numEnd] * div;
	}
	NormalizationChunks(num.size(), true);
	//poor program design & with 'while' instead 'if' kinda works(2000/250 wasn't working)
	//{
	while (chunks.back() < 0) {
		--div;
		for (int64_t end = chunks.size() - 1, numEnd = num.size() - 1; numEnd >= 0; --end, --numEnd) {
			chunks[end] += num[numEnd];
		}
		NormalizationChunks(num.size(), true);
	}
	//}
	return div;
}
//Compare with BigNumber
//return: 0 - equal; >0 - greater; <0 - lesser
int64_t BigNumber::cmp(BigNumber& a) {
	std::vector<int64_t>& chunks = *_chunks;
	std::vector<int64_t>& chunks2 = *a._chunks;
	//if(this->sign!=a.sign){
	//return this->sign;//sign=+-1
	//}
	if (chunks.size() != chunks2.size()) {
		throw NOT_SAME_SIZE;
	}
	size_t i = chunks.size()-1;
	while (i>0 && chunks[i] == chunks2[i]) {
		--i;
	}
	return chunks[i] - chunks2[i];
}
//Функция сложения
BigNumber BigNumber::Plus(BigNumber &num) {
	BigNumber res;
	for (size_t i = 0; i < this->_chunks->size(); i++) {
		res._chunks->push_back( (*(this->_chunks))[i] + (*(num._chunks))[i]);
	}
	return res;
}
//Функция вычитания
BigNumber BigNumber::Minus(BigNumber &num) {
	BigNumber res;
	for (size_t i = 0; i < this->_chunks->size(); i++) {
		res._chunks->push_back((*(this->_chunks))[i] - (*(num._chunks))[i]);
	}
	return res;
}
//Функция умножения
void BigNumber::Multiply(BigNumber &num, BigNumber &res) {
	std::vector<int64_t>& thisChunks = *_chunks;
	std::vector<int64_t>& numChunks = *(num._chunks);
	std::vector<int64_t>& resChunks = *(res._chunks);
	for (size_t numPos = 0; numPos< numChunks.size(); ++numPos) {
		for (size_t thisPos = 0; thisPos < thisChunks.size(); ++thisPos) {
			resChunks[thisPos+numPos] += thisChunks[thisPos]*numChunks[numPos];
			res.NormalizationChunks(thisChunks.size()+numPos);
		}
	}
}
//Функция деления
void BigNumber::Divide(BigNumber &num, BigNumber &res) {
	std::vector<int64_t>& resChunks = *res._chunks;
	std::vector<int64_t>& thisChunks = *this->_chunks;
	std::vector<int64_t>& numChunks = *num._chunks;

	if (num.isZero()) {
		resChunks.resize(0);
		return;
	}
	else {
		uint64_t division = 0;
		size_t thisSize = this->ChunksBaseCount();
		size_t numSize = num.ChunksBaseCount();
		if (thisSize >= numSize) {
			this->Shift((int64_t)(numSize - 1) % BASE - (thisSize - 1) % BASE);//Выравниваем по разрядам
			for (size_t toBeDone = thisSize - numSize; toBeDone > 0; --toBeDone) {
				division = MaxDiv(numChunks);
				this->Shift();//left
				AddToRes(&resChunks, division, toBeDone);
			}
			AddToRes(&resChunks, MaxDiv(numChunks), 0);//2000/900 breaks
		}
	}
}
//Оператор +, выполняет корректное сложение больших чисел
BigNumber BigNumber::operator + (BigNumber &num) {
	BigNumber res;
	//Приводим размер чанок обоих чисел
	if (this->_chunks->size() > num._chunks->size()) {
		num.Resize(_chunks->size());
	}
	else {
		this->Resize(num._chunks->size());
	}
	
	res = (*this).Plus(num);
	//Нормализуем результат
	res.NormalizationChunks();
	return res;
}
//Оператор -, выполняет корректное вычитание
BigNumber BigNumber::operator - (BigNumber &num) {
	BigNumber res;

	//Проверяем что первое число "не меньше" второго 
	if (this->_chunks->size() >= num._chunks->size()) {
		//Приводим размер чанок
		num.Resize(_chunks->size());
		res = this->Minus(num);
		//Нормализуем результат
		res.NormalizationChunks();
	}

	return res;
}

BigNumber BigNumber::operator * (BigNumber &num) {
	BigNumber res;
	//remove insignificant zeros
	this->ZeroRemover();
	num.ZeroRemover();
	//resize res, so every chunk could fit
	res.Resize(this->_chunks->size() + num._chunks->size());
	this->Multiply(num,res);
	
	return res;
}

BigNumber BigNumber::operator ^ (BigNumber &num) {
	BigNumber res("1");
	BigNumber zero("0");
	//remove insignificant zeros
	this->ZeroRemover();
	num.ZeroRemover();
	while (num > zero) {
		--num;
		res = res * (*this);
	}
	return res;
}

BigNumber& BigNumber::operator-- () {
	(*_chunks)[0] -= 1;
	NormalizationChunks(1);
	return *this;
}

BigNumber BigNumber::operator / (BigNumber &num) {
	BigNumber res;
	if (!num.isZero()) {
		this->ZeroRemover();
		num.ZeroRemover();
		if (this->_chunks->size() < num._chunks->size()) {
			return BigNumber("0");
		}
		res.Resize(this->_chunks->size() - num._chunks->size() + 1);
		this->Divide(num, res);
	}
	return res;
}

bool BigNumber::operator == (BigNumber& num) {
	ZeroRemover(); num.ZeroRemover();
	if (this->_chunks->size() > num._chunks->size()) {
		num.Resize(_chunks->size());
	}
	else {
		this->Resize(num._chunks->size());
	}
	return cmp(num) == 0;
}

bool BigNumber::operator > (BigNumber& num) {
	ZeroRemover(); num.ZeroRemover();
	if (this->_chunks->size() > num._chunks->size()) {
		num.Resize(_chunks->size());
	}
	else {
		this->Resize(num._chunks->size());
	}
	return cmp(num) > 0;
}

bool BigNumber::operator < (BigNumber& num) {
	ZeroRemover(); num.ZeroRemover();
	if (this->_chunks->size() > num._chunks->size()) {
		num.Resize(_chunks->size());
	}
	else {
		this->Resize(num._chunks->size());
	}
	return cmp(num) < 0;
}

bool BigNumber::isZero() {
	ZeroRemover();
	return _chunks->size() == 1 && _chunks->at(0) == 0;
}

MATHLIBRARY_API std::ostream & operator << (std::ostream &os, BigNumber &num) {
	
	if (num.HaveErrorCheck()) {
		os << "Error";
	}
	else{
		num.ZeroRemover();
		os << (*(num._chunks))[num._chunks->size() - 1];
		for (int i = num._chunks->size() - 2; i >= 0; i--) {
			os << setw(num.GetBASE()) << setfill('0') << (*(num._chunks))[i];
		}
	}
	return os;
}

