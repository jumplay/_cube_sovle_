#include "code_rotate.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <new>


bool cmpr(uint64_t a, uint64_t b) {
	static ccd_t ccd;
	return ccd.cmpr(a, b) < 0;
}

class path_t {
		uint64_t* buf;
		uint32_t _cnt;
		uint32_t _size;
		uint32_t pd;
		ccd_t ccd;

public:
	path_t(): buf(0), _cnt(0), _size(0), pd(0u-1) {}

	void set_pd(uint32_t d) { pd = d; }
	
	void alloc(uint32_t l) {
		try {
			buf = new uint64_t[l];
			_size = l;
		} catch(std::bad_alloc& ba) {
			printf("error: <buf_alloc> [pd: %u, l: %u]", pd, l);;
		}
	}

	void re_alloc() {
		try {
			uint64_t* buf_b = new uint64_t[_cnt];
			memcpy(buf_b, buf, sizeof(uint64_t) * _cnt);
			buf = buf_b;
			_size = _cnt;
		} catch(std::bad_alloc& ba) {
			printf("error: <re_alloc> [pd: %u, cnt: %u]", pd, _cnt);
		}
	}

	void alloc(uint64_t* buf_x, uint32_t l) {
		if (buf || _cnt || _size) {
			printf("error: <alloc> [...]\n");
			getchar();
			return;
		}
		buf = buf_x;
		_size = l;
	}

	void fix() {
		std::sort(buf, buf + _cnt, cmpr);

		uint32_t cnt = 1;
		for (uint32_t i = 1; i < _cnt; i++) {
			if (!ccd.cmpr(buf[i - 1], buf[i])) { continue; }
			buf[cnt++] = buf[i];
		}

		printf("<fix> p_x[%u] %u -> %u\n", pd, _cnt, cnt);

		uint64_t* buf_new;
		try {
			buf_new = new uint64_t[cnt];
			_size = _cnt = cnt;
		} catch(std::bad_alloc& ba) {
			printf("error: <fix> [%s]", ba.what());
			getchar();
			return;
		}

		memcpy(buf_new, buf, sizeof(uint64_t) * cnt);
		buf = buf_new;
	}
	
	void append(uint64_t x) {
		if (_cnt == _size) {
			printf("error: <append> [overflow, pd: %u, cnt: %u]", pd, _cnt);
			getchar();
			return;
		}

		buf[_cnt++] = x;
	}

	uint32_t size() { return _size; }
	uint32_t cnt()  { return _cnt; }

	uint64_t& operator [](uint32_t i) {
		return buf[i];
	}

	uint32_t search(uint64_t x) {
		uint32_t l = 0;
		uint32_t r = _cnt - 1;

		if (ccd.cmpr(buf[l], x) > 0) {
			return 0;
		}
		if (ccd.cmpr(buf[r], x) < 0) {
			return 0;
		}
		if (l == r) {
			return 1;
		}

		uint32_t m;
		while (1) {
			if (l == r - 1) {
				if (!ccd.cmpr(buf[l], x)) {
					return 1;
				}
				if (!ccd.cmpr(buf[r], x)) {
					return 1;
				}
				return 0;
			}
			m = (l + r) / 2;
			if (ccd.cmpr(buf[m], x) > 0) {
				r = m;
			} else {
				l = m;
			}
		}
	}

	void print(uint32_t i) {
	}
};


static path_t p_x[10];

void path_init() {
	for (uint32_t i = 0; i < 10; i++) {
		p_x[i].set_pd(i);
	}

	p_x[0].alloc(RCN);
	for (uint32_t i = 1; i < RCN + 1; i++) {
		p_x[0].append(i);
	}
	p_x[0].fix();
}

uint32_t is_exist(uint64_t path, uint32_t n) {
	uint32_t ex = 0;

	//printf("path: %u %u\n", (uint32_t)(path & PHMASK), (uint32_t)(path >> 5)&PHMASK);

	for (uint32_t i = 0; i < n && !ex; i++) {
		ex = p_x[i].search(path);
	}
	//puts("ssssssssss");
	return ex;
}


void check() {
	p_x[0].print(0);

	path_t& p_xt = p_x[1];
	for (uint32_t i = 0; i < p_xt.cnt(); i++) {
		p_xt.print(i);
	}
}

uint32_t xxx__() {
	ccd_t ccd;

	//ccd.print(1);

	time_t t_s, t_e;
	t_s = time(NULL);
	
	for (uint32_t i = 0; i < (1 << 28); i++) {
		for (uint32_t j = 1; j < RCN + 1; j++) {
			//printf("rx: <%u, %u>\n", (j - 1) / 3, (j - 1) % 3);
			ccd.go(j);
			//ccd.print(1);
			//getchar();
		}
	}

	t_e = time(NULL);
	printf("\ttime: %u\n", (uint32_t)difftime(t_e, t_s));
}

uint32_t xxx() {
	uint64_t* buf = 0;
	const uint32_t BCN = 1 << 30;
	try {
		buf = new uint64_t[BCN];
	} catch(std::bad_alloc& bd) {
		printf("error: %s\n", bd.what());
	}

	path_init();


	time_t t_s, t_e;

	for (uint32_t i = 1; i <= 8; i++) {
		path_t& p_xi = p_x[i - 1];
		path_t& p_xo = p_x[i];
		p_xo.alloc(buf, BCN);

		uint32_t cnt = p_xi.cnt();

		t_s = time(NULL);

		for (uint32_t j = 0; j < cnt; j++) {
			uint64_t xi = p_xi[j];

			for (uint64_t h = 1; h < RCN + 1; h++) {
				uint64_t xo = (h << PHBN * i) | xi;

				if (!is_exist(xo, i)) {
					p_xo.append(xo);
				}
			}
		}

		t_e = time(NULL);
		printf("\ttime: %u\n", (uint32_t)difftime(t_e, t_s));
		t_s = time(NULL);

		p_xo.fix();

		t_e = time(NULL);
		printf("\ttime: %u\n", (uint32_t)difftime(t_e, t_s));
	}
}

