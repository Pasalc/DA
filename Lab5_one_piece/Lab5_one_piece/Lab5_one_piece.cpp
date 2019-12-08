// Lab5_SufTree.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include <iostream>
#include <assert.h>
#include <cstdint>
#include <ios>
#include <memory>
#include <string>
#include <vector>
#include <functional> 
#include <algorithm> 
#include <random>
#include <string.h>

#pragma region Defines

#define FIRST_CHAR 'a'
#define TERMINAL_CHAR '{'
#define FULL_BYTE 0xFF
#define BIT_IN_BYTE 8

#pragma endregion

#pragma region Variables
std::string text;
uint64_t suffix=0;
#pragma endregion

#pragma region RadixSort
void ByteCount(std::vector<uint64_t> key, uint64_t size, uint64_t* count, const uint8_t& ToCount) {
	memset(count, 0, sizeof(uint64_t) * 256);
	while (size > 0)
	{
		++count[(key[--size] >> ToCount * BIT_IN_BYTE) & FULL_BYTE];
	}
}

void Order(uint64_t* count) {
	for (int i = 254; i >= 0; --i) {
		count[i] += count[i + 1];
	}
}

void RadixSort(std::vector<uint64_t>& key, uint64_t size, uint64_t* count, const uint8_t& ToCount, std::vector<uint64_t>& key2) {
	int i;
	while (size > 0) {
		i = --count[(key[--size] >> ToCount * BIT_IN_BYTE) & FULL_BYTE];
		key2[i] = key[size];
	}
}


uint64_t* count = (uint64_t*)malloc(sizeof(uint64_t) * 256);//std::vector<uint64_t> count(256,0);

void RSort(std::vector<uint64_t>& index_of_pattern) {
	std::vector<uint64_t> index_for_radix_sort(index_of_pattern.size());
	for (uint8_t current_byte_count = 0; current_byte_count < sizeof(uint64_t); ++current_byte_count) {
		ByteCount(index_of_pattern, index_of_pattern.size(), count, current_byte_count);
		Order(count);
		RadixSort(index_of_pattern, index_of_pattern.size(), count, current_byte_count, index_for_radix_sort);
		index_of_pattern.swap(index_for_radix_sort);
	}
}

void Swap(uint64_t* first, uint64_t* second) {
	uint64_t* temp = first;
	first = second;
	second = temp;
}

#pragma endregion

#pragma region NodeArray.h
class Node;
class NodeArray;
constexpr auto ALPHABET_LENGTH = 27;
constexpr auto TEXT_END = TERMINAL_CHAR;
constexpr auto ROOT = -1;

class NodeArray
{
public:
	NodeArray* _sufLink;
	Node* _children[ALPHABET_LENGTH];
	//Constructors
	NodeArray();
	NodeArray(Node* child, char charOnPos); //Creates leaf

	//Methods
	bool InsertNA(uint64_t& activeLen);
	void InsertSplit(const uint64_t& activeLen);
	void PatternSearch(const uint64_t& line, uint64_t&& pattern_same_len = 0);
	bool CheckExist(std::string& substring, int64_t dist);
	void AddSuffixLink(NodeArray* sufLink);

	//Destructor
	virtual ~NodeArray();
};
#pragma endregion

#pragma region  Node.h
class Node
{
public:

	uint64_t _left;
	std::shared_ptr<uint64_t> _right;
	std::shared_ptr<NodeArray> _childs;//Shouldd be unique_ptr?
	uint64_t suff_pos;

	//Constructors
	Node();
	Node(const uint64_t& left, const std::shared_ptr<uint64_t>& right);	//Creates internal node
	Node(const uint64_t& left, const std::shared_ptr<uint64_t>& right, const std::shared_ptr<NodeArray>& children); //Creates internal node with _sufLink

	//Methods
	Node* Split(uint64_t& pos);
	uint64_t ArcLen();
	bool IsLeaf();
	void LeafPos(std::vector<uint64_t>& index);
	void DisplayPos(const uint64_t& line);

	//Destructor
	virtual ~Node();

};
#pragma endregion

std::shared_ptr<uint64_t> textend(new uint64_t(0));
std::shared_ptr<NodeArray> root = nullptr;
NodeArray* lastAdded;
NodeArray* activeNode;
uint64_t leaf_pos = 1;
std::vector<uint64_t> index_of_all_suff;

#pragma region NodeArray.cpp

NodeArray::NodeArray()
{
	_sufLink = &(*root);
	for (int i = 0; i < ALPHABET_LENGTH; i++) {
		_children[i] = nullptr;
	}
}

NodeArray::NodeArray(Node* child, char charOnPos)
{
	_sufLink = &(*root);
	_children[charOnPos - FIRST_CHAR] = child;

}

void NodeArray::AddSuffixLink(NodeArray* sufLink) {
	if (this != &(*root)) {
		_sufLink = sufLink;
	}
}


bool NodeArray::InsertNA(uint64_t& activeLen) {
	if (_children[text[suffix] - FIRST_CHAR] != nullptr) {
		Node* child = _children[text[suffix] - FIRST_CHAR];
		
		if (child->ArcLen() < activeLen) {
			activeLen -= child->ArcLen() + 1;
			suffix += child->ArcLen() + 1;
			return child->_childs->InsertNA(activeLen);

		}
		else {
			if (text[child->_left + activeLen] != text[suffix + activeLen]) {
				child->Split(activeLen);
				child->suff_pos = -1;
				//activeLen -= child->ArcLen() + 1;
				assert(activeLen - (child->ArcLen() + 1) == 0);
				uint64_t zero = 0; //kostil
				child->_childs->InsertSplit(activeLen);
				activeNode = this->_sufLink;
				if (this == &(*root)) {
					--activeLen;
					++suffix;
				}
				return true;
			}
			else {
				++activeLen;
				activeNode = this;

				lastAdded->AddSuffixLink(this);
				lastAdded = this;
				return false;
			}
		}
	}
	else {
		assert(activeLen == 0);
		_children[text[suffix + activeLen] - FIRST_CHAR] = new Node(suffix, textend);
		_children[text[suffix + activeLen] - FIRST_CHAR]->suff_pos = leaf_pos++;
		if (this == &(*root)) {
			++suffix;
		}

		lastAdded->AddSuffixLink(this);
		lastAdded = this;

		activeNode = this->_sufLink;
		return true;
	}
}

void NodeArray::InsertSplit(const uint64_t& activeLen) {
	_children[text[suffix + activeLen] - FIRST_CHAR] = new Node(suffix+activeLen, textend);
	_children[text[suffix + activeLen] - FIRST_CHAR]->suff_pos = leaf_pos++;

	lastAdded->AddSuffixLink(this);
	lastAdded = this;
}

void NodeArray::PatternSearch(const uint64_t& line, uint64_t&& pattern_same_len) {
	char charOnInput;
	pattern_same_len = 0;
	if (std::cin.eof()) {
		
	}
	if((charOnInput = std::cin.get()) != '\n'&& !(std::cin.eof())) {
		Node* child = _children[charOnInput - FIRST_CHAR];
		if (child != nullptr) {
			size_t child_len = child->ArcLen();//ab{ will be 2 which is bad and sad//Decrement textEnd
			do {
				if (child_len < pattern_same_len) {
					if (child->_childs != nullptr) {//Brokes when child is leaf?//looks like not
						std::cin.unget();
						child->_childs->PatternSearch(line);
					}
					else {
						std::cout << "\t\tToo much\n";
						//Clean Line
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

					}
					return;
				}

				if (charOnInput == text[child->_left + pattern_same_len]) {
					++pattern_same_len;
				}
				else {
					std::cout << "Pattern is diff\n";
					//Clean Line
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					return;
				}
			} while ((charOnInput = std::cin.get())!= '\n' && !(std::cin.eof()) );
		}
		else {
			std::cout << "\t\t\t\tDo Not Exist\n";
			//Clean Line
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			return;
		}
		//Find all suffix//Should be separate method
		child->DisplayPos(line);
	}
	else {
		std::cout << "Empty line?";
	}
}

NodeArray::~NodeArray()
{
	for (int i = 0; i < ALPHABET_LENGTH; ++i) {
		if (_children[i] != nullptr) {
			delete _children[i];
		}
	}
}

#pragma endregion

#pragma region Node.cpp

Node::Node()
{
	_right = textend;
	_left = ROOT;
	_childs = nullptr;
}

Node::Node(const uint64_t& left, const std::shared_ptr<uint64_t>& right)
{
	_left = left;
	_right = right;
	_childs = nullptr;
}

Node::Node(const uint64_t& left, const std::shared_ptr<uint64_t>& right, const std::shared_ptr<NodeArray>& children)
{
	_left = left;
	_right = right;
	_childs = children;
}

uint64_t Node::ArcLen()
{
	return *_right - _left;
}

bool Node::IsLeaf()
{
	return _childs == nullptr;//*_right == *textend;//? Check _right == textend
}

void Node::LeafPos(std::vector<uint64_t>& suffix_index) {
	if (this->IsLeaf()) {
		return suffix_index.push_back(suff_pos);
	}
	else {
		for (int ChildCount = 0; ChildCount < ALPHABET_LENGTH; ++ChildCount) {
			if (_childs->_children[ChildCount] != nullptr) {
				_childs->_children[ChildCount]->LeafPos(suffix_index);
			}
		}
	}
}

void Node::DisplayPos(const uint64_t& line) {
	LeafPos(index_of_all_suff);

	RSort(index_of_all_suff);

	std::cout << line << ": " << index_of_all_suff.back();
	index_of_all_suff.pop_back();
	while (index_of_all_suff.size() > 0) {
		std::cout << ", " << index_of_all_suff.back();
		index_of_all_suff.pop_back();
	}
	std::cout << "\n";
}

Node* Node::Split(uint64_t& activeLen)
{
	assert(activeLen > 0);
	assert(!(activeLen > ArcLen()));//<= !(>)
	char charOnPos = text[_left + activeLen];
	Node* childNode(new Node(_left + activeLen, _right, _childs));
	_right = std::shared_ptr<uint64_t>(new uint64_t(_left + activeLen - 1));
	_childs = std::shared_ptr<NodeArray>(new NodeArray(childNode, charOnPos));
	childNode->suff_pos = suff_pos;
	return this;
}

Node::~Node()
{
}

#pragma endregion

#pragma region SufTree.h

class SufTree
{
public:
	//variables
	std::string _text;
	Node* Nodes;
	Node* SuffixNode;

	//Methods
	void Add();
	void Insert();
	void Search();

	//private:
	void ClearSNode();


	//Constructors&Destructors
	SufTree();
	SufTree(std::string text);
	~SufTree();

};
#pragma endregion

#pragma region Check
//
//bool NodeArray::CheckExist(std::string& subs, int64_t dist) {
//	for (int64_t begin = 0; begin < dist; ++begin) {
//		NodeArray* rt = this;
//		int64_t sovpad = 0;
//		while (1) {
//			Node* child = rt->_children[subs[begin+sovpad] - FIRST_CHAR];
//			if (child != nullptr) {
//				size_t child_len = child->ArcLen() + 1;
//				size_t comp_len = *textend - begin - sovpad+1;
//				comp_len = comp_len > child_len ? child_len : comp_len;
//				if (subs.compare(begin + sovpad, comp_len, text + child->_left, comp_len) != 0) {
//					return false;
//				}
//				else {
//					if (comp_len ==  *textend - begin - sovpad+1) { //Pattern полностью совпал
//
//						child->LeafPos(index_of_all_suff);
//
//						RSort(index_of_all_suff);
//
//						if ((int64_t)index_of_all_suff.front() != begin) {
//							index_of_all_suff.clear();
//							return false;
//						}
//						index_of_all_suff.clear();
//						break;
//					}
//					else {
//						if (child->_childs != nullptr) {
//							rt = child->_childs;
//							sovpad += comp_len;
//							continue;
//						}
//						else {
//							return false;
//						}
//					}
//				}
//			}
//			else {
//				return false;
//			}
//		}
//	}
//	return true;
//}

#pragma endregion
//
//typedef std::vector<char> char_array;
//
//char_array charset()
//{
//	//Change this to suit
//	return char_array(
//		{ 
//		'a','b','c','d','e','f',
//		'g','h','i','j','k',
//		'l','m','n','o','p',
//		'q','r','s','t','u',
//		'v','w','x','y','z'
//		});
//};
//
//// given a function that generates a random character,
//// return a string of the requested length
//std::string random_string(size_t length, std::function<char(void)> rand_char)
//{
//	std::string str(length, 0);
//	std::generate_n(str.begin(), length, rand_char);
//	return str;
//}

void freeMem() {
	free(count);
	textend = nullptr;
	root = nullptr;
}

int main()
{
	//
	////0) create the character set.
	////   yes, you can use an array here, 
	////   but a function is cleaner and more flexible
	//const auto ch_set = charset();

	////1) create a non-deterministic random number generator      
	//std::default_random_engine rng(std::random_device{}());

	////2) create a random number "shaper" that will give
	////   us uniformly distributed indices into the character set
	//std::uniform_int_distribution<> dist(0, ch_set.size() - 1);

	////3) create a function that ties them together, to get:
	////   a non-deterministic uniform distribution from the 
	////   character set of your choice.
	//auto randchar = [ch_set, &dist, &rng]() {return ch_set[dist(rng)]; };

	////4) set the length of the string you want and profit!        
	//auto length = 10000;
	//
	//(random_string(length, randchar));
	std::getline(std::cin, text);
	text += TERMINAL_CHAR;

	//memset(NodeArrayBegin, 0, sizeof(NodeArray) * texts.length());
	//memset(NodeBegin, 0, sizeof(Node) * 2 * (texts.length() - 1));

	root = std::shared_ptr<NodeArray>(new NodeArray());

	root->_sufLink = &(*root);
	lastAdded = &(*root);
	activeNode = &(*root);
	uint64_t len = 0;
	uint64_t remainder = 1;//uint64_t remainder =1;
	index_of_all_suff.reserve(text.length());//delete
	//assert(text[0] != 'n' || text[1] != 'x' || text[2] != 'c');
	for (uint64_t textLen = 0; textLen < text.length(); ++textLen, ++(*textend)) {
		///assert(root==root);
		if (activeNode->InsertNA(len)) {
			do {
				if (--remainder == 0) {
					break;
				}
			} while (activeNode->InsertNA(len));


		}
		lastAdded = &(*root);
		//assert(root->CheckExist(texts, leaf_pos));
		///assert(len + 1 != 0);
		++remainder;
	}

	(*textend)-=2;
	uint64_t line(1);
	/*for (int count = 0; count < 100; ++line, ++count) {
		std::string rand_str("a");
		root->PatternSearch(rand_str, line);
	}*/
	while(!std::cin.eof()) {
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
