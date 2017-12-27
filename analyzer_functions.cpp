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
    unsigned int i, j, k(1), Max, stuff(0), data_length, index;
    bool SRR, RTR, RRS, IDE, FDF, BRS, ESI;

    cout <<"Frame: ";
    print_table(frame);
//---------------------------------------------
//-----SOF-------------------------------------
//---------------------------------------------
    i=0;
    if(frame[i]==1)
    {
        cout <<"SOF error" <<endl <<endl;
        return 0;
    }
    else
    {
        cout <<setw(8) <<"SOF"
             <<" 0 "
             <<"SOF" <<endl<<endl;
    }




//---------------------------------------------
//-----Arbitration field-----------------------
//---------------------------------------------
    i++;
    Max=11;
    index=28;
    for(j=i;j<=Max;j++)
    {
        //Stuff bits count
        if(frame[j]==frame[j-1])
            k++;
        else
            k=1;

        //insert bit in the identifier table
        identifier.insert(identifier.begin(),frame[j]);
        //display
        cout <<setw(6) <<"ID" <<index
            <<" " <<frame[j]
            <<" IDENTIFIER" <<endl;

        //If 5 successive bits then jump bit
        if(k==5)
        {
            //display
            cout <<setw(8) <<"S"
                <<" " <<frame[j+1]
                <<" IDENTIFIER" <<endl;

            stuff++;
            Max++;
            k=1;
            j++;
        }
        index--;
    }

    //Read RTR or SRR
    i=j;
    RTR=frame[i];
        //Stuff bits count
        if(frame[i]==frame[i-1])
            k++;
        else
            k=1;
        if(k!=5)
        {
            IDE=frame[i+1];
            if(IDE==0) //Basic Identifier
            {
                //display
                cout <<setw(8) <<"RTR"
                    <<" " <<RTR
                    <<" IDENTIFIER" <<endl;
                i++; //Move to Control Field
            }
            else if(IDE==1) //extended Identifier
            {
                SRR=frame[i];
                //display
                cout <<setw(8) <<"SRR"
                    <<" " <<SRR
                    <<" IDENTIFIER" <<endl;
                i++; //Move to IDE
            }
        }
        if(k==5)
        {
            IDE=frame[i+2];
            if(IDE==0) //Basic Identifier
            {
                //display
                cout <<setw(8) <<"RTR"
                    <<" " <<RTR
                    <<" IDENTIFIER" <<endl;
            }
            else if(IDE==1) //extended Identifier
            {
                SRR=frame[i];
                //display
                cout <<setw(8) <<"SRR"
                    <<" " <<SRR
                    <<" IDENTIFIER" <<endl;
            }
            //display
            cout <<setw(8) <<"S"
                <<" " <<frame[i+1]
                <<" IDENTIFIER" <<endl;
            stuff++;
            k=1;
            i+=2; //move to IDE or control Field
        }

    if(IDE==1) //Extended Identifier
    {
        IDE=frame[i];
            //display
            cout <<setw(8) <<"IDE"
                <<" " <<IDE
                <<" IDENTIFIER" <<endl;
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
                //display
                cout <<setw(8) <<"S"
                        <<" " <<frame[i+1]
                        <<" IDENTIFIER" <<endl;
                stuff++;
                k=1;
                i++;
            }
            //IDE must be recessive
            if(IDE==0)
            {
                cout <<"Form Error: IDE must be recessive" <<endl <<endl;
                return 0;
            }

        //move to identifier extension
        i++;
        Max=18;
        index=17;
        for(j=i;j<i+Max;j++)
        {
            //Stuff bits count
            if(frame[j]==frame[j-1])
                k++;
            else
                k=1;

            //insert bit in the identifier table
            identifier.insert(identifier.begin(),frame[j]);
            //display
            if(index>=10)
                cout <<setw(6) <<"ID" <<index;
            else if(index<10)
                cout <<setw(7) <<"ID0" <<index;
            cout <<" " <<frame[j]
                 <<" IDENTIFIER" <<endl;

            //Stuff bits count
            if(k==5)
            {
                //display
                cout <<setw(8) <<"S"
                        <<" " <<frame[j+1]
                        <<" IDENTIFIER" <<endl;
                stuff++;
                k=1;
                Max++;
                j++;
            }
            index--;
        }

        //Read RTR/RRS
        i=j;
        if(IDE==0) //CAN
        {
            RTR=frame[i];
                //display
                cout <<setw(8) <<"RTR"
                     <<" " <<frame[i]
                     <<" IDENTIFIER" <<endl;
                //Stuff bits count
                if(frame[i]==frame[i-1])
                    k++;
                else
                    k=1;
                if(k==5)
                {
                    //display
                    cout <<setw(8) <<"S"
                            <<" " <<frame[i+1]
                            <<" IDENTIFIER" <<endl;
                    stuff++;
                    k=1;
                    i++;
                }
        }
        else if(IDE==1) //CAN FD
        {
            RRS=frame[i];RTR=frame[i];
                //display
                cout <<setw(8) <<"RRS"
                     <<" " <<frame[i]
                     <<" IDENTIFIER" <<endl;
                //Stuff bits count
                if(frame[i]==frame[i-1])
                    k++;
                else
                    k=1;
                if(k==5)
                {
                    //display
                    cout <<setw(8) <<"S"
                         <<" " <<frame[i+1]
                         <<" IDENTIFIER" <<endl;
                    stuff++;
                    k=1;
                    i++;
                }
        }
        i++; //Move to Control Field
    }
    cout <<endl;


//---------------------------------------------
//-----Control field-----------------------
//---------------------------------------------
    if(IDE==0) //basic
    {
        //display
        cout <<setw(8) <<"IDE"
             <<" " <<IDE
             <<" CONTROL" <<endl;
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
                //display
                cout <<setw(8) <<"S"
                     <<" " <<frame[i+1]
                     <<" CONTROL" <<endl;
                stuff++;
                k=1;
                i++;
            }
            //IDE must be dominant
            if(IDE==1)
            {
                cout <<"Format Error: IDE must be dominant" <<endl <<endl;
                return 0;
            }

        //move to FDF
        i++;
    }
    else if(IDE==1) //extended
    {
        if(frame[i]==0) //R1 CAN
        {
            cout <<setw(8) <<"R1"
                 <<" 0"
                 <<" CONTROL" <<endl;
                //Stuff bits count
                if(frame[i]==frame[i-1])
                    k++;
                else
                    k=1;
                if(k==5)
                {
                    //display
                    cout <<setw(8) <<"S"
                        <<" " <<frame[i+1]
                        <<" CONTROL" <<endl;
                    stuff++;
                    k=1;
                    i++;
                }
            i++; //Move to FDF
        }
    }

    //FDF
    FDF=frame[i];
        //display
        cout <<setw(8) <<"FDF"
             <<" " <<FDF
             <<" CONTROL" <<endl;
        //Stuff bits count
        if(frame[i]==frame[i-1])
            k++;
        else
            k=1;
        if(k==5)
        {
            //display
            cout <<setw(8) <<"S"
                 <<" " <<frame[i+1]
                 <<" CONTROL" <<endl;
            stuff++;
            k=1;
            i++;
        }

    if(FDF==0) //CAN
    {
        //nothing to do move to DLC after
    }
    else if(FDF==1) //CAN FD
    {
        //R0 must be dominant
        i++;
        cout <<setw(8) <<"R0"
             <<" " <<frame[i]
             <<" CONTROL" <<endl;
        if(frame[i]==1)
        {
            cout <<"Frame Error: R0 must be dominant" <<endl <<endl;
            return 0;
        }
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
               //display
                cout <<setw(8) <<"S"
                     <<" " <<frame[i+1]
                     <<" CONTROL" <<endl;
                stuff++;
                k=1;
                i++;
            }

        //Move to BRS
        i++;
        BRS=frame[i];
            //display
            cout <<setw(8) <<"BRS"
                <<" " <<BRS
                <<" CONTROL" <<endl;
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
               //display
                cout <<setw(8) <<"S"
                     <<" " <<frame[i+1]
                     <<" CONTROL" <<endl;
                stuff++;
                k=1;
                i++;
            }

        //Move to ESI
        i++;
        ESI=frame[i];
            //display
            cout <<setw(8) <<"ESI"
                <<" " <<ESI
                <<" CONTROL" <<endl;
            //Stuff bits count
            if(frame[i]==frame[i-1])
                k++;
            else
                k=1;
            if(k==5)
            {
               //display
                cout <<setw(8) <<"ESI"
                     <<" " <<ESI
                     <<" CONTROL" <<endl;
                stuff++;
                k=1;
                i++;
            }
    }


        //DLC
        i++;
        Max=i+3;
        index=3;
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
            //display
            cout <<setw(7) <<"DLC" <<index
                <<" " <<frame[j]
                <<" CONTROL" <<endl;

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
                       //display
                        cout <<setw(8) <<"S"
                             <<" " <<frame[j+1]
                             <<" CONTROL" <<endl;
                        stuff++;
                        k=1;
                        Max++;
                        j++;
                    }
                }

                else if(FDF==0)//can
                {
                   //display
                    cout <<setw(8) <<"S"
                         <<" " <<frame[j+1]
                         <<" CONTROL" <<endl;
                    stuff++;
                    k=1;
                    Max++;
                    j++;
                }

            }
            index--;
        }
        i=j;



//---------------------------------------------
//------------Data Field-----------------------
//---------------------------------------------
        if(data_length!=0)
        {
            //data field
            Max=i+(data_length*8);
            index=0;

            for(j=i;j<Max-1;j++)
            {
                //Stuff bits count
                if(frame[j]==frame[j-1])
                    k++;
                else
                    k=1;

                DATA.push_back(frame[j]);
                //display
                if((index%8)==0)
                {
                    cout <<endl;
                    (index<80)?(cout <<setw(7) <<"DATA0" <<index/8 <<" "):(cout <<setw(6) <<"DATA" <<index/8 <<" ");
                }
                cout <<frame[j] <<" ";


                //Stuff bits count
                if(k==5)
                {
                   //display
                    cout <<"(" <<frame[j+1] <<") ";
                    stuff++;
                    k=1;
                    Max++;
                    j++;
                }
                index++;
            }
            i=j;

            if(FDF==1) //CAN FD
            {
                DATA.push_back(frame[i]);
                cout <<frame[j] <<endl;
            }
            else if(FDF==0)
            {
                //Stuff bits count
                if(frame[i]==frame[i-1])
                    k++;
                else
                    k=1;

                DATA.push_back(frame[i]);
                cout <<frame[j] <<" ";

                //Stuff bits count
                if(k==5)
                {
                    //display
                    cout <<"(" <<frame[j+1] <<") ";
                    stuff++;
                    k=1;
                    i++;
                }
                cout <<endl;
            }
            i++;//move to CRC
        }
        cout <<endl;



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
        if(FDF==0) //CAN
            calculated_CRC=crc(unstuff_frame(crc_input),15);

        else if (FDF==1) //CAN FD
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
            //display
            cout <<setw(8) <<"S"
                 <<" " <<frame[i]
                 <<" CRC" <<endl;
            //Jump first fixed stuff bit
            i++;

            //Read Stuff count
            for(j=i;j<i+4;j++)
            {
                received_stuff_count.push_back(frame[j]);
                //display
                (j==i+3)?(cout <<setw(8) <<"P"):(cout <<setw(7) <<"SC" <<2-(j-i));
                cout <<" " <<frame[j]
                     <<" CRC" <<endl;
            }
            i=j; //2nd stuff bit

            //Read CRC
            if(data_length<=16) //CRC 17, 5 fixed stuff bits
            {
                Max=i+17+5;
                index=16;
            }
            else                //CRC 21, 6 fixed stuff bits
            {
                Max=i+21+6;
                index=20;
            }

            for(j=i;j<Max;j++)
            {
                //jump fixed stuff bits after every 4 bits
                if((j-i)%5!=0)
                {
                    received_CRC.push_back(frame[j]);
                    //display
                    (index<10)?(cout <<setw(7) <<"CRC0"):(cout <<setw(6) <<"CRC");
                    cout <<index
                         <<" " <<frame[j]
                         <<" CRC" <<endl;
                    index--;
                }
                else
                {
                    //display
                    cout <<setw(8) <<"S"
                         <<" " <<frame[j]
                         <<" CRC" <<endl;
                }
            }
            i=j;
        }
        else if(FDF==0)//can
        {
            Max=i+15;
            index=14;
            for(j=i;j<Max;j++)
            {
                //Stuff bits count
                if(frame[j]==frame[j-1])
                    k++;
                else
                    k=1;

                received_CRC.push_back(frame[j]);
                //display
                (index<10)?(cout <<setw(7) <<"CRC0"):(cout <<setw(6) <<"CRC");
                cout <<index
                <<" " <<frame[j]
                <<" CRC" <<endl;

                //Stuff bits count
                if(k==5)
                {
                    //display
                    cout <<setw(8) <<"S"
                         <<" " <<frame[j+1]
                         <<" CRC" <<endl;
                    stuff++;
                    k=1;
                    Max++;
                    j++;
                }
                index--;
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
                cout <<"CRC Error: Stuff count doesn't match" <<endl <<endl;
                return 0;
            }
        }
        if(compare_two_tables(received_CRC,calculated_CRC)==0)
        {
            cout <<FDF<<"CRC Error: CRC doesn't match" <<endl <<endl;
            return 0;
        }

    //----------------------------------------
    //CRC Delimiter------CANFD----------------
    //----------------------------------------
       //display
       cout <<setw(8) <<"CRC DEL"
            <<" " <<frame[i]
            <<" CRC" <<endl;
        if(frame[i]==0) //First CRC Delimiter
        {
            cout <<"Frame Error: CRC Delimiter must be Recessif" <<endl <<endl;
            return 0;
        }
        if(frame[i+1]==1) //Second CRC Delimiter
        {
            i++;
            //display
           cout <<setw(8) <<"CRC DEL"
                <<" " <<frame[i]
                <<" CRC" <<endl;
        }

        //Move to ACK
        i++;
        cout <<endl;




//---------------------------------------------
//------------ACK Field------------------------
//---------------------------------------------
    //display
    cout <<setw(8) <<"ACK SLOT"
         <<" " <<frame[i]
         <<" ACK" <<endl;
    if(frame[i]==1)
    {
        cout <<"No node acknowledged the frame" <<endl <<endl;
        return 0;
    }

    i++;
    //display
    cout <<setw(8) <<"ACK DEL"
         <<" " <<frame[i]
         <<" ACK" <<endl;
    if(frame[i]==0)
    {
        cout <<"Frame Error: ACK Delimiter must be recessive" <<endl <<endl;
        return 0;
    }

    i++;
    cout <<endl;



//---------------------------------------------
//------------EOF INTERFRAME-------------------
//---------------------------------------------
    cout <<setw(8) <<"EOF";
    for(j=i;j<i+10;j++)
    {
        //display
        cout <<" " <<frame[i];
        if(frame[j]==0)
        {
            cout <<"EOF ERROR" <<endl <<endl;
            return 0;
        }
    }
    cout <<endl <<endl;

    //General information
    FDF?cout <<"CAN FD ":cout <<"CAN ";
    IDE?cout <<"Extended ":cout <<"Base ";
    RTR?cout <<"Remote ":cout <<"Data ";
    cout <<"Frame, ";
    if(data_length==1) cout <<"1 Data Byte, ";
    else
    {
        data_length?cout <<data_length:cout <<"No";
        cout <<" Data bytes, ";
    }
    cout <<"length: " <<frame.size()/8 <<" bytes";
    cout <<endl <<endl;


//---------------------------------------------
//------------------Free-----------------------
//---------------------------------------------
identifier.clear();DLC.clear();DATA.clear();received_stuff_count.clear();received_CRC.clear();
crc_input.clear();calculated_stuff_count.clear();calculated_CRC.clear();

    return 1;
}
