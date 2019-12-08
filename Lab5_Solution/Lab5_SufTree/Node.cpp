#include "pch.h"
//#include "SufTree.h"
#include <assert.h>
#include "Node.h"
#include "RadixSort.h"
//
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
