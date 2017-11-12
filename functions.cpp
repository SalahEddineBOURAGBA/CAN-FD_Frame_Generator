#include "iostream"
#include "vector"
#include "functions.h"

using namespace std;

//calculate CRC-15 of a frame
vector <unsigned int> crc_15(const vector <unsigned int>& input)
{
    vector <unsigned int> crc_input=input;
    vector <unsigned int> crc_output (15,0);
    vector <unsigned int> zeros (15,0);

    int loop=crc_input.size();
    int i,j;

    //Add 15 zeros to the end of crc_input
    crc_input.insert(crc_input.begin(),zeros.begin(),zeros.end());
    /* display
    cout <<"00: ";
    print_table(crc_output);*/

    for(i=loop;i>0;i--)
    {
        //if crc_output=0 then shift left
        if(crc_output[14]==0)
        {
            for(j=14;j>0;j--)
                crc_output[j]=crc_output[j-1];

            crc_output[0]=crc_input[crc_input.size()-1];

            crc_input.pop_back();
        }

        //if crc_output=1 then shift left and xor with crc-15 polynomial
        else if(crc_output[14]==1)
        {
            for(j=14;j>0;j--)
            {
                if(j==14 || j==10 || j==8 || j==7 || j==4 || j==3)
                    crc_output[j]=xor_with_1(crc_output[j-1]);
                else
                    crc_output[j]=crc_output[j-1];
            }

            crc_output[0]=xor_with_1(crc_input[crc_input.size()-1]);

            crc_input.pop_back();
        }
        /*display
        if((loop-i+1)<10) cout <<"0";
        cout <<loop-i+1 <<": ";
        print_table(crc_output);*/
    }

    return crc_output;
}

//xor with 1
unsigned int xor_with_1(const unsigned int a)
{
    if(a==1)
        return 0;
    if(a==0)
        return 1;
    else
        exit(EXIT_FAILURE);
}

//display passed table
void print_table(const vector <unsigned int>& table)
{
    for (int i=table.size()-1;i>=0;i--)
        cout <<table[i] <<' ';

    cout <<endl <<endl;
}
