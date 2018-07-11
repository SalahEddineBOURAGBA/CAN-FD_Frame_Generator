#include "CANFDFrame.h"

//default constructor
CANFDFrame::CANFDFrame(): mID(0), mIDE(0), mFDF(0), mRTR(0), mBRS(0), mESI(0), mDLC(0)
{
}

//random frame constructor
CANFDFrame::CANFDFrame(const bool x)
{
    //Seed srand
    struct timeval time;
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec));

    //generate random values for FDF IDE BRS ESI
    mFDF=((rand()%100)>50)?1:0;
    mIDE=((rand()%100)>50)?1:0;
    mBRS=((rand()%100)>50)?1:0;
    //mESI=((rand()%100)>50)?1:0;
    mESI=0;
    //mRTR=((rand()%100)>50)?1:0;
    mRTR=0;

    //Identifier Field
    mID = 0;
    for(unsigned int i=0;i<11;i++)
    {
        if((rand()%100)>50)
            mID |= (0x1 << i);
    }

    if(mIDE==1)//Extended frame
    {
        for(unsigned int i=11;i<29;i++)
        {
            if((rand()%100)>50)
                mID |= (0x1 << i);
        }
    }

    //DLC Field
    mDLC = 0;
    for(unsigned int i=0;i<4;i++)
    {
        if((rand()%100)>50)
                mDLC |= (0x1 << i);
    }

    //DATA Field
    unsigned int length = dataLengthFromDLC(mDLC);
    if(mFDF == 0) //CAN
        if(length > 8)
            length = 8;

    for(unsigned int i=0;i<length;i++)
    {
        mData.push(0);
        for(unsigned int j=0;j<8;j++)
        {
            if((rand()%100)>50)
                mData[i] |= (0x1 << j);
        }
    }
}

//wanted frame constructor
CANFDFrame::CANFDFrame(const bool inFDF, const bool inIDE, const bool inRTR, const uint32_t inID, const bool inBRS,
               const bool inESI, const uint8_t inDataLength, embeddedvector <uint8_t> const& inData):
mID(inID), mIDE(inIDE), mFDF(inFDF), mRTR(inRTR), mBRS(inBRS), mESI(inESI)               
{
    //DLC
     switch(inDataLength) //////////////rethink
    {
    case 12:
        mDLC = 0x09;
    case 16:
        mDLC = 0x0A;
    case 20:
        mDLC = 0x0B;
    case 24:
        mDLC = 0x0C;
    case 32:
        mDLC = 0x0D;
    case 48:
        mDLC = 0x0E;
    case 64:
        mDLC = 0x0F;
    default:
        mDLC = inDataLength;
    }
    
    //Data
    for(unsigned int i=0;i<inDataLength;i++)
        mData.push(inData[i]);
}

CANFDFrame::~CANFDFrame()
{
  mData.destroy();
}

//surcharge '=' operator to copy elements
CANFDFrame& CANFDFrame::operator = (const CANFDFrame &other)
{
  if(this != &other)
  {
    mID  = other.mID;
    mFDF = other.mFDF;
    mIDE = other.mIDE;
    mRTR = other.mRTR;
    mBRS = other.mBRS;
    mESI = other.mESI;
    mDLC = other.mDLC;
    mData.clear();
    for(unsigned int i=0;i<other.mData.length();i++)
      mData.push(other.mData[i]);
  }
  return *this;
}

//surcharge "==" operator to do boolean comparison
bool CANFDFrame::operator == (const CANFDFrame &frame)
{
  if(mID!=frame.mID || mFDF!=frame.mFDF || mIDE!=frame.mIDE || mRTR!=frame.mRTR
     || mBRS!=frame.mBRS || mESI!=frame.mESI || mDLC!=frame.mDLC)
     return false;
  
  for(unsigned int i=0;i<dataLengthFromDLC(frame.mDLC);i++)
    if(mData[i]!=frame.mData[i])
      return false;

  return true;
}

uint8_t dataLengthFromDLC(const uint8_t inDLC)
{
    switch(inDLC)
    {
    case 0x09:
        return 12;
    case 0x0A:
        return 16;
    case 0x0B:
        return 20;
    case 0x0C:
        return 24;
    case 0x0D:
        return 32;
    case 0x0E:
        return 48;
    case 0x0F:
        return 64;
    default:
        return inDLC;
    }
}