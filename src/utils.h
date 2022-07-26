#pragma once

#include <cstdint>
#include <string>
#include <vector>

uint8_t get_register(std::string register_name);

uint64_t integer_to_hexint(std::string integer, int line_number);
uint64_t integer_to_hexint(uint64_t integer, int line_number);
uint64_t hexint_to_integer(uint64_t hexint);
std::vector<uint8_t> integer_to_bytes(std::string integer, int size);