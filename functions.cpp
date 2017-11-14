#include "iostream"
#include "vector"
#include <algorithm>
#include "functions.h"

using namespace std;

/*------------------------------------------------------------------------------
------------------------------Analyze a given frame-----------------------------
------------------------------------------------------------------------------*/

//Analyze a given frame
void analyse_frame(vector <unsigned int> const& frame)
{
    vector <unsigned int> identifier, DLC, DATA, received_stuff_count, received_CRC;
    vector <unsigned int> crc_input, calculated_stuff_count, calculated_CRC;
    unsigned int i, j, k(1), Max, stuff(0);
    unsigned int RRS, IDE, FDF, BRS, ESI, data_length;



//---------------------------------------------
//-----SOF-------------------------------------
//---------------------------------------------
    i=0;
    if(frame[i]!=0)
    {
        cout <<"SOF error";
        exit(EXIT_FAILURE);
    }



//---------------------------------------------
//-----Arbitration field-----------------------
//---------------------------------------------
    i++;
    Max=11;
    //Base identifier
    for(j=i;j<=Max;j++)
    {
        //Stuff bits count
        if(frame[j]==frame[j-1])
            k++;
        else
            k=1;

        //insert bit in the identifier table
        identifier.insert(identifier.begin(),frame[j]);

        //If 5 successive bits then jump bit
        if(k==5)
        {
            stuff++;
            Max++;
            k=1;
            j++;
        }

    }

    //Read RRS/SRR
    i=j;
    RRS=frame[i];

        //Stuff bits count
        if(frame[i]==frame[i-1])
            k++;
        else
            k=1;
        if(k==5)
        {
            stuff++;
            k=1;
            i++;
        }

    //RRS --> basic format
    if(RRS==0)
    {
        //move to IDE
        i++;
        IDE=frame[i];
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
                stuff++;
                k=1;
                i++;
            }

        //IDE must be dominant
        if(IDE==1)
        {
            cout <<"Format Error: IDE must be dominant";
            exit(EXIT_FAILURE);
        }

        //move to FDF
        i++;
    }

    //SRR, continue arbitration field
    else if(RRS==1)
    {
        //move to IDE
        i++;
        IDE=frame[i];
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
                stuff++;
                k=1;
                i++;
            }

        //IDE must be recessif
        if(IDE==0)
        {
            cout <<"Form Error: IDE is recessif";
            exit(EXIT_FAILURE);
        }

        //move to identifier extension
        i++;
        Max=18;

        for(j=i;j<=i+Max;j++)
        {
            //Stuff bits count
            if(frame[j]==frame[j-1])
                k++;
            else
                k=1;

            //insert bit in the identifier table
            identifier.insert(identifier.begin(),frame[j]);

            //Stuff bits count
            if(k==5)
            {
                stuff++;
                k=1;
                Max++;
                j++;
            }
        }

        //check RRS, must be dominant
        i=j;
        unsigned int RRS2=frame[i];

            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
                stuff++;
                k=1;
                i++;
            }

        if(RRS2==1)
        {
            cout <<"Format Error: RRS must be dominant";
            exit(EXIT_FAILURE);
        }

        //Move to FDF
        i++;
    }

    FDF=frame[i];
        //Stuff bits count
        if(frame[i]==frame[i-1])
            k++;
        else
            k=1;
        if(k==5)
        {
            stuff++;
            k=1;
            i++;
        }

    //check FDF
    if(FDF==0) //CAN
    {
        //code CAN
    }
    else if(FDF==1) //CAN FD
    {
        //Reserved bit must be dominant
        i++;
        if(frame[i]==1)
        {
            cout <<"Frame Error: Reserved bit must be dominant";
            exit(EXIT_FAILURE);
        }
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
                stuff++;
                k=1;
                i++;
            }

        //Move to BRS
        i++;
        BRS=frame[i];
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
                stuff++;
                k=1;
                i++;
            }

        //Move to ESI
        i++;
        ESI=frame[i];
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
                stuff++;
                k=1;
                i++;
            }



//---------------------------------------------
//------------DLC Field------------------------
//---------------------------------------------
        i++;
        Max=i+3;

        for(j=i;j<=Max;j++)
        {
            //Stuff bits count
            if(frame[j]==frame[j-1])
                k++;
            else
                k=1;

            //DLC stoked inverted
            DLC.insert(DLC.begin(),frame[j]);

            //Stuff bits count
            if(k==5)
            {
                stuff++;
                k=1;
                Max++;
                j++;
            }
        }

        //calculate data length
        data_length=data_length_from_dlc(DLC);




//---------------------------------------------
//------------Data Field-----------------------
//---------------------------------------------
        //data field
        i=j;
        Max=i+(data_length*8);

        for(j=i;j<Max;j++)
        {
            //Stuff bits count
            if(frame[j]==frame[j-1])
                k++;
            else
                k=1;

            DATA.push_back(frame[j]);

            //Stuff bits count
            if(k==5)
            {
                stuff++;
                k=1;
                Max++;
                j++;
            }
        }
        i=j;




//---------------------------------------------
//------------CRC Field------------------------
//---------------------------------------------

    //----------------------------------------
    //Calculate the CRC of the receiving frame
    //----------------------------------------

        //Construct the sub frame to calculate it's CRC
        for(j=0;j<i;j++)
            crc_input.push_back(frame[j]);

        //Calculate CRC
        if(data_length<=16) //CRC 17
            calculated_CRC=crc(crc_input,17);
        else                //CRC 21
            calculated_CRC=crc(crc_input,21);

        //Calculate Stuff count
        calculated_stuff_count=stuff_bit_count(crc_input);


    //----------------------------------------
    //Calculate the CRC of the receiving frame
    //----------------------------------------

        //Read Stuff count
        for(j=i;j<i+4;j++)
            received_stuff_count.push_back(frame[j]);
        i+=4;

        //Read CRC
        if(data_length<=16) //CRC 17
            Max=17;
        else                //CRC 21
            Max=21;

        for(j=i;j<i+Max;j++)
            received_CRC.push_back(frame[j]);
        i=j;

    //----------------------------------------
    //Compare the CRC and the stuff count-----
    //----------------------------------------
        if(compare_two_tables(received_stuff_count,calculated_stuff_count)==0)
        {
            cout <<"CRC Error: Stuff count doesn't match";
            exit(EXIT_FAILURE);
        }
        if(compare_two_tables(received_CRC,calculated_CRC)==0)
        {
            cout <<"CRC Error: CRC doesn't match";
            exit(EXIT_FAILURE);
        }

    //----------------------------------------
    //CRC Delimiter------CANFD----------------
    //----------------------------------------
        if(frame[i]==0) //First CRC Delimiter
        {
            cout <<"Frame Error: CRC Delimiter must be Recessif";
            exit(EXIT_FAILURE);
        }
        if(frame[i+1]==1) //Second CRC Delimiter
            i++;

        //Move to ACK
        i++;

    }//end else if - CAN FD



//---------------------------------------------
//------------ACK Field------------------------
//---------------------------------------------
    if(frame[i]==1)
    {
        cout <<"No node acknowledged the frame";
        exit(EXIT_FAILURE);
    }

    i++;
    if(frame[i]==0)
    {
        cout <<"Frame Error: ACK Delimiter must be recessif";
        exit(EXIT_FAILURE);
    }

    i++;



//---------------------------------------------
//------------EOF INTERFRAME-------------------
//---------------------------------------------
    for(j=i;j<i+10;j++)
        if(frame[j]==0)
        {
            cout <<"EOF ERROR";
            exit(EXIT_FAILURE);
        }


//---------------------------------------------
//------------Display frame--------------------
//---------------------------------------------
    display_frame(identifier,DATA, received_stuff_count,received_CRC,IDE,BRS,ESI,data_length);
}

//return data length using DLC Field
unsigned int data_length_from_dlc(vector <unsigned int> DLC)
{
    if(DLC[0]==0)
    {
        if(DLC[1]==0)
        {
            if(DLC[2]==0)
            {
                if(DLC[3]==0)
                    return 0;
                else
                    return 1;
            }
            else
            {
                if(DLC[3]==0)
                    return 2;
                else
                    return 3;
            }
        }
        else
        {
            if(DLC[2]==0)
            {
                if(DLC[3]==0)
                    return 4;
                else
                    return 5;
            }
            else
            {
                if(DLC[3]==0)
                    return 6;
                else
                    return 7;
            }
        }
    }
    else
    {
        if(DLC[1]==0)
        {
             if(DLC[2]==0)
            {
                if(DLC[3]==0)
                    return 8;
                else
                    return 12;
            }
            else
            {
                if(DLC[3]==0)
                    return 16;
                else
                    return 20;
            }
        }
        else
        {
            if(DLC[2]==0)
            {
                if(DLC[3]==0)
                    return 24;
                else
                    return 32;
            }
            else
            {
                if(DLC[3]==0)
                    return 48;
                else
                    return 64;
            }
        }
    }
}

void display_frame(vector <unsigned int> const& identifier, vector <unsigned int> const&  DATA, vector <unsigned int>
                    const& stuff_count, vector <unsigned int> const& CRC, unsigned int IDE, unsigned int BRS,
                    unsigned int ESI, unsigned int data_length)
{
    cout <<"CAN FD Frame: ";

    //basic or long format
    if(IDE==0)
        cout <<"Basic Format.";
    else
        cout <<"Extended Format.";

    cout <<endl <<endl;

    //BRS
    cout <<"Bit Rate Switch ";
    if(BRS==1)
        cout <<"Activated.";
    else
        cout <<"Deactivated";

    cout <<endl <<endl;

    //ESI
    cout <<"Frame in: ";
    if(ESI==0)
        cout <<"Active Error State.";
    else
        cout <<"Passive Error State.";
    cout <<endl <<endl;

    //Identifier
    cout <<"Frame Identifier: ";
    print_table(identifier);

    //Data Field
    cout <<"Frame has " <<data_length <<" bytes data: ";
    print_table(DATA);

    //CRC Field
    print_stuff_count(stuff_count);

    cout <<"CRC Sequence: ";
    print_table_inverted(CRC);

}

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
----------------------------------Table manipulation-----------------------------
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

//display stuff count
void print_stuff_count(vector <unsigned int> const& table)
{
    cout <<"Stuff count: ";
    cout <<table[2] <<' ';
    cout <<table[1] <<' ';
    cout <<table[0] <<' ';

    cout <<", Parity: ";
    cout <<table[3];

    cout <<endl <<endl;
}

//compare 2 tables
unsigned int compare_two_tables(vector <unsigned int> const& table1,vector <unsigned int> const& table2)
{
    if(table1.size() != table2.size())
        return 0;

    for(unsigned int j=0;j<table1.size();j++)
        if(table1[j] != table2[j])
            return 0;

    return 1;
}
