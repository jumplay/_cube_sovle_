#include "code_rotate.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <new>
#include "x_rotate.h"

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

#define _c3_(x,y,z) x##y##z

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
		if (_cft_(stack, rx[i])) { continue; }

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

uint32_t xxx_0() {
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

	time_count tc_a;

	tc_a.start();
	go<6>(ccd, px, stack, 1);
	tc_a.end("x ");


	uint32_t cnt = (uint32_t)(px - buf) / CBN;

	uint8_t* buf_2 = buf + cnt * CBN;

	radix_sort<CBN>(buf, buf_2, cnt);

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

template <uint32_t XN, typename T>
void go_a(ccd_t ccd, T* pxx, uint32_t* stack, uint32_t dp) {
	if (dp > XN) {
		pxx[ccd.code_25()]++;
		return;
	}

	for (uint32_t i = 1; i < RCN + 1; i++) {
		if (_cft_(stack, rx[i])) { continue; }

#		if 1
		ccd_t tmp_ccd(ccd);
		tmp_ccd.one_step(rx[i]);
		*stack = rx[i];
		go_a<XN>(tmp_ccd, pxx, stack + 1, dp + 1);
#		else
		*stack = rx[i];
		go_a<XN>(ccd, pxx, stack + 1, dp + 1);
#		endif
	}
}

bool cmpr(uint64_t a, uint64_t b) {
	return a < b;
}


uint32_t xxx_1() {
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

	typedef uint64_t T;
	T* pxx = (T*)buf;
	uint32_t c25 = 1 << 25;
	for (uint32_t i = 0; i < c25; i++) { pxx[i] = 0; }

	time_count tc_a;
	tc_a.start();
	go_a<1>(ccd, pxx, stack, 1);
	go_a<2>(ccd, pxx, stack, 1);
	go_a<3>(ccd, pxx, stack, 1);
	go_a<4>(ccd, pxx, stack, 1);
	go_a<5>(ccd, pxx, stack, 1);
	go_a<6>(ccd, pxx, stack, 1);
	tc_a.end("x ");

	uint64_t cnt = 0;
	for (uint32_t i = 0; i < c25; i++) { cnt += pxx[i]; }
	printf("cnt: %lu\n", cnt);

	std::sort(pxx, pxx + c25, cmpr);
	for (uint32_t i = 0; i < c25; i++) {
		printf("%lu\n", (uint64_t)pxx[i]);
	}

}

static const uint32_t* VPRM;
static const uint32_t* VORM;
static const uint64_t* EPRM;
static const uint32_t* EORM;

template <uint32_t XN, typename T>
void go_b(uint32_t code, T* pxx, uint32_t* stack, uint32_t dp) {
	if (dp > XN) {
		pxx[code]++;
		return;
	}

	for (uint32_t i = 1; i < RCN + 1; i++) {
		if (_cft_(stack, rx[i])) { continue; }

		uint32_t tmp_code = (VORM[((code >> 12) << 5) | rx[i]] << 12) | EORM[((code & ((1 << 12) - 1)) << 5) | rx[i]];

		*stack = rx[i];
		go_b<XN>(tmp_code, pxx, stack + 1, dp + 1);
	}
}

template <uint32_t XN, typename T>
void go_c(uint32_t code, T* pxx, uint32_t* stack, uint32_t dp) {
	if (dp > XN) {
		pxx[code]++;
		return;
	}

				//tmp_code = (VORM[((code >> 12) << 5) | rx[i]] << 12) | EORM[((code & ((1 << 12) - 1)) << 5) | rx[i]];
				//tmp_code = EORM[((code & ((1 << 12) - 1)) << 5) | rx[i]];	
				//tmp_code = EORM[(code << 5) | rx[i]];
#	define go_ss(i)																										\
			if (!_cft_(stack, rx[i])) {																					\
				tmp_code = VORM[(code << 5) | rx[i]];																	\
				*stack = rx[i];																							\
				go_c<XN>(tmp_code, pxx, stack + 1, dp + 1);																\
			}																											
	uint32_t tmp_code;

	go_ss(1);
	go_ss(2);
	go_ss(3);
	go_ss(4);
	go_ss(5);
	go_ss(6);
	go_ss(7);
	go_ss(8);
	go_ss(9);
	go_ss(10);
	go_ss(11);
	go_ss(12);
	go_ss(13);
	go_ss(14);
	go_ss(15);
	go_ss(16);
	go_ss(17);
	go_ss(18);
}

template <uint32_t XN, typename T>
void go_e(uint32_t code, T* pxx, uint32_t* stack, uint32_t dp) {
	if (dp > XN) {
		pxx[code]++;
		return;
	}


	uint32_t tmp_code = 0;

#	define _set_eo_(i)																				\
		if (!_cft_(stack, rx[i])) {																	\
			_c3_(_eo_, i, _)(tmp_code, code);														\
			*stack = rx[i];																			\
			go_e<XN>(tmp_code, pxx, stack + 1, dp + 1);												\
		}

	_set_eo_(1);
	_set_eo_(2);
	_set_eo_(3);
	_set_eo_(4);
	_set_eo_(5);
	_set_eo_(6);
	_set_eo_(7);
	_set_eo_(8);
	_set_eo_(9);
	_set_eo_(10);
	_set_eo_(11);
	_set_eo_(12);
	_set_eo_(13);
	_set_eo_(14);
	_set_eo_(15);
	_set_eo_(16);
	_set_eo_(17);
	_set_eo_(18);

#		undef  _set_1_
#		undef  _set_2_
#		undef  _set_3_
}

#define GO go_c

uint32_t xxx_2() {
	VPRM = x_vprm();
	VORM = x_vorm();
	EPRM = x_eprm();
	EORM = x_eorm();

	uint8_t* buf = 0;
	const uint64_t BSN = (_XN_7_ + _XN_6_ + _XN_5_ + _XN_4_ + _XN_3_ + _XN_2_ + _XN_1_) * CBN;
	try {
		buf = new uint8_t[BSN];
	} catch(std::bad_alloc& bd) {
		printf("error: %s\n", bd.what());
	}

	uint32_t _stack[12] = { 0 };
	uint32_t* stack = &_stack[2];

	uint8_t* px = buf;

	uint64_t* pxx = (uint64_t*)buf;
	uint32_t c25 = 1 << 25;
	for (uint32_t i = 0; i < c25; i++) { pxx[i] = 0; }

	time_count tc_a;
	tc_a.start();
	uint32_t code = 0;
	GO<8>(code, pxx, stack, 1);
	tc_a.end("x ");

	uint64_t cnt = 0;
	for (uint32_t i = 0; i < c25; i++) { cnt += pxx[i]; }
	printf("cnt: %lu\n", cnt);

#	if 1
//	std::sort(pxx, pxx + c25, cmpr);
	for (uint32_t i = 0; i < c25; i++) {
		if (pxx[i]) {
			printf("%u - %lu\n", i, (uint64_t)pxx[i]);
		}
	}
#	endif
}

template <uint32_t XN, typename T>
void go_f(uint32_t code, T* pxx, uint32_t* stack, uint32_t dp) {
	if (dp > XN) {
		pxx[code]++;
		return;
	}

	uint32_t tmp_code;
#	define _set_vo_(i)																				\
		if (!_cft_(stack, rx[i])) {																	\
			_c3_(_vo_, i, _)(tmp_code, code);														\
			*stack = rx[i];																			\
			go_f<XN>(tmp_code, pxx, stack + 1, dp + 1);												\
		}

	_set_vo_(1);
	_set_vo_(2);
	_set_vo_(3);
	_set_vo_(4);
	_set_vo_(5);
	_set_vo_(6);
	_set_vo_(7);
	_set_vo_(8);
	_set_vo_(9);
	_set_vo_(10);
	_set_vo_(11);
	_set_vo_(12);
	_set_vo_(13);
	_set_vo_(14);
	_set_vo_(15);
	_set_vo_(16);
	_set_vo_(17);
	_set_vo_(18);

}

uint32_t xxx_3() {
	VPRM = x_vprm();
	VORM = x_vorm();
	EPRM = x_eprm();
	EORM = x_eorm();

	uint8_t* buf = 0;
	const uint64_t BSN = (_XN_7_ + _XN_6_ + _XN_5_ + _XN_4_ + _XN_3_ + _XN_2_ + _XN_1_) * CBN;
	try {
		buf = new uint8_t[BSN];
	} catch(std::bad_alloc& bd) {
		printf("error: %s\n", bd.what());
	}

	uint32_t _stack[12] = { 0 };
	uint32_t* stack = &_stack[2];

	uint8_t* px = buf;

	uint64_t* pxx = (uint64_t*)buf;
	uint32_t c25 = 1 << 25;
	for (uint32_t i = 0; i < c25; i++) { pxx[i] = 0; }

	time_count tc_a;
	tc_a.start();
	uint32_t code = (1 << 16) - 1;
	go_f<8>(code, pxx, stack, 1);
	tc_a.end("x ");

	uint64_t cnt = 0;
	for (uint32_t i = 0; i < c25; i++) { cnt += pxx[i]; }
	printf("cnt: %lu\n", cnt);

#	if 1
#	define _vertex_ort_code_to_idx(code)		\
			(									\
			 ((code      ) & 0x03) * 2187 +		\
			 ((code >>  2) & 0x03) * 729  +		\
			 ((code >>  4) & 0x03) * 243  +		\
			 ((code >>  6) & 0x03) * 81   +		\
			 ((code >>  8) & 0x03) * 27   +		\
			 ((code >> 10) & 0x03) * 9    +		\
			 ((code >> 12) & 0x03) * 3    +		\
			 ((code >> 14) & 0x03)				\
			)
	uint64_t pzz[65536] = { 0 };
	for (uint64_t i = 0; i < c25; i++) {
		if (pxx[i]) {
			uint64_t x = ~i;
			x = ( x & 0x0003)        |
				( x & 0x000C)        |
				((x & 0x0030) << 6)  |
				( x & 0x00C0)        |
				((x & 0x0300) <<  4) |
				((x & 0x0C00) <<  4) |
				((x & 0x3000) >>  4) |
				((x & 0xC000) >> 10)
				;
			x = _vertex_ort_code_to_idx(x);
			pzz[x] = (x << 32) | pxx[i];
		}
	}
	std::sort(pzz, pzz + 65536, cmpr);
	for (uint32_t i = 0; i < 65536; i++) {
		if ((uint32_t)pzz[i]) {
			printf("%u - %u\n", (uint32_t)(pzz[i] >> 32), (uint32_t)pzz[i]);
		}
	}
#	endif
}

uint32_t xxx() {
	return xxx_3();
}
