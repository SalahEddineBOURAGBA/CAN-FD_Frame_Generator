#include "functions.h"

using namespace std;

int main()
{
    vector <unsigned int> frame {0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,1,0,1,1,0,0,1,0,1,1,1};
    vector <unsigned int> output_crc(15);

    //calculate crc_15
    output_crc=crc_15(frame);

    print_table(frame);
    print_table(output_crc);

    return 0;
}
