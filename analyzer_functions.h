#ifndef ANALYZER_FUNCTIONS
#define ANALYZER_FUNCTIONS

#include <iostream>
#include <vector>
#include "functions.h"

/*------------------------------------------------------------------------------
------------------------------Analyze a given frame-----------------------------
------------------------------------------------------------------------------*/

bool analyse_frame(std::vector <bool> const& frame);
void display_analyzed_frame(std::vector <bool> const& identifier, std::vector <bool> const&  DLC,
                            std::vector <bool> const&  DATA, std::vector <bool> const& stuff_count,
                            std::vector <bool> const& CRC, const bool IDE, const bool FDF, const bool BRS, const bool ESI);



/*------------------------------------------------------------------------------
--------------------------------CRC Calculation---------------------------------
------------------------------------------------------------------------------*/

std::vector <bool> crc(std::vector <bool> const& input,const unsigned int crc_version);
std::vector <bool> stuff_bit_count(const unsigned int stuff);
std::vector <bool> unstuff_frame(std::vector <bool> const& input);


#endif // FONCTIONS_H_INCLUDED
