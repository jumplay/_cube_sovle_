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
//	vertex code order: <0, xyz>, <1, -xyz>, <2, x-yz>, <3, xy-z>, <4, -x-yz>, <5, -xy-z>, <6, x-y-z>, <7, -x-y-z>
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
	{ 16 + 1, 16 + 2, 16 + 3 },
	{  8 + 1,  8 + 2,  8 + 3 },
	{  4 + 1,  4 + 2,  4 + 3 },
	{  8 + 4 + 1,  8 + 4 + 2,  8 + 4 + 3 },
	{ 16 + 4 + 1, 16 + 4 + 2, 16 + 4 + 3 },
	{ 16 + 8 + 1, 16 + 8 + 2, 16 + 8 + 3 },
};

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

void create_rotation_map() {
	create_vertex_pos_code_map();
	create_vertex_pos_rotation_map();

}
