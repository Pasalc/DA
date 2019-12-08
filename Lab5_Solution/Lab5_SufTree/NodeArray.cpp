#include "pch.h"
//#include "SufTree.h"
#include <assert.h>
#include "NodeArray.h"
#include "Node.h"


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
