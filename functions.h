#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include "iostream"
#include "vector"

std::vector <unsigned int> crc(std::vector <unsigned int> const& input,const unsigned int crc_version);
unsigned int xor_with_1(const unsigned int a);
std::vector <unsigned int> stuff_bit_count(std::vector <unsigned int> const& input);

void print_table(std::vector <unsigned int> const& table);
void print_table_inverted(std::vector <unsigned int> const& table);

#endif // FONCTIONS_H_INCLUDED
