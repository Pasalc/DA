#pragma once

#include <cstdint>
#include <ios>
#include "Variables.h"
#include "NodeArray.h"
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