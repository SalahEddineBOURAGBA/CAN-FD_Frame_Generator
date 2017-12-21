#include "generator_functions.h"

using namespace std;

vector <bool> generate_frame()
{
    bool SOF(0), RTR(0), SRR(1), IDE, FDF, R0(0), R1(0), BRS, ESI;
    unsigned int length, nbr_stuff_bits, i;
    vector <bool> DLC, stuff_count, CRC, stuffed_crc;
    vector <bool> frame;

    //Seed srand
    struct timeval time;
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec));

    //generate random values for FDF IDE BRS ESI
    FDF=((rand()%100)>50)?1:0;
    IDE=((rand()%100)>50)?1:0;
    BRS=((rand()%100)>50)?1:0;
    ESI=((rand()%100)>50)?1:0;

    //SOF
    frame.push_back(SOF);

    //Identifier Field
    for(i=0;i<11;i++)
        frame.push_back(((rand()%100)>50)?1:0);

    if(IDE==1)//Extended frame
    {
        //SRR
        frame.push_back(SRR);
        frame.push_back(IDE);
        for(i=0;i<18;i++)
                frame.push_back(((rand()%100)>50)?1:0);
    }
    frame.push_back(RTR);

    if(FDF==0)//CAN
    {
        if(IDE==0)
            frame.push_back(IDE);
        else
            frame.push_back(R1);
        frame.push_back(R0);
    }

    else if (FDF==1) //CAN FD
    {
        if(IDE==0)
            frame.push_back(IDE);

        //Control Field
        frame.push_back(FDF);
        frame.push_back(R0);
        frame.push_back(BRS);
        frame.push_back(ESI);
    }

    for(i=0;i<4;i++)
            DLC.push_back(((rand()%100)>50)?1:0);
    frame.insert(frame.end(),DLC.begin(),DLC.end());

    //DATA Field
    length=data_length_from_dlc(DLC);
    if(FDF==0) //CAN
        if(length>8)
            length=8;
    DLC.clear();

    for(i=0;i<length*8;i++)
            frame.push_back(((rand()%100)>50)?1:0);

    //CRC Field
    if(FDF==0) //CAN
    {
        //In CAN calculate CRC then stuff frame
        CRC=crc(frame,15);
        frame.insert(frame.end(),CRC.begin(),CRC.end());
        CRC.clear();

        //the second parameter is '1' to stuff the hole frame
        stuff_frame(frame,1);
    }

    else if (FDF==1) //CAN FD
    {
        //Stuff the frame before calculating CRC and return stuff count
        //the second parameter is '0' to stuff the hole frame except the last 5 bits
        nbr_stuff_bits=stuff_frame(frame,0);

        //Stuff count
        stuff_count=stuff_bit_count(nbr_stuff_bits);
        //CRC
        if(length<17)
            CRC=crc(frame,17);
        else
            CRC=crc(frame,21);

        //Fixed stuffed CRC Field
        stuffed_crc=stuff_crc(stuff_count,CRC,frame[frame.size()-1]);
        //add fixed stuffed CRC to the frame
        frame.insert(frame.end(),stuffed_crc.begin(),stuffed_crc.end());

        //free space
        stuffed_crc.clear();CRC.clear();stuff_count.clear();
    }

    //CRC Delimiter
    frame.push_back(1);

    //ACK
    frame.push_back(0);
    frame.push_back(1);

    //EOF
    for(i=1;i<=10;i++)
        frame.push_back(1);

    return(frame);
}

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

