#include "functions.h"

using namespace std;

int main()
{
    //No stuff bits yet
    //Only CAN FD frames, no CAN frames

    //Tampon frame from SOF to last Data bit
    vector <unsigned int> frame {0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,0,1};
    //Tampon frame from CRC delimiter to last interframe bit
    vector <unsigned int> frame2 {1,0,1,1,1,1,1,1,1,1,1,1,1};
    //stuff count and CRC
    vector <unsigned int> crc_output,stuff_count;

    //Calculate stuff count and CRC
    stuff_count=stuff_bit_count(frame);
    crc_output=crc(frame,17);

    //Complete frame
    frame.insert(frame.end(),stuff_count.begin(),stuff_count.end());
    frame.insert(frame.end(),crc_output.begin(),crc_output.end());
    frame.insert(frame.end(),frame2.begin(),frame.end());

    //Analyse frame
    analyse_frame(frame);

    return 0;
}
