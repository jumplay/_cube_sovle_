#include "code_rotate.h"
#include "tools.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <new>

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

uint32_t xxx_1() {
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

#define PCBN 12
#define PB   5

class path_t {
	uint8_t* buf;
	uint32_t _cnt;
	uint32_t _size;
	uint32_t pbn;
	uint32_t tbn;
	uint32_t pd;

public:
	path_t(): buf(0), _cnt(0), _size(0), pbn(0), tbn(0), pd(0u-1) {}

	void set_pbn(uint32_t n) { pbn = n; tbn = PCBN + pbn; }
	void set_pd(uint32_t d) { pd = d; }
	
	void alloc(uint32_t l) {
		try {
			buf = new uint8_t[tbn * l];
			_size = l;
		} catch(std::bad_alloc& ba) {
			printf("error: <buf_alloc> [pd: %u, l: %u]", pd, l);;
		}
	}

	void re_alloc() {
		try {
			uint8_t* buf_b = new uint8_t[tbn * _cnt];
			memcpy(buf_b, buf, tbn * _cnt);
			buf = buf_b;
			_size = _cnt;
		} catch(std::bad_alloc& ba) {
			printf("error: <re_alloc> [pd: %u, cnt: %u]", pd, _cnt);
		}
	}

	void alloc(uint8_t* buf_x, uint32_t l) {
		if (buf || _cnt || _size) {
			printf("error: <alloc> [...]\n");
			getchar();
			return;
		}
		buf = buf_x;
		_size = l;
	}

	void sort_and_clean() {
	}
	
	void append(const uint8_t* x, uint64_t path = 0) {
		if (_cnt == _size) {
			printf("error: <append> [overflow, pd: %u, cnt: %u]", pd, _cnt);
			getchar();
			return;
		}

		memcpy(buf + _cnt * tbn, x, PCBN);
		memcpy(buf + _cnt * tbn + PCBN, (uint8_t*)& path, pbn);
		_cnt++;
	}

	uint32_t size() { return _size; }
	uint32_t cnt()  { return _cnt; }

	const uint8_t* get_x(uint32_t i) { return buf + tbn * i; }
	uint64_t get_path(uint32_t i)  { return (*(uint64_t*)(buf + tbn * i + PCBN)) & ((1 << PB * pd) - 1); }

	uint32_t search(const uint8_t* x) {
		uint32_t l = 0;
		uint32_t r = _cnt - 1;

		if (memcmp(buf + l * tbn, x, PCBN) > 0) {
			return 0;
		}
		if (memcmp(buf + r * tbn, x, PCBN) < 0) {
			return 0;
		}
		if (l == r) {
			return 1;
		}

		uint32_t m;
		while (1) {
			if (l == r - 1) {
				if (!memcmp(buf + l * tbn, x, PCBN)) {
					return 1;
				}
				if (!memcmp(buf + l * tbn, x, PCBN)) {
					return 1;
				}
				return 0;
			}
			m = (l + r) / 2;
			if (memcmp(buf + m * tbn, x, PCBN) > 0) {
				r = m;
			} else {
				l = m;
			}
		}
	}

	void print(uint32_t i) {
		const uint8_t* x = buf + i * tbn;
		uint64_t path = (*(uint64_t*)(buf + i * tbn + PCBN)) & ((1ul << (pbn * 8)) - 1);
		printf("\t<p_x[%u]>", pd);
		while (path) {
			printf("\t%u,", (uint32_t)(path & ((1 << PB) - 1)));
			path >>= PB;
		}
		printf("\n");

		print_cube_x(x);
		printf("\n");
	}
};

#define RCN 18
static path_t p_x[11];
static uint32_t rx[RCN];

void path_init() {
	for (uint32_t i = 0; i < RCN; i++) {
		rx[i] = rotation_idx[i / 3][i % 3];
	}

	for (uint32_t i = 0; i < 11; i++) {
		p_x[i].set_pbn((i * PB + 7) / 8);
		p_x[i].set_pd(i);
	}

	uint8_t buf[12];
	*(uint64_t*)buf = (1ul << 4) | ( 2ul << 8) | (3ul << 12) | (4ul << 16) | (5ul << 20) | (6ul << 24) | (7ul << 28) | (8ul << 32) | (9ul << 36) | (10ul << 40) | (11ul << 44);
	*(uint16_t*)(buf + 6) = 0;
	*(uint16_t*)(buf + 8) = 0;
	*(uint16_t*)(buf + 10) = 0;

	p_x[0].alloc(1);
	p_x[0].append(buf);

}


const uint8_t* rotate(const uint8_t* xi, uint8_t* xo, uint32_t rx) {
	*(uint64_t*)xo        =           edge_pos_rotate(   *(uint64_t*)xi      ,  rx);
	*(uint16_t*)(xo + 6)  = (uint16_t)edge_ort_rotate(   *(uint16_t*)(xi + 6),  rx);
	*(uint16_t*)(xo + 8)  = (uint16_t)vertex_pos_rotate( *(uint16_t*)(xi + 8),  rx);
	*(uint16_t*)(xo + 10) = (uint16_t)vertex_ort_rotate( *(uint16_t*)(xi + 10), rx);

	return xo;
}

uint32_t is_exist(const uint8_t* x, uint32_t n) {
	uint32_t ex = 0;
	for (uint32_t i = 0; i < n && !ex; i++) {
		ex = p_x[i].search(x);
	}
	return ex;
}


void check() {
	p_x[0].print(0);

	path_t& p_xt = p_x[1];
	for (uint32_t i = 0; i < p_xt.cnt(); i++) {
		p_xt.print(i);
	}
}

uint32_t xxx() {
	uint8_t* buf_a = 0;
	const uint32_t BCN = 1 << 28;
	try {
		buf_a = new uint8_t[BCN];
	} catch(std::bad_alloc& bd) {
		printf("error: %s\n", bd.what());
	}

	path_init();

	uint8_t xx[19];

	for (uint32_t i = 1; i <= 1; i++) {
		path_t& p_xi = p_x[i - 1];
		path_t& p_xo = p_x[i];
		p_xo.alloc(buf_a, BCN);

		uint32_t cnt = p_xi.cnt();
		for (uint32_t j = 0; j < cnt; j++) {
			const uint8_t* xi = p_xi.get_x(j);
			for (uint32_t h = 0; h < RCN; h++) {
				const uint8_t* xo = rotate(xi, xx, rx[h]);

#				if 0
				printf(">%u\n", h);
				print_cube_x(xo);
#				endif

				if (!is_exist(xo, i)) {
					uint64_t path = p_xi.get_path(i);
					path = (path << PB) | rx[h];
					p_xo.append(xo, path);
				}
			}
		}
		p_xo.sort_and_clean();
		p_xo.re_alloc();
	}

	check();
}
