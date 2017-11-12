#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include "iostream"
#include "vector"

std::vector <unsigned int> crc(std::vector <unsigned int> const& input,const unsigned int crc_version);
unsigned int xor_with_1(const unsigned int a);
void print_table(const std::vector <unsigned int>& table);

#endif // FONCTIONS_H_INCLUDED
