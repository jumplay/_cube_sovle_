#include "code_rotate.h"
#include "tools.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

uint32_t search(uint8_t* buf, uint32_t l, uint32_t r, uint8_t* x) {
	if (memcmp(buf + l * 19, x, 12) > 0) {
		return (l << 1) | 1;
	}
	if (memcmp(buf + r * 19, x, 12) < 0) {
		return ((r + 1) << 1) | 1;
	}
	if (l == r) {
		return 0;
	}

	uint32_t m;
	while (1) {
		if (l == r - 1) {
			if (!memcmp(buf + l * 19, x, 12)) {
				return 0;
			}
			if (!memcmp(buf + l * 19, x, 12)) {
				return 0;
			}
			return (r << 1) | 1;
		}
		m = (l + r) / 2;
		if (memcmp(buf + m * 19, x, 12) > 0) {
			r = m;
		} else {
			l = m;
		}
	}
}

void insert(uint8_t* buf, uint8_t* x, uint32_t pos, uint32_t len) {
	if (pos < len) {
		memmove(buf + (pos + 1) * 19, buf + pos * 19, (len - pos) * 19);
	}
	memcpy(buf + pos * 19, x, 19);
}

uint32_t xxx() {
	uint64_t x_0 = (1ul << 4) | ( 2ul << 8) | (3ul << 12) | (4ul << 16) | (5ul << 20) | (6ul << 24) | (7ul << 28) | (8ul << 32) | (9ul << 36) | (10ul << 40) | (11ul << 44);

	uint8_t* buf = new uint8_t[(1 << 24) * 19];
	memset(buf, 0, (1 << 24) * 19);
	memcpy(buf, &x_0, 6);

	uint8_t x[19] = { 0 };

	uint32_t len = 1;

	time_t t_s = 0, t_e = 0;
	time(&t_s);

	for (uint32_t i = 0; i < 8; i++) {
		for (uint32_t j = 0; j < len; j++) {
			(buf + j * 19)[18] |= (1 << 7);
		}
		for (uint32_t j = 0; j < len; j++) {
			if (!((buf + j * 19)[18] & (1 << 7))) { continue; }

			(buf + j * 19)[18] &= ((1 << 7) - 1);
			memcpy(x, buf + j * 19, 19);
			uint64_t epc = (*(uint64_t*)x) & ((1ul << 48) - 1);
			uint64_t path = (*(uint64_t*)(x + 12)) & ((1ul << 55) - 1);

			for (uint32_t h = 0; h < 18; h++) {
				uint32_t rx = rotation_idx[h / 3][h % 3];
				uint64_t tx = edge_pos_rotate(epc, rx);
				memcpy(x, &tx, 6);

				uint32_t pos = search(buf, 0, len - 1, x);
				if (pos) {
					pos >>= 1;
					//printf("pos: %u\n", pos);
					tx = (path << 5) + rx;
					memcpy(x + 12, &tx, 7);
					insert(buf, x, pos, len);
					len++;
				}

			}
		}
	}

	time(&t_e);
	
	printf("time: %u\n", (uint32_t)(t_e - t_s));

	printf("len: %u\n", len);

	return 0;

	for (uint32_t i = 0; i < len; i++) {
		uint8_t  c12[12];
		uint64_t c64 = *(uint64_t*)(buf + i * 19);
		uint64_t r64 = *(uint64_t*)(buf + i * 19 + 12);

		printf("<%u>------------------------------------\n", i);
		printf("\t--0--\n");
		edge_pos_64_2_12(x_0, c12);
		print_edge(c12);
		
		uint32_t cnt = 0;
		uint64_t ct = x_0;
		for (uint32_t j = 10; j-- > 0; ) {
			uint32_t rx = (r64 >> (j * 5)) & 0x1F;
			if (!rx) { continue; }

			printf("\t--0-%u-%u\n", cnt, rx);
			
			ct = edge_pos_rotate(ct, rx);
			edge_pos_64_2_12(ct, c12);
			print_edge(c12);

			cnt++;
		}
		printf("\t--1----\n");
		edge_pos_64_2_12(c64, c12);
		print_edge(c12);
	}

	return 0;

}

