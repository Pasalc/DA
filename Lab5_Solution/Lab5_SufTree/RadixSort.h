#pragma once
#include "Variables.h"

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
	uint64_t i;
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
