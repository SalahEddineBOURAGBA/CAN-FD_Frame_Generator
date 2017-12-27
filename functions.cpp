#include "functions.h"

using namespace std;

//return data length using DLC Field
unsigned int data_length_from_dlc(vector <bool> const& DLC)
{
    if(DLC[3]==0)
    {
        if(DLC[2]==0)
        {
            if(DLC[1]==0)
            {
                if(DLC[0]==0)
                    return 0;
                else
                    return 1;
            }
            else
            {
                if(DLC[0]==0)
                    return 2;
                else
                    return 3;
            }
        }
        else
        {
            if(DLC[1]==0)
            {
                if(DLC[0]==0)
                    return 4;
                else
                    return 5;
            }
            else
            {
                if(DLC[0]==0)
                    return 6;
                else
                    return 7;
            }
        }
    }
    else
    {
        if(DLC[2]==0)
        {
             if(DLC[1]==0)
            {
                if(DLC[0]==0)
                    return 8;
                else
                    return 12;
            }
            else
            {
                if(DLC[0]==0)
                    return 16;
                else
                    return 20;
            }
        }
        else
        {
            if(DLC[1]==0)
            {
                if(DLC[0]==0)
                    return 24;
                else
                    return 32;
            }
            else
            {
                if(DLC[0]==0)
                    return 48;
                else
                    return 64;
            }
        }
    }
}

/*-------------------------------------------------------------------------------
----------------------------------Table manipulation-----------------------------
-------------------------------------------------------------------------------*/

//display table
void print_table(vector <bool> const& table)
{
    for (unsigned int i=0;i<table.size();i++)
        cout <<table[i];

    cout <<endl <<endl;
}

//compare 2 tables
bool compare_two_tables(vector <bool> const& table1,vector <bool> const& table2)
{
    if(table1.size() != table2.size())
        return 0;

    for(unsigned int j=0;j<table1.size();j++)
        if(table1[j] != table2[j])
            return 0;

    return 1;
}



/*------------------------------------------------------------------------------
--------------------------------CRC Calculation---------------------------------
------------------------------------------------------------------------------*/

//calculate CRC of a frame
//In CAN, the input is the no-stuffed sub frame (from SOF to last Data byte)
//In CAN FD, the input is the dynamic stuffed sub frame (from SOF to last Data byte)
vector <bool> crc(vector <bool> const& input,const unsigned int crc_version)
{
    vector <bool> crc_input=input;
    vector <bool> crc_output(crc_version,0);
    vector <bool> zeros (crc_version,0);

    unsigned int i,j,loop;

    //Modify crc init in CRC-17 and CRC-21
    if(crc_version==17 || crc_version==21)
        crc_output[crc_version-1]=1;

    //Add 'crc_version' zeros to the end of crc_input
    crc_input.insert(crc_input.end(),zeros.begin(),zeros.end());
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
                        crc_output[j]=crc_output[j-1]^1;
                    else
                        crc_output[j]=crc_output[j-1];
                }
            }
            else if(crc_version==17)
            {
                for(j=crc_version-1;j>0;j--)
                {
                    if(j==16 || j==14 || j==13 || j==11 || j==6 || j==4 || j==3 || j==1)
                        crc_output[j]=crc_output[j-1]^1;
                    else
                        crc_output[j]=crc_output[j-1];
                }
            }
            else if(crc_version==21)
            {
                for(j=crc_version-1;j>0;j--)
                {
                    if(j==20 || j==13 || j==11 || j==7 || j==4 || j==3 || j==1)
                        crc_output[j]=crc_output[j-1]^1;
                    else
                        crc_output[j]=crc_output[j-1];
                }
            }

            crc_output[0]=crc_input[0]^1;

            crc_input.erase(crc_input.begin());
        }

    }

    //free
    crc_input.clear(); zeros.clear();

    return crc_output;
}

//calculate stuff count of a dynamic stuffed sub frame
vector <bool> stuff_bit_count(const unsigned int stuff)
{
    vector <bool> output(4);
    unsigned int stuff_mod_8;

    stuff_mod_8=stuff%8;

    //return stuff count as follows
    //bit2 bit1 bit0 parity
    switch(stuff_mod_8)
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



/*------------------------------------------------------------------------------
--------------------------------Stuffing----------------------------------------
------------------------------------------------------------------------------*/

//Stuff a CAN or CAN FD frame
unsigned int stuff_frame(vector <bool>& frame, bool can)
{
    unsigned int k(1), nbr_stuff_bits(0);
    vector <bool>::iterator it;

    //In CAN FD don't stuff the last 5 consecutive bits
    //that ends at the end of the frame
    for(it=frame.begin()+1;it<frame.end()-1;it++)
    {
        if(*it==*(it-1))
        {
            k++;
            if(k==5)
            {
                frame.insert(it+1,!(*it));
                k=1;
                nbr_stuff_bits++;
                it++;
            }
        }
        else
            k=1;
    }

    //Take in consideration the last 5 bits in CAN
    //There is no fixed stuffing
    if(can)
    {
        if(*it==*(it-1))
        {
            k++;
            if(k==5)
            {
               frame.insert(it+1,!(*it));
               nbr_stuff_bits++;
            }
        }
    }

    return nbr_stuff_bits;
}

//stuff the CRC Field of a CAN FD frame
vector <bool> stuff_crc(vector <bool> const& stuff_count, vector <bool> const& crc, const bool first_bit)
{
    vector <bool> output;
    unsigned int i(0);
    vector<bool>::iterator it;

    //1st fixed stuff bit
    output.push_back(!first_bit);
    //Stuff count
    output.insert(output.end(),stuff_count.begin(),stuff_count.end());
    //2nd fixed stuff bit
    output.push_back(!output[output.size()-1]);
    //Add the CRC and stuff it after
    output.insert(output.end(),crc.begin(),crc.end());

    //Fixed stuff CRC
    for(it=output.begin()+6;it<output.end();it++)
    {
        i++;
        if(i==4)
        {
            i=0;
            output.insert(it+1,!(*it));
            it++;
        }

    }
    return output;
}

//For CAN, CRC calculation is used on unstuffed frame
vector <bool> unstuff_frame(vector <bool> const& input)
{
    vector <bool> frame=input;
    unsigned int k(1);
    vector <bool>::iterator it;

    for(it=frame.begin()+1;it<frame.end()-1;it++)
    {
        if(*it==*(it-1))
        {
            k++;
            if(k==5)
            {
                k=1;
                if(*(it+1)==*(it+2))
                    k++;
                frame.erase(it+1);
                it++;
            }
        }
        else
            k=1;
    }

    return frame;
}
