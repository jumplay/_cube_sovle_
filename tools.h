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
