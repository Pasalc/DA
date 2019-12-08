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

///Classes
class Node;
class NodeArray;

///Defines
#define FIRST_CHAR 'a'
#define TERMINAL_CHAR '{'
#define FULL_BYTE 0xFF
#define BIT_IN_BYTE 8

///Variables
std::string text;
uint64_t suffix = 0;

constexpr auto ALPHABET_LENGTH = 27;
constexpr auto TEXT_END = TERMINAL_CHAR;
constexpr auto ROOT = -1;

std::shared_ptr<uint64_t> textend(new uint64_t(0));
std::shared_ptr<NodeArray> root = nullptr;
NodeArray* lastAdded;
NodeArray* activeNode;
uint64_t leaf_pos = 1;
std::vector<uint64_t> index_of_all_suff;

uint64_t* count = (uint64_t*)malloc(sizeof(uint64_t) * 256);//std::vector<uint64_t> count(256,0);

///RadixSort
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


///NodeArray.h
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

///Node.h
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

///NodeArray.cpp
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
	for (int i = 0; i < ALPHABET_LENGTH; i++) {
		_children[i] = nullptr;
	}
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
				assert(activeLen - (child->ArcLen() + 1) == 0);
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
	_children[text[suffix + activeLen] - FIRST_CHAR] = new Node(suffix + activeLen, textend);
	_children[text[suffix + activeLen] - FIRST_CHAR]->suff_pos = leaf_pos++;

	lastAdded->AddSuffixLink(this);
	lastAdded = this;
}

void NodeArray::PatternSearch(const uint64_t& line, uint64_t&& pattern_same_len) {
	char charOnInput;
	pattern_same_len = 0;
	if ((charOnInput = std::cin.get()) != '\n' && !(std::cin.eof())) {
		Node* child = _children[charOnInput - FIRST_CHAR];
		if (child != nullptr) {
			size_t child_len = child->ArcLen();
			do {
				if (child_len < pattern_same_len) {
					if (child->_childs != nullptr) {
						std::cin.unget();
						child->_childs->PatternSearch(line);
					}
					else {
						//Clean Line
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

					}
					return;
				}

				if (charOnInput == text[child->_left + pattern_same_len]) {
					++pattern_same_len;
				}
				else {
					//Clean Line
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					return;
				}
			} while ((charOnInput = std::cin.get()) != '\n' && !(std::cin.eof()));
		}
		else {
			//Clean Line
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			return;
		}
		//Find all income
		child->DisplayPos(line);
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

//Node.cpp
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

///SufTree.h
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