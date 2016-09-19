#include "code_rotate.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <new>

#define PCBN 12
#define PB   5

int cmpr(const void* a, const void* b) {
	return memcmp(*(uint8_t**)a, *(uint8_t**)b, PCBN);
}
bool cmpr_s(const uint8_t* a, const uint8_t* b) {
	return memcmp(a, b, PCBN) < 0;
}

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

	void fix(uint8_t** sbp) {

		for (uint32_t i = 0; i < _cnt; i++) {
			sbp[i] = buf + i * tbn;
		}

		std::sort(sbp, sbp + _cnt, cmpr_s);

#		if 0
		for (uint32_t i = 1; i < _cnt; i++) {
			if (memcmp(sbp[i - 1], sbp[i], PCBN) > 0) {
				printf("bbbbbbbbbbb");
				getchar();
			}
		}
#		endif

		uint8_t* buf_2;
		try {
			buf_2 = new uint8_t[_cnt * tbn];
			_size = _cnt;
		} catch(std::bad_alloc& ba) {
			printf("error: <fix> [%s]\n", ba.what());
			getchar();
		}

		memcpy(buf_2, sbp[0], tbn);
		uint32_t cnt = 1;

		for (uint32_t i = 1; i < _cnt; i++) {
			if (!memcmp(sbp[i - 1], sbp[i], PCBN)) {
				continue;
			}
			memcpy(buf_2 + cnt * tbn, sbp[i], tbn);
			cnt++;
		}

		printf("fix p_x[%u], cnt: %u -> %u\n", pd, _cnt, cnt);

		_cnt = cnt;
		buf = buf_2;
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

#define MAX_TNB 19

uint32_t xxx() {
	uint8_t* buf_a = 0;
	uint8_t** sbp;
	const uint32_t BCN = 1 << 28;
	try {
		buf_a = new uint8_t[BCN * MAX_TNB];
		sbp = new uint8_t*[BCN];
	} catch(std::bad_alloc& bd) {
		printf("error: %s\n", bd.what());
	}

	path_init();

	uint8_t xx[19];

	time_t t_s, t_e;

	for (uint32_t i = 1; i <= 10; i++) {
		path_t& p_xi = p_x[i - 1];
		path_t& p_xo = p_x[i];
		p_xo.alloc(buf_a, BCN);

		uint32_t cnt = p_xi.cnt();

		t_s = time(NULL);
		for (uint32_t j = 0; j < cnt; j++) {
			const uint8_t* xi = p_xi.get_x(j);

			for (uint32_t h = 0; h < RCN; h++) {
				const uint8_t* xo = rotate(xi, xx, rx[h]);

				if (!is_exist(xo, i)) {
					uint64_t path = p_xi.get_path(i);
					path = (path << PB) | rx[h];
					p_xo.append(xo, path);
				}
			}
		}
		t_e = time(NULL);
		printf("\ttime: %u\n", (uint32_t)difftime(t_e, t_s));

		t_s = time(NULL);
		p_xo.fix(sbp);
		t_e = time(NULL);
		printf("\ttime: %u\n", (uint32_t)difftime(t_e, t_s));
	}
}
