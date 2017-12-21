#include "functions.h"

using namespace std;

int main()
{
    //No stuff bits yet
    //Only CAN FD frames, no CAN frames

    //Tampon frame from SOF to last Data bit
    vector <unsigned int> frame {0, 0,0,0,0,1,0,0,1,1,1,1,1,0,0, 0,1,0,1,1,1,0,0,0, 0,0,1,0,1,1,1,1,1};
    //Tampon frame from 1st fixed stuff bit to last CRC bit
    vector <unsigned int> frame2 {0,0,1,0,1,0,1,1,1,1,0,0,1,1,0,1,0,1,1,1,0,1,1,1,1,0,0};
    //Tampon frame from CRC delimiter to last interframe bit
    vector <unsigned int> frame3 {1,0,1,1,1,1,1,1,1,1,1,1,1};
    //Complete frame
    frame.insert(frame.end(),frame2.begin(),frame2.end());
    frame.insert(frame.end(),frame3.begin(),frame3.end());


    //Analyse frame
    analyse_frame(frame);

    /*
    vector <unsigned int> crc_output,stuff_count;
    stuff_count=stuff_bit_count(3);
    crc_output=crc(frame,17);
    print_table(stuff_count);
    print_table(crc_output);*/

    return 0;
}
