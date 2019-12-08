#pragma once

#include "Variables.h"
#include "Node.h"

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



