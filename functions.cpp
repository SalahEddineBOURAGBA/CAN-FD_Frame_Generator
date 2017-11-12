#include "iostream"
#include "vector"
#include "functions.h"

using namespace std;


/*------------------------------------------------------------------------------
--------------------------------CRC Calculation---------------------------------
------------------------------------------------------------------------------*/

//calculate CRC of a frame
//In CAN, the input is the destuffed sub frame (from SOF to last Data byte)
//In CAN FD, the input is the dynamique stuffed sub frame (from SOF to last Data byte)
vector <unsigned int> crc(vector <unsigned int> const& input,const unsigned int crc_version)
{
    vector <unsigned int> crc_input=input;
    vector <unsigned int> crc_output (crc_version,0);
    vector <unsigned int> zeros (crc_version,0);

    int i,j,loop;

    //Modify crc init in CRC-17 and CRC-21
    if(crc_version==17 || crc_version==21)
        zeros[0]=1;

    //Add 'crc_version' zeros to the end of crc_input
    crc_input.insert(crc_input.begin(),zeros.begin(),zeros.end());
    loop=crc_input.size();

    //calculate crc
    for(i=0;i<loop;i++)
    {
        //if MSB of crc_output=0 then shift left
        if(crc_output[crc_version-1]==0)
        {
            for(j=crc_version-1;j>0;j--)
                crc_output[j]=crc_output[j-1];

            crc_output[0]=crc_input[0];

            crc_input.erase(crc_input.begin());
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

            crc_output[0]=xor_with_1(crc_input[0]);

            crc_input.erase(crc_input.begin());
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

//calculate stuff count of a dynamic stuffed sub frame
vector <unsigned int> stuff_bit_count(vector <unsigned int> const& input)
{
    vector <unsigned int> output(4);
    unsigned int i,k(1),stuff(0);

    for(i=0;i<input.size()-1;i++)
    {
        if(input[i]==input[i+1])
            k++;
        else
            k=1;

        if(k==5)
        {
            stuff++;
            k=1;
            i++;
        }
    }
    stuff%=8;

    //return stuff count as follows
    //bit2 bit1 bit0 parity
    switch(stuff)
    {
    case 0:
        output={0,0,0,0};
        break;
    case 1:
        output={1,0,0,1};
        break;
    case 2:
        output={1,1,0,0};
        break;
    case 3:
        output={0,1,0,1};
        break;
    case 4:
        output={0,1,1,0};
        break;
    case 5:
        output={1,1,1,1};
        break;
    case 6:
        output={1,0,1,0};
        break;
    case 7:
        output={0,0,1,1};
        break;
    }
    return output;
}

/*-------------------------------------------------------------------------------
----------------------------------Display----------------------------------------
-------------------------------------------------------------------------------*/

//display table
void print_table(vector <unsigned int> const& table)
{
    for (unsigned int i=0;i<table.size();i++)
        cout <<table[i] <<' ';

    cout <<endl <<endl;
}

//display inverted table
void print_table_inverted(vector <unsigned int> const& table)
{
    for (int i=table.size()-1;i>=0;i--)
        cout <<table[i] <<' ';

    cout <<endl <<endl;
}
