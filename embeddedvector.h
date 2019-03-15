#ifndef EMBEDDEDVECTOR_H
#define EMBEDDEDVECTOR_H

#include <cstring>
#include <stdlib.h>
  
template <class TYPE> class embeddedvector 
{
	//private properties
	private: TYPE * mData;
	private: size_t mLength;
	private: size_t mSize;

	//accessors
	public: inline size_t length (void) const { return mLength; }
	public: inline TYPE* getData (void) const { return mData; }

	//default constructor
	public: embeddedvector (void) : mData (nullptr), mLength (0), mSize (20) 
	{
		mData = new TYPE [mSize];
	}

	//constructor: allocate inCount elements initialized with inData
	public: embeddedvector (const size_t inCount, const TYPE inData) : mData (nullptr),	mLength (inCount), mSize (inCount + 20) 
	{
		mData = new TYPE [mSize];
		for (size_t i=0 ; i<inCount ; i++) 
		{
			mData [i] = inData;
		}
	}

	//constructor: allocate inCount elements of a table
	public: embeddedvector (const TYPE inArray [], const size_t inCount) :	mData (nullptr), mLength (inCount),	mSize (inCount + 20)
	{
		mData = new TYPE [mSize] ;
		for (size_t i=0 ; i<mLength ; i++) 
		{
			mData[i] = inArray [i] ;
		}
	}

	//destructor
	public: virtual ~embeddedvector (void) 
	{
	    delete [] mData ; mData = nullptr ;
	    mLength = 0;
	    mSize = 0 ;
	}

	//clear a table and free memory
	public: void destroy (void) 
	{
		delete [] mData ; mData = nullptr ;
	    mLength = 0;
	    mSize = 0 ;
	}

	//remove an element from a fixed position
	public: void removeAtIndex (const size_t inIndex) 
	{
    		if (inIndex < mLength) 
	    	{
			mLength -- ;
			for (size_t i=inIndex ; i<mLength ; i++) 
			{
				mData [i] = mData [i+1] ;
			}
	    	}
	}

	//remove successive elements between two positions
	public: void removeAtIndex (const size_t inStartIndex, const size_t inLastIndex) 
	{
		if ((inStartIndex <= inLastIndex) && (inLastIndex < mLength)) 
		{
			mLength -= inLastIndex - inStartIndex + 1;
			for (size_t i=inStartIndex ; i<mLength ; i++) 
			{
				mData [i] = mData [i+ inLastIndex - inStartIndex + 1] ;
			}
		}
	}

	//reverse the elemnts of the table
	public: void reverse (void) 
	{
		if (mLength > 1) 
		{
			size_t lowIndex = 0 ;
			size_t highIndex = mLength - 1 ;
			while (lowIndex < highIndex) 
			{
				const TYPE temp = mData [lowIndex] ;
				mData[lowIndex] = mData [highIndex] ;
				mData[highIndex] = temp ;
				lowIndex ++ ;
				highIndex -- ;
			}
		}
	}

	//clear the table (does not free space)
	public: void clear (void) 
	{
		mLength = 0 ;
	}

	//change the size of the table (does not change mLength)
	void resize (const size_t inNewSize) 
	{
		if (inNewSize == 0) 
		{
			destroy () ;
		}
		else if (inNewSize > mSize) 
		{
			TYPE * newData = new TYPE [inNewSize] ;
			for (size_t i=0 ; i<mLength ; i++) 
			{
				newData [i] = mData [i];
			}
			delete [] mData ; mData = newData ;
			mSize = inNewSize ;
		}
	}

	//add an element to the end of the table
	public: void push (const TYPE inData) 
	{
	    resize (mLength + 1) ;
		mData [mLength] = inData ;
	    mLength ++ ;
	}

	 //add an element to a fixed position
  	public: void insertAtIndex (const size_t inIndex, const TYPE inData) 
  	{
    	resize (mLength + 1) ;
		for (size_t i=mLength ; i>inIndex ; i--) 
		{
			mData [i] = mData [i-1] ;
		}
		mData [inIndex] = inData ;
    	mLength ++ ;
	}

	//add inCount elements from a table to the end of the current table
	public: void append (const TYPE inTable [], const size_t inCount) 
	{
	    resize (mLength + inCount) ;
	    for (size_t i=0 ; i<inCount ; i++) 
	    {
	      mData [mLength + i] = inTable [i] ;
	    }
	    mLength += inCount ;
	}

	//swap 2 elements of the table
	public: void swap (const size_t inIndex1, const size_t inIndex2) 
	{
		const TYPE temp = mData[inIndex1] ;
		mData[inIndex1] = mData[inIndex2] ;
		mData[inIndex2] = temp ;
	}

	//operator overloading
	public: embeddedvector & operator += (embeddedvector <TYPE> const & inOther) 
	{
		if (inOther.mLength > 0) 
		{
		  this->append (inOther.mData, inOther.mLength) ;
		}
		return (*this);
	}

	//operator overloading
	public: embeddedvector & operator = (embeddedvector <TYPE> const & inOther) 
	{
		if (this != &inOther) 
		{
			resize (inOther.mLength) ;
			for (size_t i=0 ; i<inOther.mLength ; i++) 
			{
				mData [i] = inOther.mData [i] ;        
			}
			mLength = inOther.mLength ;
		}
		return (*this) ;
	}

	//copy constructor
	public: embeddedvector (const embeddedvector <TYPE> & inOther) :
	mData (new TYPE [inOther.mSize]),
	mLength (inOther.mLength),
	mSize (inOther.mSize) 
	{
		for (size_t i=0 ; i<inOther.mLength ; i++) 
		{
			mData [i] = inOther.mData [i] ;
		}
	}

 	//get an element of the table
	public: TYPE & operator [] (const size_t inIndex) const 
	{
		return mData [inIndex] ;
	}

} ;

#endif //EMBEDDEDVECTOR_H
