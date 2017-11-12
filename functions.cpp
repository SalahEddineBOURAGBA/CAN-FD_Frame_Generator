#include "iostream"
#include "vector"
#include "functions.h"

using namespace std;

vector <unsigned int> crc_15(vector <unsigned int>& input)
{
    vector <unsigned int> output {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int loop=input.size();
    int i,j;

    cout <<"00: ";
    print_table(output);

    for(i=loop;i>0;i--)
    {
        if(output[14]==0)
        {
            for(j=14;j>0;j--)
                output[j]=output[j-1];

            output[0]=input[input.size()-1];

            input.pop_back();
        }

        else if(output[14]==1)
        {
            for(j=14;j>0;j--)
            {
                if(j==14 || j==10 || j==8 || j==7 || j==4 || j==3)
                    output[j]=xor_with_1(output[j-1]);
                else
                    output[j]=output[j-1];
            }

            output[0]=xor_with_1(input[input.size()-1]);

            input.pop_back();
        }

        if((loop-i+1)<10) cout <<"0";
        cout <<loop-i+1 <<": ";
        print_table(output);
    }

    return output;
}

unsigned int xor_with_1(const unsigned int a)
{
    if(a==1)
        return 0;
    if(a==0)
        return 1;
    else
        exit(EXIT_FAILURE);
}

void print_table(const vector <unsigned int>& table)
{
    for (int i=table.size()-1;i>=0;i--)
        cout <<table[i] <<' ';

    cout <<endl <<endl;
}
