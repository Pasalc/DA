#pragma once
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

class Node;
class NodeArray;

#define FIRST_CHAR 'a'
#define TERMINAL_CHAR '{'
#define FULL_BYTE 0xFF
#define BIT_IN_BYTE 8

#define ALPHABET_LENGTH 27
#define TEXT_END TERMINAL_CHAR
#define ROOT -1

//
//constexpr auto ALPHABET_LENGTH = 27;
//constexpr auto TEXT_END = TERMINAL_CHAR;
//constexpr auto ROOT = -1;

extern std::string text;
extern uint64_t suffix;

extern std::shared_ptr<uint64_t> textend;
extern std::shared_ptr<NodeArray> root;
extern NodeArray* lastAdded;
extern NodeArray* activeNode;
extern uint64_t leaf_pos;
extern std::vector<uint64_t> index_of_all_suff;
extern std::vector<uint64_t> index_of_all_suff;
extern uint64_t* count;
