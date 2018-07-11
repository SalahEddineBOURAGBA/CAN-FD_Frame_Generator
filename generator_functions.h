#ifndef GENERATOR_FUNCTIONS
#define GENERATOR_FUNCTIONS


#include <stdlib.h>
#include "embeddedvector.h"
#include "CANFDFrame.h"


/*------------------------------------------------------------------------------
--------------------------------CRC Calculation---------------------------------
------------------------------------------------------------------------------*/
//Return the CRC of a frame
embeddedvector <bool> crc(const embeddedvector <bool>& input,const unsigned int crc_version);
//return the stuff bit count
embeddedvector <bool> stuff_bit_count(const unsigned int stuff);


/*------------------------------------------------------------------------------
--------------------------------Stuffing----------------------------------------
------------------------------------------------------------------------------*/
//Stuff a CAN or CAN FD frame
unsigned int stuff_frame(embeddedvector <bool>& frame, bool can);
//Fixed Stuffed CRC Field for CAN FD
embeddedvector <bool> stuff_crc(embeddedvector <bool> const& stuff_count, embeddedvector <bool> const& crc,
                             const bool first_bit);
//Unstuff a CAN or CAN FD frame
embeddedvector <bool> unstuff_frame(embeddedvector <bool> const& input);


/*------------------------------------------------------------------------------
------------------------------Generate a frame----------------------------------
------------------------------------------------------------------------------*/
embeddedvector <bool> generate_frame(const CANFDFrame &inFrame);


#endif // GENERATOR_FUNCTIONS
