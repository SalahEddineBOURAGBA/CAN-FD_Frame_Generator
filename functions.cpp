#include "iostream"
#include "vector"
#include "functions.h"

using namespace std;

//calculate CRC of a frame
vector <unsigned int> crc(vector <unsigned int> const& input,const unsigned int crc_version)
{
    vector <unsigned int> crc_input=input;
    vector <unsigned int> crc_output (crc_version,0);
    vector <unsigned int> zeros (crc_version,0);

    int i,j,loop;

    //exit if crc_version != (15,17,21)
    if (crc_version!=15 && crc_version!=17 && crc_version!=21)
        exit(EXIT_FAILURE);

    //Add 'crc_version' zeros to the end of crc_input
    crc_input.insert(crc_input.begin(),zeros.begin(),zeros.end());
    loop=crc_input.size();

    //calculate crc
    for(i=loop;i>0;i--)
    {
        //if MSB of crc_output=0 then shift left
        if(crc_output[crc_version-1]==0)
        {
            for(j=crc_version-1;j>0;j--)
                crc_output[j]=crc_output[j-1];

            crc_output[0]=crc_input[crc_input.size()-1];

            crc_input.pop_back();
        }

        //if MSB of crc_output=1 then shift left and xor with crc polynomial
        else if(crc_output[crc_version-1]==1)
        {
            //xor with polynomial depending on crc_version
            if(crc_version==15)
            {
                for(j=crc_version-1;j>0;j--)
                {
                    if(j==14 || j==10 || j==8 || j==7 || j==4 || j==3)
                        crc_output[j]=xor_with_1(crc_output[j-1]);
                    else
                        crc_output[j]=crc_output[j-1];
                }
            }
            else if(crc_version==17)
            {
                for(j=crc_version-1;j>0;j--)
                {
                    if(j==16 || j==14 || j==13 || j==11 || j==6 || j==4 || j==3 || j==1)
                        crc_output[j]=xor_with_1(crc_output[j-1]);
                    else
                        crc_output[j]=crc_output[j-1];
                }
            }
            else if(crc_version==21)
            {
                for(j=crc_version-1;j>0;j--)
                {
                    if(j==20 || j==13 || j==11 || j==7 || j==4 || j==3 || j==1)
                        crc_output[j]=xor_with_1(crc_output[j-1]);
                    else
                        crc_output[j]=crc_output[j-1];
                }
            }

            crc_output[0]=xor_with_1(crc_input[crc_input.size()-1]);

            crc_input.pop_back();
        }
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
