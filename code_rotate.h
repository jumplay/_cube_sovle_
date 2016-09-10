#pragma once
#include <stdint.h>

void create_rotation_map();

uint32_t vertex_pos_code_to_idx(uint64_t code);
uint64_t vertex_pos_idx_to_code(uint32_t idx);

uint32_t vertex_ort_code_to_idx(uint64_t code);
uint64_t vertex_ort_idx_to_code(uint32_t idx);

uint64_t vertex_pos_rotate(uint64_t idx, uint32_t rx);
uint64_t vertex_ort_rotate(uint64_t idx, uint32_t rx);

uint64_t edge_pos_rotate(uint64_t code, uint32_t rx);
uint32_t edge_ort_rotate(uint32_t code, uint32_t rx);
