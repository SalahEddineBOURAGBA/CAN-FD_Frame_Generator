#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include "iostream"
#include "vector"
#include <algorithm>

/*------------------------------------------------------------------------------
------------------------------Analyze a given frame-----------------------------
------------------------------------------------------------------------------*/
void analyse_frame(std::vector <unsigned int> const& frame);
unsigned int data_length_from_dlc(std::vector <unsigned int> CRC);
void display_frame(std::vector <unsigned int> const& identifier, std::vector <unsigned int> const& ,
                   std::vector <unsigned int> const& stuff_count, std::vector <unsigned int> const& CRC,
                   unsigned int IDE, unsigned int BRS, unsigned int ESI, unsigned int data_length);

/*------------------------------------------------------------------------------
--------------------------------CRC Calculation---------------------------------
------------------------------------------------------------------------------*/
std::vector <unsigned int> crc(std::vector <unsigned int> const& input,const unsigned int crc_version);
unsigned int xor_with_1(const unsigned int a);
std::vector <unsigned int> stuff_bit_count(unsigned int stuff);

/*-------------------------------------------------------------------------------
----------------------------------Table manipulation-----------------------------
-------------------------------------------------------------------------------*/
void print_table(std::vector <unsigned int> const& table);
void print_table_inverted(std::vector <unsigned int> const& table);
void print_stuff_count(std::vector <unsigned int> const& table);
unsigned int compare_two_tables(std::vector <unsigned int> const& table1,std::vector <unsigned int> const& table2);

#endif // FONCTIONS_H_INCLUDED
