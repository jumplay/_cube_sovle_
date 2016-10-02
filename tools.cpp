#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "tools.h"

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
