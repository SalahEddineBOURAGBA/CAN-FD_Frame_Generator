#ifndef FONCTIONS
#define FONCTIONS

#include <iostream>
#include <vector>


unsigned int data_length_from_dlc(std::vector <bool> const& DLC);

/*-------------------------------------------------------------------------------
----------------------------------Table manipulation-----------------------------
-------------------------------------------------------------------------------*/
//print a table
void print_table(std::vector <bool> const& table);
//compare two tables
bool compare_two_tables(std::vector <bool> const& table1,std::vector <bool> const& table2);


/*------------------------------------------------------------------------------
--------------------------------CRC Calculation---------------------------------
------------------------------------------------------------------------------*/
//Return the CRC of a frame
std::vector <bool> crc(std::vector <bool> const& input,const unsigned int crc_version);
//return the stuff bit count
std::vector <bool> stuff_bit_count(const unsigned int stuff);


/*------------------------------------------------------------------------------
--------------------------------Stuffing----------------------------------------
------------------------------------------------------------------------------*/
//Stuff a CAN or CAN FD frame
unsigned int stuff_frame(std::vector <bool>& frame, bool can);
//Fixed Stuffed CRC Field for CAN FD
std::vector <bool> stuff_crc(std::vector <bool> const& stuff_count, std::vector <bool> const& crc,
                             const bool first_bit);
//Unstuff a CAN or CAN FD frame
std::vector <bool> unstuff_frame(std::vector <bool> const& input);


#endif // FONCTIONS_H_INCLUDED
