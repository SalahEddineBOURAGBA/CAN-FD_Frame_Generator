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
        cout <<table[i] <<' ';

    cout <<endl <<endl;
}

//display inverted table
void print_table_inverted(vector <bool> const& table)
{
    for (int i=(table.size()-1);i>=0;i--)
        cout <<table[i] <<' ';

    cout <<endl <<endl;
}

//display stuff count
void print_stuff_count(vector <bool> const& table)
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
bool compare_two_tables(vector <bool> const& table1,vector <bool> const& table2)
{
    if(table1.size() != table2.size())
        return 0;

    for(unsigned int j=0;j<table1.size();j++)
        if(table1[j] != table2[j])
            return 0;

    return 1;
}
