#include <stdio.h>
#include <stdint.h>

static uint8_t* edge_pos_64_2_12(uint64_t c, uint8_t* code) { 
	for (uint32_t i = 0; i < 12; i++) {
		code[i] = (c >> (i * 4)) & 0x0F;
	}

	return code;
}

static void print_vertex(uint8_t* code) {
	printf("\t   %u_________%u\n", (uint32_t)code[4], (uint32_t)code[1]);
	printf("\t   /|       /|\n");
	printf("\t  / |      / |\n");
	printf("\t%u/__|____%u/  |\n", (uint32_t)code[2], (uint32_t)code[0]);
	printf("\t |  |_____|__|\n");
	printf("\t |  /%u    |  /%u\n", (uint32_t)code[7], (uint32_t)code[5]);
	printf("\t | /      | /\n");
	printf("\t |/_______|/\n");
	printf("\t %u         %u\n", (uint32_t)code[6], (uint32_t)code[3]);
}

static void print_edge(uint8_t* code) {

	printf("\t    ___%2u____\n", code[8]);
	printf("\t   /|       /|\n");
	printf("\t %2u %-2u    %2u %-2u\n", code[4], code[9], code[1], code[3]);
	printf("\t /__|_%-2u__/  |\n", code[2]);
	printf("\t |  |___%2u|__|\n", code[11]);
	printf("\t%2u  /     %-2u /\n", code[6], code[0]);
	printf("\t | %-2u     |%2u\n", code[10], code[7]);
	printf("\t |/__%2u___|/\n", code[5]);
}
