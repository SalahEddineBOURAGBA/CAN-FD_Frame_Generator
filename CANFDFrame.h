#ifndef CANFDFRAME_H
#define CANFDFRAME_H


#include <cstdint>
#include <stdlib.h>
#include <sys/time.h>
#include "embeddedvector.h"

class CANFDFrame
{
private:
    //members
    uint32_t mID; //Identifier
    bool mIDE; //0: standard ID, 1: extended ID
    bool mFDF; //0: CAN2.0B frame, 1: CANFD frame
    bool mRTR; //0: data frame, 1: remote frame
    bool mBRS; //0: no switching between bit rates, 1: switch between arbitration and data bit rates
    bool mESI; //0: active error, 1: passive error
    uint8_t mDLC; //data length
    embeddedvector <uint8_t> mData; //data bytes //0..64

public:
    //default constructor
    CANFDFrame(); 
    //random frame constructor
    CANFDFrame(const bool x);
    //specific frame constructor
    CANFDFrame(const bool inFDF, const bool inIDE, const bool inRTR, const uint32_t inID, const bool inBRS,
               const bool inESI, const uint8_t inDataLength, embeddedvector <uint8_t> const& inData);
    //destructor
    ~CANFDFrame();

    //surcharge operator '='
    CANFDFrame& operator = (const CANFDFrame &other);
    //surcharge operator '=='
    bool operator == (const CANFDFrame &frame);

    //accessors 
    inline uint32_t getID (void) const {return mID;}
    inline bool getIDE(void) const {return mIDE;}
    inline bool getFDF(void) const {return mFDF;}
    inline bool getRTR(void) const {return mRTR;}
    inline bool getBRS(void) const {return mBRS;}
    inline bool getESI(void) const {return mESI;}
    inline uint8_t getDLC(void) const {return mDLC;}
    inline embeddedvector <uint8_t> getData(void) const {return mData;}

    //no copy
private: CANFDFrame (const CANFDFrame& inOther);
};

uint8_t dataLengthFromDLC(const uint8_t inDLC);


#endif // CANFDFRAME_H