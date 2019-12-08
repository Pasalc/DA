// Lab5_SufTree.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "NodeArray.h"
#include "Node.h"
#include "Variables.h"


///Variables
std::string text;
uint64_t suffix = 0;

std::shared_ptr<uint64_t> textend(new uint64_t(0));
std::shared_ptr<NodeArray> root = nullptr;
NodeArray* lastAdded;
NodeArray* activeNode;
uint64_t leaf_pos = 1;
std::vector<uint64_t> index_of_all_suff;

uint64_t* count = (uint64_t*)malloc(sizeof(uint64_t) * 256);


void freeMem() {
	free(count);
	textend = nullptr;
	root = nullptr;
}

int main()
{
	std::getline(std::cin, text);
	text += TERMINAL_CHAR;

	root = std::shared_ptr<NodeArray>(new NodeArray());

	root->_sufLink = &(*root);
	lastAdded = &(*root);
	activeNode = &(*root);
	uint64_t len = 0;
	uint64_t remainder = 1;
	index_of_all_suff.reserve(text.length());//delete
	for (uint64_t textLen = 0; textLen < text.length(); ++textLen, ++(*textend)) {
		if (activeNode->InsertNA(len)) {
			do {
				if (--remainder == 0) {
					break;
				}
			} while (activeNode->InsertNA(len));


		}
		lastAdded = &(*root);
		++remainder;
	}

	(*textend) -= 2;
	uint64_t line(1);
	while (!std::cin.eof()) {
		root->PatternSearch(line);
		++line;
	}
	//Free
	freeMem();
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
