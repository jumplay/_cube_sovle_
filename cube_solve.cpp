//
//						z  -x
//	                    | /
//	                    |/
//	             -y ____|____ y
//	                   /|
//	                  / |
//                   x  |
//	                    -z
//
//	vertex code order: <0, xyz> <1, -xyz> <2, x-yz> <3, xy-z> <4, -x-yz> <5, -xy-z> <6, x-y-z> <7, -x-y-z>
//
//	                 4_________1
//	                 /|       /|
//	                / |      / |
//	              2/__|____0/  |
//	               |  |_____|__|
//	               |  /7    |  /5		
//	               | /      | /		
//	               |/_______|/		
//	               6         3
//
//
//	edge code order: <0, xy> <1, yz> <2, zx> <3, -xy> <4, -yz> <5, -zx > <6, x-y> <7, y-z> <8, z-x> <9, -x-y> <10, -y-z> <11, -z-x>
//
//	                  ____8____
//	                 /|       /|
//	                4 9      1 3
//	               /____2___/  |
//	               |  |___11|__|
//	               6  /     0  /		
//	               | 10     | 7		
//	               |/___5___|/		
//	                        
//
//
//
//	rotation code:
//			bit - 10001: (x anticlockwise rotate once)
//	                  4_________1             4_________1
//	                  /|       /|             /|       /|
//	                 / |      / |            / |      / |
//	               2/__|____0/  |          0/__|____3/  |
//	                |  |_____|__|    -->    |  |_____|__|
//	                |  /7    |  /5	        |  /7    |  /5		
//	                | /      | /            | /      | /		
//	                |/_______|/             |/_______|/		
//	                6         3             2         6
//			bit - 10010: (x anticlockwise rotate twice)
//			bit - 10011: (x anticlockwise rotate thrice)
//
//			bit - 01001: (y anticlockwise rotate once)
//	                  4_________1             4_________5
//	                  /|       /|             /|       /|
//	                 / |      / |            / |      / |
//	               2/__|____0/  |          2/__|____1/  |
//	                |  |_____|__|    -->    |  |_____|__|
//	                |  /7    |  /5	        |  /7    |  /3		
//	                | /      | /            | /      | /		
//	                |/_______|/             |/_______|/		
//	                6         3             6         0
//			bit - 01010: (y anticlockwise rotate twice)
//			bit - 01011: (y anticlockwise rotate thrice)
//
//			bit - 00101: (z anticlockwise rotate once)
//	                  4_________1             1 ________0
//	                  /|       /|             /|       /|
//	                 / |      / |            / |      / |
//	               2/__|____0/  |          4/__|____2/  |
//	                |  |_____|__|    -->    |  |_____|__|
//	                |  /7    |  /5	        |  /7    |  /5		
//	                | /      | /            | /      | /		
//	                |/_______|/             |/_______|/		
//	                6         3             6         3
//			bit - 00110: (z anticlockwise rotate twice)
//			bit - 00111: (z anticlockwise rotate thrice)
//
//			bit - 01101: (-x anticlockwise rotate once)
//	                  4_________1             7_________4
//	                  /|       /|             /|       /|
//	                 / |      / |            / |      / |
//	               2/__|____0/  |          2/__|____0/  |
//	                |  |_____|__|    -->    |  |_____|__|
//	                |  /7    |  /5	        |  /5    |  /1		
//	                | /      | /            | /      | /		
//	                |/_______|/             |/_______|/		
//	                6         3             6         3
//			bit - 01110: (-x anticlockwise rotate twice)
//			bit - 01111: (-x anticlockwise rotate thrice)
//
//			bit - 10101: (-y anticlockwise rotate once)
//	                  4_________1             2_________1
//	                  /|       /|             /|       /|
//	                 / |      / |            / |      / |
//	               2/__|____0/  |          6/__|____0/  |
//	                |  |_____|__|    -->    |  |_____|__|
//	                |  /7    |  /5	        |  /4    |  /5		
//	                | /      | /            | /      | /		
//	                |/_______|/             |/_______|/		
//	                6         3             7         3
//			bit - 10110: (-y anticlockwise rotate twice)
//			bit - 10111: (-y anticlockwise rotate thrice)
//					
//			bit - 11001: (-z anticlockwise rotate once)
//	                  4_________1             4_________1
//	                  /|       /|             /|       /|
//	                 / |      / |            / |      / |
//	               2/__|____0/  |          2/__|____0/  |
//	                |  |_____|__|    -->    |  |_____|__|
//	                |  /7    |  /5	        |  /6    |  /7		
//	                | /      | /            | /      | /		
//	                |/_______|/             |/_______|/		
//	                6         3             3         5
//			bit - 11010: (-z anticlockwise rotate twice)
//			bit - 11011: (-z anticlockwise rotate thrice)
//

#include "cube_solve.h"
#include <stdio.h>
#include <stdint.h>
#include "tools.h"

#define V_N 8			// vertex count
#define E_N 12			// edge count

#define _concatenate_3(a,b,c)	a##b##c
#define _concatenate_2(a,b)		a##b

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

//////////////////////////////////////////////////////
static uint64_t*& vertex_pos_code_map() {
	static uint64_t* _vpcm = 0;
	return _vpcm;
}
#define _VPCM vertex_pos_code_map()
#define VPCM ((const uint64_t*)_VPCM)

static uint32_t*& vertex_pos_rotation_map() {
	static uint32_t* _vprm = 0;
	return _vprm;
}
#define _VPRM vertex_pos_rotation_map()
#define VPRM ((const uint32_t*)_VPRM)
//////////////////////////////////////////////////////
static uint64_t*& vertex_ort_code_map() {
	static uint64_t* _vocm = 0;
	return _vocm;
}
#define _VOCM vertex_ort_code_map()
#define VOCM ((const uint64_t*)_VOCM)

static uint32_t*& vertex_ort_rotation_map() {
	static uint32_t* _vorm = 0;
	return _vorm;
}
#define _VORM vertex_ort_rotation_map()
#define VORM ((const uint32_t*)_VORM)
//////////////////////////////////////////////////////
static uint64_t*& edge_pos_rotation_map() {
	static uint64_t* _eprm = 0;
	return _eprm;
}
#define _EPRM edge_pos_rotation_map()
#define EPRM ((const uint64_t*)_EPRM)
//////////////////////////////////////////////////////
static uint32_t*& edge_ort_rotation_map() {
	static uint32_t* _eorm = 0;
	return _eorm;
}
#define _EORM edge_ort_rotation_map()
#define EORM ((const uint32_t*)_EORM)
//////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//vertex_pos

// create permutation of [0, total), save in vp_code.
template <uint32_t total, uint32_t shift>
void permutation(uint64_t* vp_code, uint64_t value, uint32_t& idx, uint32_t depth, uint32_t *exclude) {
	for (uint32_t i = 0; i < total; i++) {
		bool excluded = false;
		for (uint32_t j = 0; j < depth && !excluded; j++) {
			excluded = (i == exclude[j]);
		}
		if (excluded) continue;
		if (depth + 1 == total) {
			vp_code[idx++] = value + (uint64_t(i) << (shift * depth));
		} else {
			exclude[depth] = i;
			permutation<total, shift>(vp_code, value + (uint64_t(i) << (shift * depth)), idx, depth + 1, exclude);
		}
	}
}

void create_vertex_pos_code_map() {
	uint32_t idx = 0;
	_VPCM = new uint64_t[factorial(8)];
	uint32_t exclude[8];
	permutation<V_N, 8>(_VPCM, 0, idx, 0, exclude);

#	if 0
	printf("idx: %u\n", idx);
	for (uint32_t i = 0; i < factorial(8); i++) {
		uint8_t* code_8 = (uint8_t*)&_VPCM[i];
		for (uint32_t j = 0; j < 8; j++) {
			uint32_t x = code_8[j];
			printf("\t%u, ", x);
		}
		printf("\n");
	}
#	endif
}

uint32_t vertex_pos_code_to_idx(uint64_t code) {
	uint32_t idx = 0;
	uint8_t * code_8 = (uint8_t*)&code;
	
#	define _c_(x,y) (code_8[x] > code_8[y] ? 1 : 0)
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

uint32_t* create_vertex_pos_rotation_map_x() {
	uint32_t cnt = factorial(8);
	uint32_t *X_VPRM = new uint32_t[cnt << 5];
	
	uint64_t code;
	uint8_t* c_p[8];
	uint8_t tmp;
	uint32_t idx;
	for (uint32_t i = 0; i < 8; i++) {
		c_p[i] = ((uint8_t*)& code) + i;
	}

	for (uint32_t i = 0; i < cnt; i++) {
#		define _set_1_(x,a,b,c,d)										\
					do {												\
						code = VPCM[i];									\
						tmp = *c_p[a];									\
						*c_p[a] = *c_p[d];								\
						*c_p[d] = *c_p[c];								\
						*c_p[c] = *c_p[b];								\
						*c_p[b] = tmp;									\
						idx = vertex_pos_code_to_idx(code);				\
						X_VPRM[(i << 5) | rotation_idx[x][0]] = idx;	\
					} while (0)

#		define _set_2_(x,a,b,c,d)										\
					do {												\
						code = VPCM[i];									\
						tmp = *c_p[a];									\
						*c_p[a] = *c_p[c];								\
						*c_p[c] = tmp;									\
						tmp = *c_p[b];									\
						*c_p[b] = *c_p[d];								\
						*c_p[d] = tmp;									\
						idx = vertex_pos_code_to_idx(code);				\
						X_VPRM[(i << 5) | rotation_idx[x][1]] = idx;	\
					} while (0)

#		define _set_3_(x,a,b,c,d)										\
					do {												\
						code = VPCM[i];									\
						tmp = *c_p[a];									\
						*c_p[a] = *c_p[b];								\
						*c_p[b] = *c_p[c];								\
						*c_p[c] = *c_p[d];								\
						*c_p[d] = tmp;									\
						idx = vertex_pos_code_to_idx(code);				\
						X_VPRM[(i << 5) | rotation_idx[x][2]] = idx;	\
					} while (0)

#		define _set_(x,a,b,c,d)								\
					do {									\
						_set_1_(x, a, b, c, d);				\
						_set_2_(x, a, b, c, d);				\
						_set_3_(x, a, b, c, d);				\
					} while (0)

		_set_(0, 0, 2, 6, 3);
		_set_(1, 0, 3, 5, 1);
		_set_(2, 0, 1, 4, 2);
		_set_(3, 7, 4, 1, 5);
		_set_(4, 7, 6, 2, 4);
		_set_(5, 7, 5, 3, 6);

#		undef  _set_
#		undef  _set_1_
#		undef  _set_2_
#		undef  _set_3_
	}
	return X_VPRM;
}

void create_vertex_pos_rotation_map() {

	// rotate x
	uint8_t map_10001[8] = { 2, 1, 6, 0, 4, 5, 3, 7 };
	uint8_t map_10010[8];
	uint8_t map_10011[8];

	// rotate y
	uint8_t map_01001[8] = { 3, 0, 2, 5, 4, 1, 6, 7 };
	uint8_t map_01010[8];
	uint8_t map_01011[8];

	// rotate z
	uint8_t map_00101[8] = { 1, 4, 0, 3, 2, 5, 6, 7 };
	uint8_t map_00110[8];
	uint8_t map_00111[8];
	
	// rotate -x
	uint8_t map_01101[8] = { 0, 5, 2, 3, 1, 7, 6, 4 };
	uint8_t map_01110[8];
	uint8_t map_01111[8];

	// rotate -y
	uint8_t map_10101[8] = { 0, 1, 4, 3, 7, 5, 2, 6 };
	uint8_t map_10110[8];
	uint8_t map_10111[8];

	// rotate -z
	uint8_t map_11001[8] = { 0, 1, 2, 6, 4, 3, 7, 5 };
	uint8_t map_11010[8];
	uint8_t map_11011[8];

#	define create_map(c,d,e,x)											\
			do {														\
				uint8_t buf_1[8];										\
				uint8_t buf_2[8];										\
				for (uint32_t i = 0; i < 8; i++) {						\
					buf_1[_concatenate_3(map_, c, d)[i]] = i;			\
				}														\
				for (uint32_t i = 0; i < 8; i++) {						\
					buf_2[_concatenate_3(map_, c, e)[i]] = buf_1[i];	\
				}														\
				for (uint32_t i = 0; i < 8; i++) {						\
					_concatenate_3(map_,c,x)[buf_2[i]] = i;				\
				}														\
			} while (0)

	create_map(100, 01, 01, 10);
	create_map(100, 10, 01, 11);
	
	create_map(010, 01, 01, 10);
	create_map(010, 10, 01, 11);

	create_map(001, 01, 01, 10);
	create_map(001, 10, 01, 11);

	create_map(011, 01, 01, 10);
	create_map(011, 10, 01, 11);

	create_map(101, 01, 01, 10);
	create_map(101, 10, 01, 11);

	create_map(110, 01, 01, 10);
	create_map(110, 10, 01, 11);

#	undef  create_map

#	define _rotate(code,c)											\
			do {													\
				uint8_t buf[8];										\
				uint8_t* code_8 = (uint8_t*)&code;					\
				for (uint32_t i = 0; i < 8; i++) {					\
					buf[_concatenate_2(map_, c)[i]] = code_8[i];	\
				}													\
				code = *(uint64_t*)&buf[0];							\
			} while (0)

#	if 0
#	define _print_map(c)											\
				do {												\
					printf("map_%s:\n", #c);						\
					for (uint32_t i = 0; i < 8; i++) {				\
						uint32_t x = _concatenate_2(map_, c)[i];	\
						printf("\t%u", x);							\
					}												\
					printf("\n");									\
				} while (0)
	
#	define _print_result(c)											\
				do {												\
					uint8_t code_8[8] = { 0, 1, 2, 3, 4, 5, 6, 7};	\
					print_vertex(code_8);							\
					uint64_t code = *(uint64_t*)&code_8[0];			\
					_rotate(code, c);								\
					print_vertex((uint8_t*)&code);					\
				} while (0)


	_print_map(10001);
	_print_result(10001);
	_print_map(10010);
	_print_result(10010);
	_print_map(10011);
	_print_result(10011);

	_print_map(01001);
	_print_result(01001);
	_print_map(01010);
	_print_result(01010);
	_print_map(01011);
	_print_result(01011);

	_print_map(00101);
	_print_result(00101);
	_print_map(00110);
	_print_result(00110);
	_print_map(00111);
	_print_result(00111);

	_print_map(01101);
	_print_result(01101);
	_print_map(01110);
	_print_result(01110);
	_print_map(01111);
	_print_result(01111);

	_print_map(10101);
	_print_result(10101);
	_print_map(10110);
	_print_result(10110);
	_print_map(10111);
	_print_result(10111);

	_print_map(11001);
	_print_result(11001);
	_print_map(11010);
	_print_result(11010);
	_print_map(11011);
	_print_result(11011);

#	undef  _print_map
#endif

	uint32_t cnt = factorial(8) << 5;
	_VPRM = new uint32_t[1<<22];

#	define _make_map(c,v)														\
				do {															\
					for (uint32_t i = 0; i < factorial(8); i++) {				\
						uint64_t code = VPCM[i];								\
						_rotate(code, c);										\
						_VPRM[(i << 5) | (v)] = vertex_pos_code_to_idx(code);	\
					}															\
				} while (0)

	_make_map(10001, 16 + 1);
	_make_map(10010, 16 + 2);
	_make_map(10011, 16 + 3);

	_make_map(01001, 8 + 1);
	_make_map(01010, 8 + 2);
	_make_map(01011, 8 + 3);

	_make_map(00101, 4 + 1);
	_make_map(00110, 4 + 2);
	_make_map(00111, 4 + 3);

	_make_map(01101, 8 + 4 + 1);
	_make_map(01110, 8 + 4 + 2);
	_make_map(01111, 8 + 4 + 3);

	_make_map(10101, 16 + 4 + 1);
	_make_map(10110, 16 + 4 + 2);
	_make_map(10111, 16 + 4 + 3);

	_make_map(11001, 16 + 8 + 1);
	_make_map(11010, 16 + 8 + 2);
	_make_map(11011, 16 + 8 + 3);

#	undef  _make_map
#	undef  _rotate
}

/////////////////////////////////////////////////////////////////////////////////////////////
// vertex_ort

#define vertex_ort_code_to_idx(code)	( (code        & 0xFF) * 2187 +		\
										 ((code >>  8) & 0xFF) * 729  +		\
										 ((code >> 16) & 0xFF) * 243  +		\
										 ((code >> 24) & 0xFF) * 81   +		\
										 ((code >> 32) & 0xFF) * 27   +		\
										 ((code >> 40) & 0xFF) * 9    +		\
										 ((code >> 48) & 0xFF) * 3    +		\
										 ((code >> 56) & 0xFF)				\
										)

void create_vertex_ort_code_map() {
	uint64_t x_m[256];
	for (uint32_t i = 0; i < 256; i++) {
		x_m[i] = (((i & 0x0003) << 24) | ((i & 0x000C) << 14) | ((i & 0x0030) << 4) | ((i & 0x00C0) >> 6));
	}

	_VOCM = new uint64_t[6561];
	uint32_t idx = 0;
	
	for (uint32_t i = 0; i < (1 << 16); i++) {
		if (((i >> 1) & 0x5555) & (i & 0x5555)) { continue; }
		_VOCM[idx++] = ((x_m[i & 0x00FF] << 32) | x_m[i >> 8]);
	}
}

void create_vertex_ort_rotation_map() {
	// 00 - x
	// 01 - y
	// 10 - z
	
	uint8_t face[6] = { 0, 1, 2, 0, 1, 2 }; // x, y, z, -x, -y, -z
	uint32_t refer[6][4] = {
		{ 0, 2, 6, 3 },		// x
		{ 1, 0, 3, 5 },		// y
		{ 1, 4, 2, 0 },		// z
		{ 1, 5, 7, 4 },		// -x
		{ 2, 4, 7, 6 },		// -y
		{ 6, 7, 5, 3 }		// -z
	};

	_VORM = new uint32_t[6561 << 5];

	uint64_t code;
	uint8_t* code_8 = (uint8_t*)& code;
	uint8_t tmp;
	for (uint32_t i = 0; i < 6; i++) {
		uint8_t fc = face[i];
		uint8_t* code_0 = code_8 + refer[i][0];
		uint8_t* code_1 = code_8 + refer[i][1];
		uint8_t* code_2 = code_8 + refer[i][2];
		uint8_t* code_3 = code_8 + refer[i][3];
		for (uint32_t j = 0; j < 6561; j++) {
#			define _change_code_(x)														\
				do {																	\
					*code_##x = *code_##x == fc ? *code_##x : (~(*code_##x | fc)) & 3;	\
				} while (0)

			// 01
			code = VOCM[j];
			
			do {
				_change_code_(0);
				_change_code_(1);
				_change_code_(2);
				_change_code_(3);

				tmp = *code_0;
				*code_0 = *code_3;
				*code_3 = *code_2;
				*code_2 = *code_1;
				*code_1 = tmp;

				_VORM[(j << 5) | rotation_idx[i][0]] = vertex_ort_code_to_idx(code);
			} while (0);

			// 10
			code = VOCM[j];
			do {
				tmp = *code_0;
				*code_0 = *code_2;
				*code_2 = tmp;
				tmp = *code_1;
				*code_1 = *code_3;
				*code_3 = tmp;

				_VORM[(j << 5) | rotation_idx[i][1]] = vertex_ort_code_to_idx(code);
			} while (0);

			// 11
			code = VOCM[j];
			do {
				_change_code_(0);
				_change_code_(1);
				_change_code_(2);
				_change_code_(3);

				tmp = *code_0;
				*code_0 = *code_1;
				*code_1 = *code_2;
				*code_2 = *code_3;
				*code_3 = tmp;

				_VORM[(j << 5) | rotation_idx[i][2]] = vertex_ort_code_to_idx(code);
			} while (0);
#			undef  _change_code_
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//edge pos

void create_edge_pos_rotation_map() {
	_EPRM = new uint64_t[1 << 21];
	uint16_t code16;
	uint32_t code32;
	uint8_t* code_0 = (uint8_t*)& code32;
	uint8_t* code_1 = code_0 + 1;
	uint8_t* code_2 = code_0 + 2;
	uint8_t* code_3 = code_0 + 3;
	uint8_t tmp;

	for (uint32_t i = 0; i < (1 << 16); i++) {
		uint32_t x = (i & 0x000F) | ((i & 0x00F0) << 4) | ((i & 0x0F00) << 8) | ((i & 0xF000) << 12);

#		define _set_map_(x,a,b,c,d)							\
				do {										\
					_EPRM[(code16 << 5) | x] =				\
						((uint64_t)*code_0 << (4 * a))	|	\
						((uint64_t)*code_1 << (4 * b))	|	\
						((uint64_t)*code_2 << (4 * c))	|	\
						((uint64_t)*code_3 << (4 * d))		\
						;									\
				} while (0)
		
		do {
			code32 = x;
			code16 = (uint16_t)*code_0 | ((uint16_t)*code_1 << 4) | ((uint16_t)*code_2 << 8) | ((uint16_t)*code_3 << 12);

			tmp = *code_0;
			*code_0 = *code_3;
			*code_3 = *code_2;
			*code_2 = *code_1;
			*code_1 = tmp;

			_set_map_(rotation_idx[0][0],  0,  2,  6,  5);
			_set_map_(rotation_idx[1][0],  0,  7,  3,  1);
			_set_map_(rotation_idx[2][0],  2,  1,  8,  4);
			_set_map_(rotation_idx[3][0],  3, 11,  9,  8);
			_set_map_(rotation_idx[4][0],  4,  9, 10,  6);
			_set_map_(rotation_idx[5][0],  5, 10, 11,  7);

		} while (0);

		do {
			code32 = x;
			code16 = (uint16_t)*code_0 | ((uint16_t)*code_1 << 4) | ((uint16_t)*code_2 << 8) | ((uint16_t)*code_3 << 12);

			tmp = *code_0;
			*code_0 = *code_2;
			*code_2 = tmp;
			tmp = *code_1;
			*code_1 = *code_3;
			*code_3 = tmp;

			_set_map_(rotation_idx[0][1],  0,  2,  6,  5);
			_set_map_(rotation_idx[1][1],  0,  7,  3,  1);
			_set_map_(rotation_idx[2][1],  2,  1,  8,  4);
			_set_map_(rotation_idx[3][1],  3, 11,  9,  8);
			_set_map_(rotation_idx[4][1],  4,  9, 10,  6);
			_set_map_(rotation_idx[5][1],  5, 10, 11,  7);
		} while (0);

		do {
			code32 = x;
			code16 = (uint16_t)*code_0 | ((uint16_t)*code_1 << 4) | ((uint16_t)*code_2 << 8) | ((uint16_t)*code_3 << 12);

			tmp = *code_0;
			*code_0 = *code_1;
			*code_1 = *code_2;
			*code_2 = *code_3;
			*code_3 = tmp;

			_set_map_(rotation_idx[0][2],  0,  2,  6,  5);
			_set_map_(rotation_idx[1][2],  0,  7,  3,  1);
			_set_map_(rotation_idx[2][2],  2,  1,  8,  4);
			_set_map_(rotation_idx[3][2],  3, 11,  9,  8);
			_set_map_(rotation_idx[4][2],  4,  9, 10,  6);
			_set_map_(rotation_idx[5][2],  5, 10, 11,  7);
		} while (0);
#		undef  _set_map_
	}
}

uint64_t edge_pos_rotate(uint64_t code, uint32_t rx) {
	uint64_t mask;
	uint32_t c32;
#	define _set_mask_(a,b,c,d)							\
			do {										\
				mask =									\
					~(									\
					 ((uint64_t)0xF << (a * 4)) |		\
					 ((uint64_t)0xF << (b * 4)) |		\
					 ((uint64_t)0xF << (c * 4)) |		\
					 ((uint64_t)0xF << (d * 4))			\
					);									\
			} while (0)
#	define _c32_(a,b,c,d)										\
			do {												\
				c32 =											\
					 (((code >> (a * 4)) & 0x0F))		|		\
					 (((code >> (b * 4)) & 0x0F) <<  4) |		\
					 (((code >> (c * 4)) & 0x0F) <<  8) |		\
					 (((code >> (d * 4)) & 0x0F) << 12)			\
					;											\
			} while (0)

	switch (rx>>2) {
		case 4: 
			_set_mask_(0,  2,  6,  5);
			_c32_(0,  2,  6,  5);
			break;
		case 2: 
			_set_mask_(0,  7,  3,  1);
			_c32_(0,  7,  3,  1);
			break;
		case 1: 
			_set_mask_(2, 1,  8,  4);
			_c32_(2, 1,  8,  4);
			break;
		case 3: 
			_set_mask_(3, 11,  9,  8);
			_c32_(3, 11,  9,  8);
			break;
		case 5: 
			_set_mask_(4,  9, 10,  6);
			_c32_(4,  9, 10,  6);
			break;
		case 6: 
			_set_mask_(5, 10, 11,  7);
			_c32_(5, 10, 11,  7);
			break;
	}
	return EPRM[(c32 << 5) | rx] | (code & mask);
#	undef  _set_mask
#	undef  _c32_
}

/////////////////////////////////////////////////////////////////////////////////////////////
//edge ort

void create_edge_ort_rotation_map() {
	_EORM = new uint32_t[1 << 17];

	uint32_t code;
	uint32_t mask;

	for (uint32_t i = 0; i < (1 << 12); i++) {
#		define _set_1_(x,a,b,c,d)																	\
					do{																				\
						code = i;																	\
						code =	((((code >> a) & 1) ^ 1) << b) |									\
								((((code >> b) & 1) ^ 1) << c) |									\
								((((code >> c) & 1) ^ 1) << d) |									\
								((((code >> d) & 1) ^ 1) << a) ;									\
						mask = (~((1 << a) | (1 << b) | (1 << c) | (1 << d))) & ((1 << 12) - 1);	\
						_EORM[(i << 5) | rotation_idx[x][0]] = code | mask;							\
					} while (0)

#		define _set_2_(x,a,b,c,d)																	\
					do{																				\
						code = i;																	\
						code =	(((code >> a) & 1) << c) |											\
								(((code >> c) & 1) << a) |											\
								(((code >> b) & 1) << d) |											\
								(((code >> d) & 1) << b) ;											\
						mask = (~((1 << a) | (1 << b) | (1 << c) | (1 << d))) & ((1 << 12) - 1);	\
						_EORM[(i << 5) | rotation_idx[x][1]] = code | mask;							\
					} while (0)

#		define _set_3_(x,a,b,c,d)																	\
					do{																				\
						code = i;																	\
						code =	((((code >> a) & 1) ^ 1) << d) |									\
								((((code >> b) & 1) ^ 1) << a) |									\
								((((code >> c) & 1) ^ 1) << b) |									\
								((((code >> d) & 1) ^ 1) << c) ;									\
						mask = (~((1 << a) | (1 << b) | (1 << c) | (1 << d))) & ((1 << 12) - 1);	\
						_EORM[(i << 5) | rotation_idx[x][2]] = code | mask;							\
					} while (0)

#		define _set_(x,a,b,c,d)											\
					do {												\
						_set_1_(x, a, b, c, d);							\
						_set_2_(x, a, b, c, d);							\
						_set_3_(x, a, b, c, d);							\
					} while (0)

		_set_(0,  0,  2,  6,  5);
		_set_(1,  0,  7,  3,  1);
		_set_(2,  2,  1,  8,  4);
		_set_(3,  3, 11,  9,  8);
		_set_(4,  4,  9, 10,  6);
		_set_(5,  5, 10, 11,  7);

#		undef  _set_
#		undef  _set_1_
#		undef  _set_2_
#		undef  _set_3_
	}
}

void create_rotation_map() {
	create_vertex_pos_code_map();
	create_vertex_pos_rotation_map();

	uint32_t* X_VPRM = create_vertex_pos_rotation_map_x();

#	if 0
	for (uint32_t i = 0; i < factorial(8); i++) {
		printf("%u\n", X_VPRM[(i << 5) | rotation_idx[0][0]]);
	}
#	endif

	for (uint32_t i = 0; i < factorial(8); i++) {
		uint32_t x = rotation_idx[5][2];
		if (X_VPRM[(i << 5) | x] != VPRM[(i << 5) | x]) {
			puts("------------------------------------------------");
			printf(">%u\n", i);
			uint64_t code = VPCM[i];
			uint64_t code_1 = VPCM[VPRM[(i << 5) | x]];
			uint64_t code_2 = VPCM[X_VPRM[(i << 5) | x]];

			print_vertex((uint8_t*) &code);
			print_vertex((uint8_t*) &code_1);
			print_vertex((uint8_t*) &code_2);
			
		}
	}
	
	create_vertex_ort_code_map();
	create_vertex_ort_rotation_map();

	create_edge_pos_rotation_map();
	create_edge_ort_rotation_map();

}

