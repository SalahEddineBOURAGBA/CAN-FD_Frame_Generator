#include "analyzer_functions.h"

using namespace std;

/*------------------------------------------------------------------------------
------------------------------Analyze a given frame-----------------------------
------------------------------------------------------------------------------*/

//Analyze a given frame
bool analyze_frame(vector <bool> const& frame)
{
    vector <bool> identifier, DLC, DATA, received_stuff_count, received_CRC;
    vector <bool> crc_input, calculated_stuff_count, calculated_CRC;
    unsigned int i, j, k(1), Max, stuff(0), data_length;
    bool SRR, RTR, IDE, FDF, BRS, ESI;


//---------------------------------------------
//-----SOF-------------------------------------
//---------------------------------------------
    i=0;
    if(frame[i]!=0)
    {
        print_table(frame);
        cout <<"SOF error" <<endl <<endl;
        return 0;
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

    //Read SRR/RTR
    i=j;
    SRR=frame[i];

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

    //SRR --> basic format
    if(SRR==0)
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
            print_table(frame);
            cout <<"Format Error: IDE must be dominant" <<endl <<endl;
            return 0;
        }

        //move to FDF
        i++;
    }

    //SRR, continue arbitration field
    else if(SRR==1)
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
            print_table(frame);
            cout <<"Form Error: IDE is recessif" <<endl <<endl;
            return 0;
        }

        //move to identifier extension
        i++;
        Max=18;

        for(j=i;j<i+Max;j++)
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

        //check RTR, must be dominant (in can for now dominant also)
        i=j;
        RTR=frame[i];

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

        if(RTR==1)
        {
            print_table(frame);
            cout <<"Format Error: RRS must be dominant" <<endl <<endl;
            return 0;
        }

        //Move to FDF or R1
        i++;

        if(frame[i]==0) //check R1 in CAN
        {
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
        //move to FDF
        i++;
        }

        //Move to FDF
//        i++;
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
        //nothing to do move to DLC after
    }
    else if(FDF==1) //CAN FD
    {
        //Reserved bit must be dominant
        i++;
        if(frame[i]==1)
        {
            print_table(frame);
            cout <<"Frame Error: Reserved bit must be dominant" <<endl <<endl;
            return 0;
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
    }



//---------------------------------------------
//------------DLC Field------------------------
//---------------------------------------------
        i++;
        Max=i+3;
        data_length=1;

        for(j=i;j<=Max;j++)
        {
            //Stuff bits count
            if(frame[j]==frame[j-1])
                k++;
            else
                k=1;

            //DLC stoked inverted
            DLC.push_back(frame[j]);

            //calculate data length
            if(DLC.size()==4)
            {
                data_length=data_length_from_dlc(DLC);
                if(FDF==0) //can
                    if(data_length>8)
                        data_length=8;
            }

            //Stuff bits count
            if(k==5)
            {
                if(FDF==1)//can fd
                {
                    if(data_length!=0)//if ==0 there is no stuff bit to skip
                    {
                        stuff++;
                        k=1;
                        Max++;
                        j++;
                    }
                }

                else if(FDF==0)//can
                {
                    stuff++;
                    k=1;
                    Max++;
                    j++;
                }

            }
        }
        i=j;



//---------------------------------------------
//------------Data Field-----------------------
//---------------------------------------------
        if(data_length!=0)
        {
            //data field
            Max=i+(data_length*8);

            for(j=i;j<Max-1;j++)
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

            if(FDF==1)
                DATA.push_back(frame[i]);
            else if(FDF==0)
            {
                //Stuff bits count
                if(frame[i]==frame[i-1])
                    k++;
                else
                    k=1;

                DATA.push_back(frame[i]);

                //Stuff bits count
                if(k==5)
                {
                    stuff++;
                    k=1;
                    i++;
                }
            }
            i++;//move to CRC
        }



//---------------------------------------------
//------------CRC Field------------------------
//---------------------------------------------

    //-----------------------------------------
    //Calculate the CRC and the stuff bit count
    //of the receiving frame
    //----------------------------------------

        //Construct the sub frame to calculate it's CRC
        for(j=0;j<i;j++)
            crc_input.push_back(frame[j]);

        //Calculate CRC
        if(FDF==0)
            calculated_CRC=crc(unstuff_frame(crc_input),15);

        else if (FDF==1)
        {
            //Calculate Stuff count
            calculated_stuff_count=stuff_bit_count(stuff);

            if(data_length<=16) //CRC 17
                calculated_CRC=crc(crc_input,17);
            else                //CRC 21
                calculated_CRC=crc(crc_input,21);
        }



    //----------------------------------------
    //Read the CRC and the stuff bit count
    //of the receiving frame
    //----------------------------------------
        if(FDF==1)//CAN FD
        {
            //Jump first fixed stuff bit
            i++;

            //Read Stuff count
            for(j=i;j<i+4;j++)
                received_stuff_count.push_back(frame[j]);
            i=j; //2nd stuff bit

            //Read CRC
            if(data_length<=16) //CRC 17, 5 fixed stuff bits
                Max=i+17+5;
            else                //CRC 21, 6 fixed stuff bits
                Max=i+21+6;


            for(j=i;j<Max;j++)
            {
                //jump fixed stuff bits after every 4 bits
                if((j-i)%5!=0)
                    received_CRC.push_back(frame[j]);
            }
            i=j;
        }
        else if(FDF==0)//can
        {
            Max=i+15;
            for(j=i;j<Max;j++)
            {
                //Stuff bits count
                if(frame[j]==frame[j-1])
                    k++;
                else
                    k=1;

                received_CRC.push_back(frame[j]);

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
        }


    //----------------------------------------
    //Compare the CRC and the stuff count-----
    //----------------------------------------
        if(FDF==1)
        {
            if(compare_two_tables(received_stuff_count,calculated_stuff_count)==0)
            {
                print_table(frame);
                cout <<"CRC Error: Stuff count doesn't match" <<endl <<endl;
                return 0;
            }
        }
        if(compare_two_tables(received_CRC,calculated_CRC)==0)
        {
            print_table(frame);
            cout <<FDF<<"CRC Error: CRC doesn't match" <<endl <<endl;
            return 0;
        }

    //----------------------------------------
    //CRC Delimiter------CANFD----------------
    //----------------------------------------
        if(frame[i]==0) //First CRC Delimiter
        {
            print_table(frame);
            cout <<"Frame Error: CRC Delimiter must be Recessif" <<endl <<endl;
            return 0;
        }
        if(frame[i+1]==1) //Second CRC Delimiter
            i++;

        //Move to ACK
        i++;

    //}//end else if - CAN FD



//---------------------------------------------
//------------ACK Field------------------------
//---------------------------------------------
    if(frame[i]==1)
    {
        print_table(frame);
        cout <<"No node acknowledged the frame" <<endl <<endl;
        return 0;
    }

    i++;
    if(frame[i]==0)
    {
        print_table(frame);
        cout <<"Frame Error: ACK Delimiter must be recessive" <<endl <<endl;
        return 0;
    }

    i++;



//---------------------------------------------
//------------EOF INTERFRAME-------------------
//---------------------------------------------
    for(j=i;j<i+10;j++)
        if(frame[j]==0)
        {
            print_table(frame);
            cout <<"EOF ERROR" <<endl <<endl;
            return 0;
        }


//---------------------------------------------
//------------Display frame--------------------
//---------------------------------------------
    display_analyzed_frame(identifier, DLC, DATA, received_stuff_count,received_CRC,IDE,FDF,BRS,ESI);

//---------------------------------------------
//------------------Free-----------------------
//---------------------------------------------
identifier.clear();DLC.clear();DATA.clear();received_stuff_count.clear();received_CRC.clear();
crc_input.clear();calculated_stuff_count.clear();calculated_CRC.clear();

    return 1;
}

void display_analyzed_frame(vector <bool> const& identifier, vector <bool> const&  DLC, vector <bool> const&  DATA,
                            vector <bool> const& stuff_count, vector <bool> const& CRC, const bool IDE, const bool FDF,
                            const bool BRS, const bool ESI)
{
    unsigned int data_length=data_length_from_dlc(DLC);

    if(FDF==0) //can
        if(data_length>8)
            data_length=8;

    cout <<endl;

    if (FDF)
        cout <<"CAN FD Data Frame:";
    else
        cout <<"CAN Data Frame:";

    cout<<endl<<endl;


    //basic or long format
    if(IDE==0)
        cout <<"In Basic Format, ";
    else
        cout <<"In Extended Format, ";




    if (FDF)
    {
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
    }

    //Identifier
    cout <<"Identifier: ";
    print_table(identifier);

    //Data Field
    cout <<"Frame has " <<data_length <<" data bytes: ";
    print_table(DATA);

    if(FDF)
    {
        //CRC Field
        print_stuff_count(stuff_count);
    }

    cout <<"CRC Sequence: ";
    print_table_inverted(CRC);

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
