#pragma once
#include <stdint.h>

static const uint32_t _factorial[] = {
	1,
	1,
	1 * 2,
	1 * 2 * 3,
	1 * 2 * 3 * 4,
	1 * 2 * 3 * 4 * 5,
	1 * 2 * 3 * 4 * 5 * 6,
	1 * 2 * 3 * 4 * 5 * 6 * 7,
	1 * 2 * 3 * 4 * 5 * 6 * 7 * 8,
	1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9,
	1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10,
	1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11,
	1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12,
};
#define factorial(x) _factorial[x]

static const uint32_t rotation_idx[6][3] = {
	{ 16 + 1, 16 + 2, 16 + 3 },					// x+
	{  8 + 1,  8 + 2,  8 + 3 },					// y+
	{  4 + 1,  4 + 2,  4 + 3 },					// z+
	{  8 + 4 + 1,  8 + 4 + 2,  8 + 4 + 3 },		// x-
	{ 16 + 4 + 1, 16 + 4 + 2, 16 + 4 + 3 },		// y-
	{ 16 + 8 + 1, 16 + 8 + 2, 16 + 8 + 3 },		// z-
};

void create_rotation_map();

uint32_t vertex_pos_code_to_idx(uint64_t code);
uint64_t vertex_pos_idx_to_code(uint32_t idx);

uint32_t vertex_ort_code_to_idx(uint64_t code);
uint64_t vertex_ort_idx_to_code(uint32_t idx);

uint32_t vertex_pos_rotate(uint32_t idx, uint32_t rx);
uint32_t vertex_ort_rotate(uint32_t idx, uint32_t rx);

uint64_t edge_pos_rotate(uint64_t code, uint32_t rx);
uint32_t edge_ort_rotate(uint32_t code, uint32_t rx);
