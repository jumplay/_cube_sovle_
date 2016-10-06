#include <stdio.h>
#include <stdint.h>
#include <time.h>

static void print_vertex(const uint8_t* code) {
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

static void print_edge(const uint8_t* code) {

	printf("\t    ___%2u____\n", code[8]);
	printf("\t   /|       /|\n");
	printf("\t %2u %-2u    %2u %-2u\n", code[4], code[9], code[1], code[3]);
	printf("\t /__|_%-2u__/  |\n", code[2]);
	printf("\t |  |___%2u|__|\n", code[11]);
	printf("\t%2u  /     %-2u /\n", code[6], code[0]);
	printf("\t | %-2u     |%2u\n", code[10], code[7]);
	printf("\t |/__%2u___|/\n", code[5]);
}

struct time_count {
	timespec _start;
	timespec _end;

	void start() { clock_gettime(CLOCK_MONOTONIC, &_start); }
	void end() { clock_gettime(CLOCK_MONOTONIC, &_end); }
	void end(const char* s) {
		clock_gettime(CLOCK_MONOTONIC, &_end);
		double t_s = ((double)(_end.tv_nsec - _start.tv_nsec) / 1000000 + (_end.tv_sec - _start.tv_sec) * 1000) / 1000;
		printf("%s%.3f\n", s, t_s);
	}
};

void get_optimal_8();
void get_optimal_12();

uint32_t voc_x2o(uint32_t x);
uint32_t eoc_x2o(uint32_t x);
uint32_t vpc_x2o(uint32_t x);
uint64_t epc_x2o(uint64_t x);
