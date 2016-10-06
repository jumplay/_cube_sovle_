#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "tools.h"
#include "code_rotate.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//find optimal code for rotation <START>////////////////////////////////////////////////////////////////////////////
template<uint32_t MIN, uint32_t MAX>
void permutation(uint32_t* data, uint32_t cnt, uint32_t ccc, void (*func)(const uint32_t*, uint32_t)) {
	if (cnt > MAX - MIN) {
		func(data, 0);
		return;
	}

	for (uint32_t i = MIN; i <= MAX; i++) {
		if ((1 << i) & ccc) { continue; }
		data[cnt] = i;
		
		permutation<MIN, MAX>(data, cnt + 1, ccc | (1 << i), func);
	}
}

void process_8(const uint32_t* data, uint32_t mode) {
	static uint32_t _buf[8];
	static uint32_t _cnt = 0;
	data--;

	if (mode) {
		printf("%u - ", _cnt);
		for (uint32_t i = 0; i < 8; i++) {
			printf("%u ", _buf[i]);
		}
		puts("");
		return;
	}

	uint32_t ccc = 0;
	for (uint32_t i = 0; i < 8; i++) {
		ccc |= (1 << data[i]);
	}
	if (ccc != (1 << 8) - 1) {
		printf("error: <process_8> ccc\n");
		return;
	}
	
	uint32_t xx[][5] = {
		{0, 2, 6, 3, 0},
		{0, 3, 5, 1, 0},
		{0, 1, 4, 2, 0},
		{1, 5, 7, 4, 1},
		{2, 4, 7, 6, 2},
		{3, 6, 7, 5, 3},
	};

	uint32_t cnt = 0;
	for (uint32_t i = 0; i < 6; i++) {
		int32_t line[5];
		for (uint32_t j = 0; j < 5; j++) {
			line[j] = data[xx[i][j]];
		}
		if (abs(line[0] - line[2]) == abs(line[1] - line[3])) {
			cnt += 2;
		}

		int32_t a = line[1] - line[0];
		int32_t b = line[2] - line[1];
		int32_t c = line[3] - line[2];
		int32_t d = line[4] - line[3];

		if ((a == b && b == c) || (a == c && c == d) || (a == b && b == d) || (b == c && c == d)) {
			cnt += 4;
		} else if (a == b || a == c || a == d || b == c || b == d || c == d) {
			cnt += 2;
		}
	}

	if (cnt > _cnt) {
		_cnt = cnt;
		for (uint32_t i = 0; i < 8; i++) {
			_buf[i] = data[i];
		}
	}

}

void process_12(const uint32_t* data, uint32_t mode) {
	static uint32_t _buf[12];
	static uint32_t _cnt = 0;
	data--;

	if (mode) {
		printf("%u - ", _cnt);
		for (uint32_t i = 0; i < 12; i++) {
			printf("%u ", _buf[i]);
		}
		puts("");
		return;
	}

	uint32_t ccc = 0;
	for (uint32_t i = 0; i < 12; i++) {
		ccc |= (1 << data[i]);
	}
	if (ccc != (1 << 12) - 1) {
		printf("error: <process_12> ccc [");
		for (uint32_t i = 0; i < 12; i++) {
			printf("%u ", data[i]);
		}
		puts("]");
		return;
	}
	
	uint32_t xx[][5] = {
		{ 0,  2,  6, 5, 0 },
		{ 0,  7,  3, 1, 0 },
		{ 1,  8,  4, 2, 1 },
		{ 3, 11,  9, 8, 3 },
		{ 4,  9, 10, 6, 4 },
		{ 5, 10, 11, 7, 5 },
	};

	uint32_t cnt = 0;
	for (uint32_t i = 0; i < 6; i++) {
		int32_t line[5];
		for (uint32_t j = 0; j < 5; j++) {
			line[j] = data[xx[i][j]];
		}
		if (abs(line[0] - line[2]) == abs(line[1] - line[3])) {
			cnt += 2;
		}

		int32_t a = line[1] - line[0];
		int32_t b = line[2] - line[1];
		int32_t c = line[3] - line[2];
		int32_t d = line[4] - line[3];

		if ((a == b && b == c) || (a == c && c == d) || (a == b && b == d) || (b == c && c == d)) {
			cnt += 4;
		} else if (a == b || a == c || a == d || b == c || b == d || c == d) {
			cnt += 2;
		}
	}

	if (cnt > _cnt) {
		_cnt = cnt;
		for (uint32_t i = 0; i < 12; i++) {
			_buf[i] = data[i];
		}
	}

}

void get_optimal_12() {
	uint32_t data[12] = { 0 };
	permutation<1, 11>(&data[0] + 1, 0, 0, process_12);
	process_12(data, 1);
}

void get_optimal_8() {
	uint32_t data[8] = { 0 };
	permutation<1, 7>(&data[0] + 1, 0, 0, process_8);
	process_8(data, 1);
}
//find optimal code for rotation <END>//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//convert optimal code to original <START>//////////////////////////////////////////////////////////////////////////
uint32_t vpc_x2o(uint32_t x) {
	uint32_t map[] = { 0, 1, 5, 3, 6, 7, 4, 2 };
	uint32_t tmp = 0;
	for (uint32_t i = 0; i < 8; i++) {
		tmp = tmp | ((map[(x >> (i * 3)) & 7]) << (i * 3));
	}
	x = tmp;

	x = ((x & (7 <<  0)) <<  0) |
		((x & (7 <<  3)) <<  0) |
		((x & (7 <<  6)) <<  9) |
		((x & (7 <<  9)) <<  0) |
		((x & (7 << 12)) <<  6) |
		((x & (7 << 15)) <<  6) |
		((x & (7 << 18)) >>  6) |
		((x & (7 << 21)) >> 15) ;

	uint32_t idx = 0;
#	define _c_(a,b) (((x >> (a * 3)) & 7) > ((x >> (b * 3)) & 7) ? 1 : 0)
	idx += factorial(7) * (_c_(0, 1) + _c_(0, 2) + _c_(0, 3) + _c_(0, 4) + _c_(0, 5) + _c_(0, 6) + _c_(0, 7));
	idx += factorial(6) * (_c_(1, 2) + _c_(1, 3) + _c_(1, 4) + _c_(1, 5) + _c_(1, 6) + _c_(1, 7));
	idx += factorial(5) * (_c_(2, 3) + _c_(2, 4) + _c_(2, 5) + _c_(2, 6) + _c_(2, 7));
	idx += factorial(4) * (_c_(3, 4) + _c_(3, 5) + _c_(3, 6) + _c_(3, 7));
	idx += factorial(3) * (_c_(4, 5) + _c_(4, 6) + _c_(4, 7));
	idx += factorial(2) * (_c_(5, 6) + _c_(5, 7));
	idx += factorial(1) * (_c_(6, 7));
#	undef  _c_

	return idx;
}

uint32_t voc_x2o(uint32_t x) {
	x = ~x;
	x = ((x & 0x0003) <<  0) |
		((x & 0x000C) <<  0) |
		((x & 0x0030) <<  6) |
		((x & 0x00C0) <<  0) |
		((x & 0x0300) <<  4) |
		((x & 0x0C00) <<  4) |
		((x & 0x3000) >>  4) |
		((x & 0xC000) >> 10) ;

	return	((x >>  0) & 0x03) * 2187 +
			((x >>  2) & 0x03) *  729 +
			((x >>  4) & 0x03) *  243 +
			((x >>  6) & 0x03) *   81 +
			((x >>  8) & 0x03) *   27 +
			((x >> 10) & 0x03) *    9 +
			((x >> 12) & 0x03) *    3 +
			((x >> 14) & 0x03);
}

uint64_t epc_x2o(uint64_t x) {
	uint64_t map[] = { 0, 1, 9, 6, 11, 2, 10, 4, 5, 3, 7, 8 };
	uint64_t tmp = 0;
	for (uint32_t i = 0; i < 12; i++) {
		tmp = tmp | (map[(x >> (i * 4)) & 0xF] << (i * 4));
	}
	x = tmp;

	return	((x & (0xFul <<  0 * 4)) << (0 * 4)) |
			((x & (0xFul <<  1 * 4)) << (0 * 4)) |
			((x & (0xFul <<  2 * 4)) << (7 * 4)) |
			((x & (0xFul <<  3 * 4)) << (3 * 4)) |
			((x & (0xFul <<  4 * 4)) << (7 * 4)) |
			((x & (0xFul <<  5 * 4)) >> (3 * 4)) |
			((x & (0xFul <<  6 * 4)) << (4 * 4)) |
			((x & (0xFul <<  7 * 4)) >> (3 * 4)) |
			((x & (0xFul <<  8 * 4)) >> (3 * 4)) |
			((x & (0xFul <<  9 * 4)) >> (6 * 4)) |
			((x & (0xFul << 10 * 4)) >> (3 * 4)) |
			((x & (0xFul << 11 * 4)) >> (3 * 4)) ;
}

uint32_t eoc_x2o(uint32_t x) {
	return	((x & 0x0001) << 0) |
			((x & 0x0002) << 0) |
			((x & 0x0004) << 7) |
			((x & 0x0008) << 3) |
			((x & 0x0010) << 7) |
			((x & 0x0020) >> 3) |
			((x & 0x0040) << 4) |
			((x & 0x0080) >> 3) |
			((x & 0x0100) >> 3) |
			((x & 0x0200) >> 6) |
			((x & 0x0400) >> 3) |
			((x & 0x0800) >> 3) ;
}

//convert optimal code to original code <END>///////////////////////////////////////////////////////////////////////

