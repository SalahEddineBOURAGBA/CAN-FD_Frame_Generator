#include "generator_functions.h"


/*------------------------------------------------------------------------------
--------------------------------CRC Calculation---------------------------------
------------------------------------------------------------------------------*/

//calculate CRC of a frame
//In CAN, the input is the no-stuffed sub frame (from SOF to last Data byte)
//In CAN FD, the input is the dynamic stuffed sub frame (from SOF to last Data byte)
embeddedvector <bool> crc(const embeddedvector <bool>& input, const unsigned int crc_version)
{
    embeddedvector <bool> zeros (crc_version,0);
    embeddedvector <bool> crc_output(crc_version,0);
    embeddedvector <bool> crc_input=input;
    
    unsigned int i,j,loop;

    //Modify crc init in CRC-17 and CRC-21
    if(crc_version==17 || crc_version==21)
        crc_output[0]=1;

    //Add 'crc_version' zeros to the end of crc_input
    crc_input.append(zeros.getData(), zeros.length());
    loop=crc_input.length();
    
    //calculate crc
    for(i=0;i<loop;i++)
    {
        //if MSB of crc_bool output[4]=0 then shift left
        if(crc_output[0]==0)
        {
            for(j=0;j<crc_version-1;j++)
                crc_output[j]=crc_output[j+1];

            crc_output[crc_version-1]=crc_input[0];

            crc_input.removeAtIndex(0);
        }

        //if MSB of crc_bool output[4]=1 then shift left and xor with crc polynomial
        else if(crc_output[0]==1)
        {
            //xor with polynomial depending on crc_version
            if(crc_version==15)
            {
                for(j=0;j<crc_version-1;j++)
                {
                    if(j==0 || j==4 || j==6 || j==7 || j==10 || j==11)
                        crc_output[j]=crc_output[j+1]^1;
                    else
                        crc_output[j]=crc_output[j+1];
                }
            }
            else if(crc_version==17)
            {
                for(j=0;j<crc_version-1;j++)
                {
                    if(j==0 || j==2 || j==3 || j==5 || j==10 || j==12 || j==13 || j==15)
                        crc_output[j]=crc_output[j+1]^1;
                    else
                        crc_output[j]=crc_output[j+1];
                }
            }
            else if(crc_version==21)
            {
                for(j=0;j<crc_version-1;j++)
                {
                    if(j==0 || j==7 || j==9 || j==13 || j==16 || j==17 || j==19)
                        crc_output[j]=crc_output[j+1]^1;
                    else
                        crc_output[j]=crc_output[j+1];
                }
            }

            crc_output[crc_version-1]=crc_input[0]^1;

            crc_input.removeAtIndex(0);
        }
    }

    return (crc_output);
}

//calculate stuff count of a dynamic stuffed sub frame
embeddedvector <bool> stuff_bit_count(const unsigned int stuff)
{
    unsigned int stuff_mod_8 = stuff % 8;
    bool output[4];
    
    //return stuff count as follows
    //bit2 bit1 bit0 parity
    switch(stuff_mod_8)
    {
    case 0:
        output[0]=0;
        output[1]=0;
        output[2]=0;
        output[3]=0;
        break;
    case 1:
        output[0]=0;
        output[1]=0;
        output[2]=1;
        output[3]=1;
        break;
    case 2:
        output[0]=0;
        output[1]=1;
        output[2]=1;
        output[3]=0;
        break;
    case 3:
        output[0]=0;
        output[1]=1;
        output[2]=0;
        output[3]=1;
        break;
    case 4:
        output[0]=1;
        output[1]=1;
        output[2]=0;
        output[3]=0;
        break;
    case 5:
        output[0]=1;
        output[1]=1;
        output[2]=1;
        output[3]=1;
        break;
    case 6:
        output[0]=1;
        output[1]=0;
        output[2]=1;
        output[3]=0;
        break;
    case 7:
        output[0]=1;
        output[1]=0;
        output[2]=0;
        output[3]=1;
        break;
    }

    embeddedvector <bool> result(output,4);
    return result;
}



/*------------------------------------------------------------------------------
--------------------------------Stuffing----------------------------------------
------------------------------------------------------------------------------*/

//Stuff a CAN or CAN FD frame
unsigned int stuff_frame(embeddedvector <bool>& frame, bool can)
{
    unsigned int i, k(1), nbr_stuff_bits(0);

    //In CAN FD don't stuff the last 5 consecutive bits
    //that ends at the end of the frame
    for(i=1;i<frame.length()-1;i++)
    {
        if(frame[i] == frame[i-1])
        {
            k++;
            if(k==5)
            {
                frame.insertAtIndex(i+1,!frame[i]);
                k=1;
                nbr_stuff_bits++;
                i++;
            }
        }
        else
            k=1;
    }

    //Take in consideration the last 5 bits in CAN
    //There is no fixed stuffing
    if(can)
    {
        if(frame[i] == frame[i-1])
        {
            k++;
            if(k==5)
            {
               frame.insertAtIndex(i+1,!frame[i]);
               nbr_stuff_bits++;
            }
        }
    }

    return nbr_stuff_bits;
}

//stuff the CRC Field of a CAN FD frame
embeddedvector <bool> stuff_crc(embeddedvector <bool> const& stuff_count, embeddedvector <bool> const& crc, const bool first_bit)
{
    embeddedvector <bool> output;
    unsigned int i(0), k(0);

    //1st fixed stuff bit
    output.push(!first_bit);
    //Stuff count
    output.append(stuff_count.getData(), stuff_count.length());
    //2nd fixed stuff bit
    output.push(!output[output.length()-1]);
    //Add the CRC and stuff it after
    output.append(crc.getData(), crc.length());

    //Fixed stuff CRC
    for(i=6;i<output.length();i++)
    {
        k++;
        if(k==4)
        {
            k=0;
            output.insertAtIndex(i+1,!output[i]);
            i++;
        }

    }
    return output;
}

//For CAN, CRC calculation is used on unstuffed frame
embeddedvector <bool> unstuff_frame(embeddedvector <bool> const& input)
{
    embeddedvector <bool> frame=input;
    unsigned int k(1), i;

    for(i=1;i<frame.length()-1;i++)
    {
        if(frame[i] == frame[i-1])
        {
            k++;
            if(k==5)
            {
                k=1;
                if(frame[i+1] == frame[i+2])
                    k++;
                frame.removeAtIndex(i+1);
                i++;
            }
        }
        else
            k=1;
    }

    return frame;
}



/*------------------------------------------------------------------------------
------------------------------Generate a frame----------------------------------
------------------------------------------------------------------------------*/

embeddedvector <bool> generate_frame(const CANFDFrame &inFrame)
{
    bool SOF(0), SRR(1), R0(0), R1(0);
    unsigned int nbr_stuff_bits, i;
    embeddedvector <bool> stuff_count, CRC, stuffed_crc;
    embeddedvector <bool> frame;

    //SOF
    frame.push(SOF);

    //Identifier Field
    if(inFrame.getIDE()==0)//base format
    {
      for(i=0;i<11;i++)
        frame.push(inFrame.getID() & (0x00000001 << (10-i)));
    }

    else if(inFrame.getIDE()==1)//Extended format
    {
        for(i=0;i<11;i++)
          frame.push(inFrame.getID() & (0x00000001 << (28-i)));
        //SRR
        frame.push(SRR);
        frame.push(inFrame.getIDE());
        for(i=11;i<29;i++)
                frame.push(inFrame.getID() & (0x00000001 << (28-i)));
    }
    frame.push(inFrame.getRTR());

    if(inFrame.getFDF()==0)//CAN
    {
        if(inFrame.getIDE()==0)
            frame.push(inFrame.getIDE());
        else
            frame.push(R1);
        frame.push(R0);
    }

    else if (inFrame.getFDF()==1) //CAN FD
    {
        if(inFrame.getIDE()==0)
            frame.push(inFrame.getIDE());

        //Control Field
        frame.push(inFrame.getFDF());
        frame.push(R0);
        frame.push(inFrame.getBRS());
        frame.push(inFrame.getESI());
    }

    for(i=0;i<4;i++) 
      frame.push(inFrame.getDLC() & (0x01 << (3-i)));

    //DATA Field
    for(i=0;i<dataLengthFromDLC(inFrame.getDLC());i++)
      for(unsigned j=0;j<8;j++)
            frame.push(inFrame.getData()[i] & (0x01 << (7-j)));
    

    //CRC Field
    if(inFrame.getFDF()==0) //CAN
    {
        //In CAN calculate CRC then stuff frame
        CRC=crc(frame,15);
        frame.append(CRC.getData(), CRC.length());

        //the second parameter is '1' to stuff the hole frame
        stuff_frame(frame,1);
    }

    else if (inFrame.getFDF()==1) //CAN FD
    {
        //Stuff the frame before calculating CRC and return stuff count
        //the second parameter is '0' to stuff the hole frame except the last 5 bits
        nbr_stuff_bits=stuff_frame(frame,0);

        //Stuff count
        stuff_count=stuff_bit_count(nbr_stuff_bits);      
        
        if(dataLengthFromDLC(inFrame.getDLC())<17)
          CRC=crc(frame,17);
        else
          CRC=crc(frame,21);

        //Fixed stuffed CRC Field
        stuffed_crc=stuff_crc(stuff_count,CRC,frame[frame.length()-1]);
        //add fixed stuffed CRC to the frame
        frame.append(stuffed_crc.getData(), stuffed_crc.length());
    }

    //CRC Delimiter
    frame.push(1);

    //ACK
    frame.push(1);
    frame.push(1);

    //EOF
    for(i=1;i<=10;i++)
        frame.push(1);

    return(frame);
}
