#include "functions.h"

using namespace std;

int main()
{
    vector <unsigned int> zeros (15,0);
    vector <unsigned int> frame {0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,1,0,1,1,0,0,1,0,1,1,1};
    vector <unsigned int> input_crc;
    vector <unsigned int> output_crc(15);

    //Union of two tables
    input_crc.insert(input_crc.begin(),frame.begin(),frame.end());
    input_crc.insert(input_crc.begin(),zeros.begin(),zeros.end());
    print_table(input_crc);

    //calculate crc_15
    output_crc=crc_15(input_crc);

    return 0;
}
