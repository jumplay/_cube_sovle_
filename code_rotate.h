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

////////////////////////////////////////////////////////////////////////////////////////////
#define RCN		18
#define PHMASK	0x1F
#define PHBN	5
#define CBN		12

static uint32_t rx[RCN + 1];
static void init_rx() {
	for (uint32_t i = 0; i < RCN; i++) {
		rx[i + 1] = rotation_idx[i / 3][i % 3];
	}
}
static uint32_t _init_rx_ = (init_rx(), 1);

struct ccd_t {
	uint16_t vpc;
	uint16_t voc;
	uint16_t eoc;
	uint64_t epc;

	ccd_t(): vpc(0), voc(0), eoc(0) {
		epc =	(0ul <<  0) | (1ul <<  4) | (2ul  <<  8) | (3ul  << 12) |	\
				(4ul << 16) | (5ul << 20) | (6ul  << 24) | (7ul  << 28) |	\
				(8ul << 32) | (9ul << 36) | (10ul << 40) | (11ul << 44);
	}

	ccd_t(ccd_t& xx): vpc(xx.vpc), voc(xx.voc), eoc(xx.eoc), epc(xx.epc) {}

	ccd_t& go(uint64_t path);

	void one_step(uint32_t x);

	uint8_t* code(uint8_t* code) const;

	void print(uint32_t x) const;
};
