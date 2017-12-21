#ifndef FONCTIONS
#define FONCTIONS

#include <iostream>
#include <vector>

unsigned int data_length_from_dlc(std::vector <bool> const& DLC);

/*-------------------------------------------------------------------------------
----------------------------------Table manipulation-----------------------------
-------------------------------------------------------------------------------*/

void print_table(std::vector <bool> const& table);
void print_table_inverted(std::vector <bool> const& table);
void print_stuff_count(std::vector <bool> const& table);
bool compare_two_tables(std::vector <bool> const& table1,std::vector <bool> const& table2);



#endif // FONCTIONS_H_INCLUDED
