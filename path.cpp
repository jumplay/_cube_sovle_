#include "code_rotate.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <new>

#define _XN_10_ 520704474624ul
#define _XN_9_   35947141632ul
#define _XN_8_    2481632640ul
#define _XN_7_     171320832ul
#define _XN_6_      11827296ul
#define _XN_5_        816480ul
#define _XN_4_         56376ul
#define _XN_3_          3888ul
#define _XN_2_           270ul
#define _XN_1_            18ul

#define _cft_(stack,x)																			\
				(																				\
				 (stack[-1] >> 2) == (x >> 2) ||												\
				 ((stack[-1] >> 2) + (stack[-2] >> 2) == 7 && (stack[-2] >> 2) == (x >> 2))		\
				)

template <uint32_t XN>
void go(ccd_t ccd, uint8_t*& px, uint32_t* stack, uint32_t dp) {
	if (dp > XN) {
		ccd.code(px);
		px += CBN;
		return;
	}

	for (uint32_t i = 1; i < RCN + 1; i++) {
		if (_cft_(stack, rx[i])) {
			continue;
		}
		ccd_t tmp_ccd(ccd);
		tmp_ccd.one_step(rx[i]);
		*stack = rx[i];
		go<XN>(tmp_ccd, px, stack + 1, dp + 1);
	}
}

template <uint32_t L>
void radix_sort(uint8_t* px, uint8_t* buf, uint32_t len) {
	uint32_t cnt[256] = { 0 };
	uint32_t pos[256] = { 0 };
	uint8_t* bu_a = px;
	uint8_t* bu_b = buf;

	for (uint32_t i = 1; i <= L; i++) {
		uint8_t* const e = bu_a + len * L;

		uint8_t* b = bu_a;
		for (; b < e; b += L) {
			cnt[b[L - i]]++;
		}

		for (uint32_t j = 0; j < 255; j++) {
			pos[j + 1] = pos[j] + cnt[j];
		}


		for (b = bu_a; b < e; b += L) {
			memcpy(bu_b + pos[b[L - i]]++ * L, b, L);
		}

		memset(&cnt[0], 0, sizeof(uint32_t) * 256);
		memset(&pos[0], 0, sizeof(uint32_t) * 256);

		uint8_t* tmp = bu_a;
		bu_a = bu_b;
		bu_b = tmp;
	}
	if (L & 1) { memcpy(px, buf, len * L); }
}

uint32_t xxx() {
	uint8_t* buf = 0;
	const uint64_t BSN = (_XN_7_ + _XN_6_ + _XN_5_ + _XN_4_ + _XN_3_ + _XN_2_ + _XN_1_) * CBN;
	try {
		buf = new uint8_t[BSN];
	} catch(std::bad_alloc& bd) {
		printf("error: %s\n", bd.what());
	}

	uint32_t _stack[12] = { 0 };
	uint32_t* stack = &_stack[2];
	ccd_t ccd;

	uint8_t* px = buf;
	go<1>(ccd, px, stack, 1);
	go<2>(ccd, px, stack, 1);
	go<3>(ccd, px, stack, 1);
	go<4>(ccd, px, stack, 1);
	go<5>(ccd, px, stack, 1);
	go<6>(ccd, px, stack, 1);
	go<7>(ccd, px, stack, 1);

	uint32_t cnt = (uint32_t)(px - buf) / CBN;

	radix_sort<CBN>(buf, buf + BSN / 2, cnt);

#	if 0
	for (uint32_t i = 0; i < cnt; i++) {
		for (uint32_t j = 0; j < CBN; j++) {
			printf("%3u ",  (uint32_t)(buf[i * CBN + j]));
		}
		printf("\n");
	}
#	endif

	uint32_t sn = 0;
	for (uint32_t i = 1; i < cnt; i++) {
		int32_t cx = memcmp(buf + (i - 1) * CBN, buf + i * CBN, CBN);
		if (cx > 0) {
			printf("error!\n");
		} else if (!cx) {
			sn++;
		}
	}
	printf(">%u -> %u\n", cnt, cnt - sn);

}

