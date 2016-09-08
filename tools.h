#include <stdio.h>
#include <stdint.h>

void print_vertex(uint8_t* code) {
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

void print_edge(uint64_t code) {
	uint32_t code_12[12];
	for (uint32_t i = 0; i < 12; i++) {
		code_12[i] = code & 0x0F;
		code >>= 4;
	}

	printf("\t    ___%2u____\n", code_12[8]);
	printf("\t   /|       /|\n");
	printf("\t %2u %-2u    %2u %-2u\n", code_12[4], code_12[9], code_12[1], code_12[3]);
	printf("\t /__|_%-2u__/  |\n", code_12[2]);
	printf("\t |  |___%2u|__|\n", code_12[11]);
	printf("\t%2u  /     %-2u /\n", code_12[6], code_12[0]);
	printf("\t | %-2u     |%2u\n", code_12[10], code_12[7]);
	printf("\t |/__%2u___|/\n", code_12[5]);
}
