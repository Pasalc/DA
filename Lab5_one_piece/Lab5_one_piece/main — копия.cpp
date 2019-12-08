// Lab5_SufTree.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
//#include "pch.h"
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

///#pragma region Defines

#define FIRST_CHAR 'a'
#define TERMINAL_CHAR '{'
#define FULL_BYTE 0xFF
#define BIT_IN_BYTE 8

///#pragma endregion

///#pragma region RadixSort
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


uint64_t* count= (uint64_t*)malloc(sizeof(uint64_t)*256);//std::vector<uint64_t> count(256,0);

void RSort(std::vector<uint64_t>& index_of_pattern) {
	std::vector<uint64_t> index_for_radix_sort(index_of_pattern.size());
	for (int current_byte_count = 0; current_byte_count < (int)sizeof(uint64_t); ++current_byte_count) {
		ByteCount(index_of_pattern, index_of_pattern.size(), count, current_byte_count);
		Order(count);
		RadixSort(index_of_pattern, index_of_pattern.size(),count,current_byte_count,index_for_radix_sort);
		index_of_pattern.swap(index_for_radix_sort);
	}
}

void Swap(uint64_t* first, uint64_t* second) {
	uint64_t* temp = first;
	first = second;
	second = temp;
}

///#pragma endregion

///#pragma region NodeArray.h
class Node;
class NodeArray;
constexpr auto ALPHABET_LENGTH = 27;
constexpr auto TEXT_END = TERMINAL_CHAR;
constexpr auto ROOT = -1;

NodeArray* NodeArrayBegin = nullptr;
NodeArray* NodeArrayCurrent=nullptr;
NodeArray* NodeArrayEnd = nullptr;

class NodeArray
{
public:
	NodeArray* _sufLink;
	Node* _children[ALPHABET_LENGTH];
	//Constructors
	NodeArray();
	NodeArray(Node* child, char charOnPos); //Creates leaf

	//Methods
	bool InsertNA(uint64_t& activeLen, char*& suffix);
	void InsertSplit(char* suffix);
	void PatternSearch(std::string& pattern, const uint64_t& line, uint64_t&& pattern_same_len=0);
	bool CheckExist(std::string& substring,int64_t dist);
	NodeArray* AddSuffixLink(NodeArray* sufLink);

	//Operators
	void *operator new(size_t size);
	void operator delete(void* p);

	//Destructor
	virtual ~NodeArray();
};
///#pragma endregion

///#pragma region  Node.h
Node* NodeBegin = nullptr;
Node* NodeCurrent = nullptr;
Node* NodeEnd = nullptr;
class Node
{
public:

	uint64_t _left;
	std::shared_ptr<uint64_t> _right;
	NodeArray* _childs;
	uint64_t leaf_index;

	//Constructors
	Node();
	Node(const uint64_t& left, const std::shared_ptr<uint64_t>& right);	//Creates internal node
	Node(const uint64_t& left, const std::shared_ptr<uint64_t>& right, NodeArray* children); //Creates internal node with _sufLink

	//Methods
	Node* Split(uint64_t& pos);
	uint64_t ArcLen();
	bool IsLeaf();
	void LeafPos(std::vector<uint64_t>& index);

	//Operators
	void *operator new(size_t size);
	void operator delete(void* p);

	//Destructor
	virtual ~Node();

};
///#pragma endregion

std::shared_ptr<uint64_t> textend(new uint64_t(0));
NodeArray* root = nullptr;
NodeArray* lastAdded = nullptr;
NodeArray* activeNode = nullptr;
uint64_t leaf_pos=1;
char* text = nullptr;
std::vector<uint64_t> index_of_all_suff;

///#pragma region NodeArray.cpp

NodeArray::NodeArray()
{
	_sufLink = root;//=root;
	for (int i = 0; i < ALPHABET_LENGTH; i++) {
		_children[i] = nullptr;
	}
}

NodeArray::NodeArray(Node* child, char charOnPos) {
	_sufLink = root;
	_children[charOnPos - FIRST_CHAR] = child;

}

void* NodeArray::operator new(size_t size) {
	assert(NodeArrayCurrent < NodeArrayEnd);
	return NodeArrayCurrent++;
}

void NodeArray::operator delete(void* p) {
	//assert(false);
}

NodeArray* NodeArray::AddSuffixLink(NodeArray* sufLink) {
	return _sufLink = this == root ? _sufLink : sufLink;
}


bool NodeArray::InsertNA(uint64_t& activeLen, char*& suffix) {
	Node* child = _children[*suffix - FIRST_CHAR];
	if (child != nullptr) {
		if (child->ArcLen() < activeLen) {
			activeLen -= child->ArcLen() + 1;
			suffix += child->ArcLen() + 1;
			return child->_childs->InsertNA(activeLen, suffix);

		}
		else {
			if (text[child->_left + activeLen] != suffix[activeLen]) {
				child->Split(activeLen);
				child->leaf_index = -1;
				//activeLen -= child->ArcLen() + 1;
				assert(activeLen - (child->ArcLen() + 1) ==0);
				uint64_t zero = 0; //kostil
				char* tempSuf= suffix + activeLen;
				child->_childs->InsertNA(zero, tempSuf);
				activeNode = this->_sufLink;
				if (this == root) {
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
		_children[*suffix - FIRST_CHAR] = new Node(suffix - text, textend);
		_children[*suffix - FIRST_CHAR]->leaf_index = leaf_pos++;
		if (this == root) {
			++suffix;
		}

		lastAdded->AddSuffixLink(this);
		lastAdded = this;

		activeNode = this->_sufLink;
		return true;
	}

}

void NodeArray::InsertSplit(char* suffix) {
	_children[*suffix - FIRST_CHAR] = new Node(suffix - text, textend);
	_children[*suffix - FIRST_CHAR]->leaf_index = leaf_pos++;

	lastAdded->AddSuffixLink(this);
	lastAdded = this;
}

void NodeArray::PatternSearch(std::string& pattern, const uint64_t& line, uint64_t&& pattern_same_len)  {
	Node* child = _children[pattern[pattern_same_len]- FIRST_CHAR];
	if (child != nullptr) {
		size_t child_len = child->ArcLen() + 1;
		size_t comp_len = pattern.length() - pattern_same_len;
		comp_len = comp_len > child_len ? child_len : comp_len;
		if (pattern.compare(pattern_same_len, comp_len, text + child->_left, comp_len) != 0) {
			//std::cout << "Not a substring\n";//TO UNDO
		}
		else {
			if (comp_len == pattern.length() - pattern_same_len) { //Pattern полностью совпал

				child->LeafPos(index_of_all_suff);

				RSort(index_of_all_suff);

				std::cout << line << ": " << index_of_all_suff.back();
				index_of_all_suff.pop_back();
				while (index_of_all_suff.size() > 0) {
					std::cout << ", " << index_of_all_suff.back();
					index_of_all_suff.pop_back();
				}
				std::cout << "\n";
			}
			else {
				if (child->_childs != nullptr) {
					child->_childs->PatternSearch(pattern, line, pattern_same_len + comp_len);
				}
				else {
					//std::cout << "\t\tToo much\n";
				}
			}
		}
	}
	else {
		//std::cout << "\t\t\t\tDo Not Exist\n";
	}
}

NodeArray::~NodeArray()
{
}

///#pragma endregion

///#pragma region Node.cpp

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

Node::Node(const uint64_t& left, const std::shared_ptr<uint64_t>& right, NodeArray* children)
{
	_left = left;
	_right = right;
	_childs = children;
}

void* Node::operator new(size_t size) {
	assert(NodeCurrent < NodeEnd);
	return NodeCurrent++;
}

void Node::operator delete(void* p) {
	std::cout << "Wait, wat??? DELETE NODE ???\n";
	///assert(false);
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
		return suffix_index.push_back(leaf_index);
	}
	else {
		for (int ChildCount = 0; ChildCount < ALPHABET_LENGTH; ++ChildCount) {
			if (_childs->_children[ChildCount] != nullptr) {
				_childs->_children[ChildCount]->LeafPos(suffix_index);
			}
		}
	}
}

Node* Node::Split(uint64_t& activeLen)
{
	assert(activeLen > 0);
	assert(!(activeLen > ArcLen()));//<= !(>)
	char charOnPos = text[_left + activeLen];
	Node* childNode = new Node(_left + activeLen, _right, _childs);
	_right = std::shared_ptr<uint64_t>(new uint64_t(_left + activeLen - 1));
	_childs = new NodeArray(childNode, charOnPos);
	childNode->leaf_index = leaf_index;
	return this;
}

Node::~Node()
{
	_right=nullptr;
}

///#pragma endregion

///#pragma region SufTree.h

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


void freeMem() {
	free(count);
	delete[] text;
	free(NodeArrayBegin);
	for (Node* curr = NodeBegin; curr < NodeCurrent; ++curr) {
		curr->_right = nullptr;
	}
	free(NodeBegin);
	textend = nullptr;
}

int main()
{
	std::string texts;
	std::getline(std::cin, texts);
	texts += TERMINAL_CHAR;
	NodeArrayCurrent = NodeArrayBegin = (NodeArray*)malloc(sizeof(NodeArray) * texts.length());
	NodeCurrent = NodeBegin = (Node*)malloc(sizeof(Node) * 2 * (texts.length() - 1));

	

	NodeArrayEnd = NodeArrayBegin + texts.length();
	NodeEnd = NodeBegin + 2 * (texts.length() - 1);

	memset(NodeArrayBegin, 0, sizeof(NodeArray) * texts.length());
	memset(NodeBegin, 0, sizeof(Node) * 2 * (texts.length() - 1));

	root = new NodeArray();
	
	//Не хватает места на текст, реализуй поиск паттерна по символам
	text = new char[texts.length()+1];
	strcpy(text, texts.c_str());
	assert(texts[0] != 'n' || texts[1] != 'x' || texts[2] != 'c');

	root->_sufLink = root;
	lastAdded = root;
	activeNode = root;
	char* suff = text;
	uint64_t len = 0;
	uint64_t remainder = 1;//uint64_t remainder =1;
	index_of_all_suff.reserve(texts.length());
	for (size_t textLen = 0; textLen < texts.length(); ++textLen, ++(*textend)) {
		///assert(root==root);
		if (activeNode->InsertNA(len, suff)) {//_childs are wrong
			do {
				if (--remainder == 0) {  
					break; 
				}
			} while (activeNode->InsertNA(len, suff));

			
		}
		lastAdded = root;
		//assert(root->CheckExist(texts, leaf_pos));
		assert(len + 1 != 0);
		++remainder;
	}
	
	--(*textend);
	uint64_t line(1);
	/*for (int count = 0; count < 100; ++line, ++count) {
		std::string rand_str("a");
		root->PatternSearch(rand_str, line);
	}*/
	for (std::string pattern; std::getline(std::cin, pattern); ++line) {
		root->PatternSearch(pattern, line);
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
