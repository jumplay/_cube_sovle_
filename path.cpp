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

#define _c3_(x,y,z) x##y##z

#define _cft_(stack,x)																			\
				(																				\
				 (stack[-1] >> 2) == (x >> 2) ||												\
				 ((stack[-1] >> 2) + (stack[-2] >> 2) == 7 && (stack[-2] >> 2) == (x >> 2))		\
				)

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

bool cmpr(uint64_t a, uint64_t b) {
	return a < b;
}

template <uint32_t XN>
void go_0(ccd_t code, uint8_t*& p_data, uint32_t* stack, uint32_t dp) {
	if (dp > XN) {
		code.code(p_data);
		p_data += 12;
		return;
	}


#	define go_ss(i)													\
			if (!_cft_(stack, rx[i])) {								\
				*stack = rx[i];										\
				ccd_t tmp_code(code);								\
				tmp_code.one_step(rx[i]);							\
				go_0<XN>(tmp_code, p_data, stack + 1, dp + 1);		\
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


uint32_t xxx_0() {
#	if 0
	uint8_t px[12];
	ccd_t ccd;
	ccd.one_step(rx[1]);
	//ccd.one_step(rx[4]);
	//ccd.one_step(rx[1]);
	//ccd.one_step(rx[4]);
	//ccd.one_step(rx[1]);
	//ccd.one_step(rx[4]);

	ccd.code(px);
	//for (uint32_t i = 0; i < 12; i++) {
	//	printf("%u ", (uint32_t)px[i]);
	//}

	printf(">%lu\n", (*(uint64_t*)(px + 6)) & 0xFFFFFFFFFFFFul);

	puts("");

	return 0;
#	endif

	uint8_t* buf_1 = 0;
	uint8_t* buf_2 = 0;
	const uint64_t bsz = 1 << 30;
	try {
		buf_1 = new uint8_t[bsz];
		buf_2 = new uint8_t[bsz];
	} catch(std::bad_alloc& bd) {
		printf("error: %s\n", bd.what());
	}

	uint32_t _stack[12] = { 0 };
	uint32_t* stack = &_stack[2];

	uint8_t* p_data = buf_1;
	uint8_t* p_tmp = p_data;

	time_count tc_a;
	tc_a.start();
	ccd_t code;
	go_0<6>(code, p_tmp, stack, 1);
	tc_a.end("x ");

	uint64_t cnt = (p_tmp - p_data) / 12;
	printf("cnt: %lu\n", cnt);

#	if 0
	uint8_t* p_rw = p_data;
	for (; p_rw < p_tmp; p_rw += 12) {
		for (uint32_t i = 0; i < 6; i++) {
			p_rw[i] = 0;
		}
	}
#	endif

#	if 1
	uint8_t* sort_buf = buf_2;
	radix_sort<12>(p_data, sort_buf, cnt);

	for (uint32_t j = 0; j < 12; j++) {
		printf("%u ", (uint32_t)p_data[j]);
	}
	puts("");

	uint32_t cnt_new = 1;
	for (uint32_t i = 1; i < cnt; i++) {
		if (memcmp(p_data + (i - 1) * 12, p_data + i * 12, 12)) {
			uint8_t* p = p_data + i * 12;
			for (uint32_t j = 0; j < 12; j++) {
				printf("%u ", (uint32_t)p[j]);
			}
			puts("");
			cnt_new++;
		}
	}
	printf("cnt_new: %u\n", cnt_new);

#	endif
}

template <uint32_t XN, typename T>
void go_xvo(uint32_t code, T* pxx, uint32_t* stack, uint32_t dp) {
	if (dp > XN) {
		pxx[code]++;
		return;
	}

	uint32_t tmp_code;
#	define _set_vo_(i)																				\
		if (!_cft_(stack, rx[i])) {																	\
			_c3_(_vo_, i, _)(tmp_code, code);														\
			*stack = rx[i];																			\
			go_xvo<XN>(tmp_code, pxx, stack + 1, dp + 1);											\
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

template <uint32_t XN, typename T>
void go_n_xeo(uint32_t code, T* pxx, uint32_t* stack, uint32_t dp) {
	if (dp > XN) {
		//pxx[code]++;
		return;
	}


	uint32_t tmp_code = 0;

#	if 0
#	define _set_eo_(i)																				\
		if (!_cft_(stack, rx[i])) {																	\
			_c3_(_eo_, i, _)(tmp_code, code);														\
			*stack = rx[i];																			\
			go_xeo<XN>(tmp_code, pxx, stack + 1, dp + 1);											\
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

#	else
#	define _set_eo_(i,s)																			\
		do {																						\
			_c3_(_eo_, i, _)(tmp_code, code);														\
			*stack = s;																				\
			go_n_xeo<XN>(tmp_code, pxx, stack, dp + 1);												\
		} while (0)

	switch (*stack) {
		case 0 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 1 :
			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 2 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;

		case 3 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 4 :
			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 5 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 6 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);
			break;
	}
#	endif
#	undef  _set_eo_
}

uint32_t xxx_3() {

	uint8_t* buf_1 = 0;
	uint8_t* buf_2 = 0;
	const uint64_t bsz = 1 << 30;
	try {
		buf_1 = new uint8_t[bsz];
		buf_2 = new uint8_t[bsz];
	} catch(std::bad_alloc& bd) {
		printf("error: %s\n", bd.what());
	}

	uint32_t _stack[12] = { 0 };
	uint32_t* stack = &_stack[2];

	uint64_t* pxx = (uint64_t*)buf_1;
	uint32_t c25 = 1 << 25;
	for (uint32_t i = 0; i < c25; i++) { pxx[i] = 0; }

	time_count tc_a;
	tc_a.start();
	uint32_t code = 0;
	go_n_xeo<6>(code, pxx, stack, 1);
	tc_a.end("x ");

	uint64_t cnt = 0;
	for (uint32_t i = 0; i < c25; i++) { cnt += pxx[i]; }
	printf("cnt: %lu\n", cnt);

#	if 0
	uint64_t pzz[4096] = { 0 };
	for (uint64_t i = 0; i < c25; i++) {
		if (pxx[i]) {
			uint64_t x = eoc_x2o((uint32_t)i);

			pzz[x] = (x << 32) | pxx[i];
		}
	}
	std::sort(pzz, pzz + 4096, cmpr);
	for (uint32_t i = 0; i < 4096; i++) {
		if ((uint32_t)pzz[i]) {
			printf("%u - %u\n", (uint32_t)(pzz[i] >> 32), (uint32_t)pzz[i]);
		}
	}
#	endif
}

template <uint32_t XN>
void go_v_xeo(uint8_t* code, uint32_t last_step, uint32_t dp, uint8_t*& p_data) {

#	define _cbn_	13
	if (dp > XN) {
		memcpy(p_data, code , 13);
		p_data += _cbn_;
		//(*(uint64_t*)p_data)++;
		return;
	}

	static uint8_t tmp[16];

	uint32_t voc = *(uint32_t*)code;
	uint32_t eoc = *(uint32_t*)(code + 2);
	uint32_t vpc = *(uint32_t*)(code + 4);
	uint64_t epc = *(uint64_t*)(code + 7);

			//																						\
			_c3_(_vo_, i, _)(*(uint32_t*)tmp, voc);													\
			_c3_(_eo_, i, _)(*(uint32_t*)(tmp + 2), eoc);											\
			_c3_(_vp_, i, _)(*(uint32_t*)(tmp + 4), vpc);											\
			_c3_(_ep_, i, _)(*(uint64_t*)(tmp + 7), epc);											\

#	define _set_eo_(i,s)																			\
		do {																						\
			_c3_(_vo_, i, _)(*(uint32_t*)tmp, voc);													\
			_c3_(_eo_, i, _)(*(uint32_t*)(tmp + 2), eoc);											\
			_c3_(_vp_, i, _)(*(uint32_t*)(tmp + 4), vpc);											\
			_c3_(_ep_, i, _)(*(uint64_t*)(tmp + 7), epc);											\
			go_v_xeo<XN>(tmp, s, dp + 1, p_data);													\
		} while (0)

	switch (last_step) {
		case 0 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 1 :
			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 2 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;

		case 3 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 4 :
			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 5 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(7,  3);
			_set_eo_(8,  3);
			_set_eo_(9,  3);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(16, 6);
			_set_eo_(17, 6);
			_set_eo_(18, 6);
			break;
		case 6 :
			_set_eo_(1,  1);
			_set_eo_(2,  1);
			_set_eo_(3,  1);

			_set_eo_(4,  2);
			_set_eo_(5,  2);
			_set_eo_(6,  2);

			_set_eo_(10, 4);
			_set_eo_(11, 4);
			_set_eo_(12, 4);

			_set_eo_(13, 5);
			_set_eo_(14, 5);
			_set_eo_(15, 5);
			break;
	}
#	undef  _set_eo_
}

uint32_t xxx_4() {
#	if 0
	uint8_t cd_0[16];
	uint8_t cd_1[16];
	uint8_t* cd_a = &cd_0[0];
	uint8_t* cd_b = &cd_1[0];
	*(uint32_t*)&cd_a[0] = 0xFFFF;
	*(uint32_t*)&cd_a[2] = 0;
	*(uint32_t*)&cd_a[4] = 076543210;
	*(uint64_t*)&cd_a[7] = 0xBA9876543210ul;

#	define _step_(i)																\
		do {																		\
			_c3_(_vo_, i, _)(*(uint32_t*)cd_b, (*(uint32_t*)(cd_a)));				\
			_c3_(_eo_, i, _)(*(uint32_t*)(cd_b + 2), (*(uint32_t*)(cd_a + 2)));		\
			_c3_(_vp_, i, _)(*(uint32_t*)(cd_b + 4), (*(uint32_t*)(cd_a + 4)));		\
			_c3_(_ep_, i, _)(*(uint64_t*)(cd_b + 7), (*(uint64_t*)(cd_a + 7)));		\
			memcpy(cd_a, cd_b, 13);													\
		} while (0)

	_step_(1);
	//_step_(4);
	//_step_(1);
	//_step_(4);
	//_step_(1);
	//_step_(4);

	*(uint32_t*)(cd_b + 0) = voc_x2o(*(uint32_t*)(cd_a + 0));
	*(uint32_t*)(cd_b + 2) = eoc_x2o(*(uint32_t*)(cd_a + 2));
	*(uint32_t*)(cd_b + 4) = vpc_x2o(*(uint32_t*)(cd_a + 4));
	*(uint64_t*)(cd_b + 6) = epc_x2o(*(uint64_t*)(cd_a + 7));
	memcpy(cd_a, cd_b, 12);
	//for (uint32_t i = 0; i < 12; i++) {
	//	printf("%u ", (uint32_t)cd_a[i]);
	//}
	printf(">%lu\n", (*(uint64_t*)(cd_a + 6)) & (0xFFFFFFFFFFFF));
	puts("");


	return 0;
#	endif

	uint8_t* buf_1 = 0;
	uint8_t* buf_2 = 0;
	const uint64_t bsz = 1 << 30;
	try {
		buf_1 = new uint8_t[bsz];
		buf_2 = new uint8_t[bsz];
	} catch(std::bad_alloc& bd) {
		printf("error: %s\n", bd.what());
	}

	uint8_t code[16];
	*(uint32_t*)&code[0] = 0xFFFF;
	*(uint32_t*)&code[2] = 0;
	*(uint32_t*)&code[4] = 076543210;
	*(uint64_t*)&code[7] = 0xBA9876543210ul;

	uint8_t* p_data = buf_1;
	uint8_t* p_tmp = p_data;

	time_count tc_a;
	tc_a.start();
	*p_tmp = 0;
	go_v_xeo<6>(code, 0, 1, p_tmp);
	tc_a.end("x ");

	uint64_t cnt = (p_tmp - p_data) / 13;
	//uint64_t cnt = *(uint64_t*)p_data;

	printf("cnt: %lu\n", cnt);

#	if 1
	uint8_t* p_w = p_data;
	uint8_t* p_r = p_data;
	for (; p_r < p_tmp; p_r += _cbn_) {
		uint8_t _buf[16];
		uint8_t* buf = &_buf[0];
		*(uint32_t*)buf = voc_x2o(*(uint32_t*)p_r);
		*(uint32_t*)(buf + 2) = eoc_x2o(*(uint32_t*)(p_r + 2));
		*(uint32_t*)(buf + 4) = vpc_x2o(*(uint32_t*)(p_r + 4));
		*(uint64_t*)(buf + 6) = epc_x2o(*(uint64_t*)(p_r + 7));
		memcpy(p_w, buf, 12);

#		if 0
		if (*(uint32_t*)p_w == ((64) | (35 << 8) | (40 << 16) | (0 << 24))) {
			printf(">%u\n", (uint32_t)(p_w - p_data) / 12);
		}
		for (uint32_t j = 0; j < 12; j++) {
			printf("%u ", (uint32_t)p_w[j]);
		}
		puts("");
		getchar();
#		endif

		p_w += 12;
	}

#	if 0
	uint8_t* p_rw = p_data;
	for (; p_rw < p_tmp; p_rw += 12) {
		for (uint32_t i = 0; i < 6; i++) {
			p_rw[i] = 0;
		}
	}
#	endif

	uint8_t* sort_buf = buf_2;
	radix_sort<12>(p_data, sort_buf, cnt);

	for (uint32_t j = 0; j < 12; j++) {
		printf("%u ", (uint32_t)p_data[j]);
	}
	puts("");

	uint32_t cnt_new = 1;
	for (uint32_t i = 1; i < cnt; i++) {
		if (memcmp(p_data + (i - 1) * 12, p_data + i * 12, 12)) {
			uint8_t* p = p_data + i * 12;
			for (uint32_t j = 0; j < 12; j++) {
				printf("%u ", (uint32_t)p[j]);
			}
			puts("");
			cnt_new++;
		}
	}
	printf("cnt_new: %u\n", cnt_new);

#	endif
}

uint32_t xxx() {
	return xxx_0();
}
