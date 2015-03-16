#ifndef BITSEQUENCE_H
#define BITSEQUENCE_H

#include <assert.h>
#include "integer.h"
#include <stdlib.h>		//For malloc.
#include "ArrayList.h"
#include "SinglyLinkedList.h"

/*Size of long long ints, in bits.*/
#define lintsize (sizeof(unsigned long long int)*8)
#define intsize  (sizeof(unsigned int)*8)
#define bytesize (sizeof(char)*8)

/*Log size of long long ints, in bits.*/
#define lintlog	Integer::BinaryLog(lintsize)

#define EXCEPTIONS_ENABLED

namespace Juice{
	class BitSequence{
	private:
		/*A pointer to the data to be referenced.*/
		unsigned long long int * data;
		/*The length of the segment, in bits.*/
		unsigned long long int len;

	public:

		/*Default constructor.  Exists solely for allocation within arrays or some goofy stuff like that.*/
		BitSequence(){
			data = (unsigned long long int *)malloc(0);
			len = 0;
		}

		BitSequence(unsigned long long int segLength){  //tested
			len = segLength;
			data = (unsigned long long int *)malloc(lengthLints()*lintsize/bytesize);
			
			if(len%lintsize)
				clearOutOfScopeBits();
		}

		BitSequence(void * byteAddress, unsigned long long int segLength){						//This constructor is dated.  Waiting to replace with the 3-param constructor.
			len = segLength;
			data = (unsigned long long int *)malloc(lengthLints()*lintsize/bytesize);

			if(!len)	return;

			if(segLength < 64){
				unsigned long long int numBytes = segLength / bytesize + (segLength % bytesize ? 1 : 0);
				
				unsigned char * srcBytes = (unsigned char*)byteAddress;
				unsigned char * dataBytes = (unsigned char*)data;

				for(unsigned long long int i = 0ULL; i < numBytes; ++i)
					dataBytes[i] = srcBytes[i];
			}else{
				unsigned long long int numLints = segLength / lintsize;
				unsigned long long int extaBytes = segLength / bytesize + (segLength % bytesize ? 1 : 0) - numLints * lintsize / bytesize;

				unsigned char * srcBytes = (unsigned char*)byteAddress;
				unsigned char * dataBytes = (unsigned char*)data;

				unsigned long long int * srcLints = (unsigned long long int *)byteAddress;

				for(unsigned long long int lintIndex = 0ULL; lintIndex < numLints; ++lintIndex)
					data[lintIndex] = srcLints[lintIndex];
				
				unsigned long long int * shiftedLastLint = (unsigned long long int*)(((unsigned char*)(data+numLints-1))+extaBytes);
				*shiftedLastLint = *((unsigned long long int*)(((unsigned char*)byteAddress)+numLints*lintsize/bytesize-lintsize/bytesize+extaBytes));
			}

			clearOutOfScopeBits();
		}

		BitSequence(void * byteAddress, unsigned char bitAddress, unsigned long long int segLength){	//tested
			assert(bitAddress < bytesize && "bitAddress cannot exceed the number of addressable bits within a byte.");

			len = segLength;
			data = (unsigned long long int *)malloc(lengthLints()*lintsize/bytesize);

			if(!len)	return;

			if(segLength + bitAddress < 64){
				unsigned long long int numBytes = segLength / bytesize + (segLength % bytesize ? 1 : 0);
				
				unsigned char * srcBytes = (unsigned char*)byteAddress;
				unsigned char * dataBytes = (unsigned char*)data;
				
				bool lastByteIsolatedShift = (bitAddress+segLength)%bytesize > bitAddress || bitAddress==0;

				for(unsigned long long int i = 0ULL; i+1 < numBytes + !lastByteIsolatedShift; ++i)
					dataBytes[i] = (srcBytes[i]>>bitAddress) | (srcBytes[i+1] << (bytesize-bitAddress));

				if(lastByteIsolatedShift)
					dataBytes[numBytes-1] = srcBytes[numBytes-1] >> bitAddress;
			}else{
				unsigned long long int numLints = ((segLength / bytesize) + (segLength%bytesize?1:0)) * bytesize / lintsize;	//You do it roundabout because you have full memory access to some lints without having full access to all the bits (a lint of 60 uses 8 bytes, but isn't technically a full lint)
				unsigned long long int extaBytes = segLength / bytesize + (segLength % bytesize ? 1 : 0) - numLints * lintsize / bytesize;

				unsigned char * srcBytes = (unsigned char*)byteAddress;
				unsigned char * dataBytes = (unsigned char*)data;

				unsigned long long int * srcLints = (unsigned long long int *)byteAddress;

				for(unsigned long long int lintIndex = 0ULL; lintIndex < numLints; ++lintIndex)
					data[lintIndex] = (srcLints[lintIndex] >> bitAddress) | (((unsigned long long int)srcBytes[(lintIndex+1)*lintsize/bytesize]) << (lintsize - bitAddress));
				
				/*I made this because I was tired of screwing around with the byte-shifting.  Just a lambda funtion to do simple pointer byte-manipulation for a long int.*/
				auto shiftPtr = [](void * pointer, long long int shiftBytes) -> unsigned long long int* {
					return (unsigned long long int*)(((char*)pointer)+shiftBytes);
				};

				unsigned long long int * lastPtr = shiftPtr(data+numLints-1 , extaBytes);
				*lastPtr = (*shiftPtr(byteAddress, extaBytes + (numLints-1) * lintsize / bytesize) >> bitAddress) | (*shiftPtr(byteAddress, extaBytes + numLints * lintsize / bytesize) << (lintsize-bitAddress));
			}

			clearOutOfScopeBits();
		}
	
		/*The copy constructor allows for deep copies of this object.  When this object is passed through a function or set equal to another variable, the data it points to will also be copied.*/
		BitSequence(BitSequence& source){	//Untested
			len = source.Length();
			data = (unsigned long long int *)malloc(lengthLints()*bytesize);

			for(unsigned long long int i = 0ULL; i < lengthLints(); ++i)
				data[i] = source.data[i];
		}
	
		/*The assignment operator works similarly to the copy constructor.*/
		BitSequence& operator=(BitSequence& source){	//Tested a little (in application, no benches)
			len = source.len;			//Reset length
			free(data);					//Clear old
			data = (unsigned long long int *)malloc(lengthLints()*lintsize/bytesize);		//Allocate new memory
			unsigned long long int lenLints = lengthLints();					//Opvar
			for(unsigned long long int i = 0ULL; i < lenLints; ++i)		//Copy new data
				data[i] = source.data[i];
			return *this;												//Return reference to this
		}

		~BitSequence(){ //tested by extension of Constructor. (If this doesn't work, it would obviously crash)
			free(data);
		}

		/*NOTE:  This method does not clear data after expansion.  They will have to be manually cleared.*/
		void Length(unsigned long long int newLength){	//no testing needed
			len = newLength;

			data = (unsigned long long int*)realloc(data, lengthLints()*lintsize/bytesize);

			if(len%lintsize)	//length may be zero, and you can't access any memory.
				clearOutOfScopeBits();
		}
		unsigned long long int Length(){	            //no testing needed
			return len;
		}

		void Clear(){
			for(unsigned long long int i = 0; i < lengthLints(); ++i) data[i] = 0ULL;
		}

		void Clear(unsigned long long int index){	//tested
			#ifdef EXCEPTIONS_ENABLED
			if (index > len){
				throw "Provided address exceeds the length of the sequence.";
			}
			#endif

			*adr(index) &= ~(1ULL<<pos(index));
		}

		void Clear(unsigned long long int index, unsigned long long int segLength)	//tested
		{	
			#ifdef EXCEPTIONS_ENABLED
			if(index + segLength > len){
				throw "Provided address exceeds the length of the sequence.";
			}
			#endif
			
			if(segLength == 0)
				return;

			unsigned long long int * firstInt = data + (index / lintsize);
			unsigned long long int * lastInt = data + ((index+segLength-1) / lintsize);

			unsigned long long int fp = ~((~0ULL) <<  (index%lintsize));
			unsigned long long int sp = ~((~0ULL) >> ((lintsize-((index+segLength)%lintsize)))%lintsize);

			if(firstInt==lastInt)	
				*firstInt &= (fp | sp);
			else{
				*firstInt &= fp;	//set first integer
				*lastInt &= sp;		//Set the last int

				for(++firstInt; firstInt < lastInt; ++firstInt)
					*firstInt = 0;
			}
		}

		void Set(unsigned long long int index){	    //tested

			#ifdef EXCEPTIONS_ENABLED
			if (index > len){
				throw "Provided address exceeds the length of the sequence.";
			}
			#endif

			*adr(index) |= 1ULL<<pos(index);
		}

		void Set(unsigned long long int index, unsigned long long int segLength){	//tested

			#ifdef EXCEPTIONS_ENABLED
			if (index + segLength > len){
				throw "Provided address exceeds the length of the sequence.";
			}
			#endif

			if(segLength == 0)
				return;

			unsigned long long int * firstInt = data + (index / lintsize);
			unsigned long long int * lastInt = data + ((index+segLength-1) / lintsize);

			unsigned long long int fp = (~0ULL) <<  (index%lintsize);
			unsigned long long int sp = ((~0ULL) >> ((lintsize-((index+segLength)%lintsize)))%lintsize);

			if(firstInt==lastInt)	
				*firstInt |= (fp & sp);
			else{
				*firstInt |= fp;	//set first integer
				*lastInt |= sp;		//Set the last int

				for(++firstInt; firstInt < lastInt; ++firstInt)
					*firstInt = ~0ULL;
			}
		}

		bool Read(unsigned long long int index){	    //tested

			#ifdef EXCEPTIONS_ENABLED
			if (index >= len){
				throw "Provided address exceeds the length of the sequence.";
			}
			#endif

			return	Integer::Read(*adr(index), pos(index));
		}

		BitSequence Read(unsigned long long int index, unsigned long long int segLength){	//A temporary read-multiple.  Tested.

			#ifdef EXCEPTIONS_ENABLED
			if (index >= len){
				throw "Provided address exceeds the length of the sequence.";
			}
			#endif

			return BitSequence(((unsigned char*)data)+index/bytesize, index%bytesize, segLength);
		}

		//Temporary replacement of Write
		void Write(unsigned long long int index, BitSequence& source){

			#ifdef EXCEPTIONS_ENABLED
			if (index >= len){
				throw "Provided address exceeds the length of the sequence.";
			}
			#endif

			if(!source.len)	return;

			unsigned long long int * firstInt = data + (index / lintsize);
			unsigned long long int * lastInt = data + ((index+source.len-1) / lintsize);

			unsigned long long int fp = ~(-1ULL << (index%lintsize));
			unsigned long long int sp = (index+source.len)%lintsize ? -1ULL << ((index+source.len)%lintsize) : 0;

			if(firstInt==lastInt){
				*firstInt &= (fp | sp);
				*firstInt |= (*source.data) << (index%lintsize);
			}
			else
			{
				*firstInt &= fp;	//clear bits in the first integer
				*lastInt &= sp;		//clear bits in the last integer

				*firstInt |= (*source.data) << (index%lintsize);

				unsigned long long int * lastSourceInt = source.data+source.lengthLints()-1;

				if((source.len-1)%lintsize >= (source.len+index-1)%lintsize) 
					*lastInt |= *lastSourceInt >> (-index%lintsize);
				else
					*lastInt |= (lastSourceInt[-1] >> (lintsize-index%lintsize)) | (*lastSourceInt << (index%lintsize));

				for(unsigned long long int i = 1ULL; i < lastInt-firstInt; ++i)
					firstInt[i] = (source.data[i-1]>>(lintsize-index%lintsize)) | (source.data[i]<<(index%lintsize));
			}
			/**/
		}

		void ShiftUp(unsigned long long int offset){	//UNTESTED
			#ifdef EXCEPTIONS_ENABLED
			if (offset >= len)	//This algorithm will step out of the memory bounds.
			{
				Clear();
				return;
			}
			#endif

			unsigned long long int offsetBits = offset % lintsize;
			unsigned long long int offsetLints = offset / lintsize;

			if(offsetBits)
			{
				for(unsigned long long int i = lengthLints()-1; i > offsetLints; --i)
					data[i] = (data[i-offsetLints-1]>>(lintsize - offsetBits))|(data[i-offsetLints]<<offsetBits);
			
				data[offsetLints] = data[0]<<offsetBits;
			} else {
				for(unsigned long long int i = lengthLints()-1; i > offsetLints; --i)
					data[i] = data[i-offsetLints];
			}

			for(unsigned long long int i = 0ULL; i < offsetLints; ++i)
				data[i] = 0ULL;

			if(len%64)
				clearOutOfScopeBits();
		}

		void ShiftDown(unsigned long long int offset){	//UNTESTED
			
			#ifdef EXCEPTIONS_ENABLED
			if (offset >= len)	//This algorithm will step out of the memory bounds.
			{
				Clear();
				return;
			}
			#endif

			unsigned long long int offsetBits = offset % lintsize;
			unsigned long long int offsetLints = offset / lintsize;

			if(offsetBits)
			{
				for(unsigned long long int i = 0; i < lengthLints()-offsetLints-1; ++i)
					data[i]=(data[i+offsetLints]>>offsetBits)|(data[i+offsetLints+1]<<(lintsize-offsetBits));
			}
			else
			{
				for(unsigned long long int i = 0; i < lengthLints()-offsetLints-1; ++i)
					data[i]=data[i+offsetLints];
			}

			for(unsigned long long int i = lengthLints()-offsetLints; i < lengthLints(); ++i)
				data[i] = 0;
			
			data[lengthLints()-offsetLints-1] >>= offsetBits;

			Clear(len - offset, offset);
		}

		bool operator==(BitSequence& compare){ //untested
			if(compare.len!=len)
				return false;

			unsigned long long int lenLints = lengthLints();
			
			for(unsigned long long int i = 0ULL; i < lenLints; ++i){
				if(compare.data[i]!=data[i])
					return false;
			}

			return true;
		}
		bool operator!=(BitSequence& compare){	//untested
			if(compare.Length()!=Length())
				return true;

			for(unsigned long long int i = 0; i < lengthLints(len); ++i){
				if(compare.data[i]!=data[i]){
					return true;
				}
			}
			return false;
		}

		static BitSequence Merge(ArrayList<BitSequence>& source){	//tested
			unsigned long long int totalLength = 0ULL;
			for(unsigned long long int i = 0ULL; i < source.Length(); ++i){
				totalLength += source[i].len;
				assert((totalLength+source[i].len >= totalLength || totalLength+source[i].len >= source[i].len) 
					&& "The merged result of these sequences exceeds the maximum allowed size.");
			}

			BitSequence result(totalLength);

			unsigned long long int currentPosition = 0ULL;

			for(unsigned long long int i = 0ULL; i < source.Length(); ++i){
				result.Write(currentPosition, source[i]);
				currentPosition += source[i].len;
			}
			return result;
		}

		ArrayList<BitSequence> Split(ArrayList<unsigned long long int> indicies){	//Input should be a sorted list.  Splits before index.  ASSUME ORDERED INDICIES.	TESTED!!!			
			ArrayList<BitSequence> returnVal(indicies.Length()+1);
			unsigned long long int currentPos = 0ULL;
			unsigned long long int segLength;
			for(unsigned long long int i = 0ULL; i < indicies.Length(); ++i){
				segLength = indicies[i] - currentPos;
				returnVal[i] = Read(currentPos, segLength);
				currentPos += segLength;
			}

			//Last one.
			returnVal[indicies.Length()] = Read(currentPos, len - currentPos);

			return returnVal;
		}

		ArrayList<unsigned long long int> Search(BitSequence searchTerm, BitSequence replacement){			//Untested
			SinglyLinkedList<unsigned long long int> matches;
			for(unsigned long long int i = 0ULL; i < len - replacement.len; ++i){
				for(unsigned long long int j = 0; j < replacement.len; ++j){
					if(Read(i)!=replacement.Read(j))
						goto BreakToOuter;
				}
				matches.Prepend(i);
				BreakToOuter:;	//Label needs a semicolon apparently?
			}
			return (SinglyLinkedList<unsigned long long int>) matches;
		}

		void Replace(ArrayList<unsigned long long int> indicies, unsigned long long int cutLength, BitSequence replacement){	//We assume no terms overlap.  (Tested in CIDER)
			ArrayList<unsigned long long int> splitPoints(indicies.Length() * 2);
			
			for(unsigned long long int i = 0ULL; i < indicies.Length(); ++i){
				splitPoints[i*2] = indicies[i];
				splitPoints[i*2+1] = indicies[i] + cutLength;
			}
			ArrayList<BitSequence> terms = Split(splitPoints);

			for(unsigned long long int i = 1ULL; i < terms.Length(); i+=2)
				terms[i] = replacement;
			
			(*this) = Merge(terms);
		}

		void Remove(ArrayList<unsigned long long int> indicies, unsigned long long int segLength){
			ArrayList<unsigned long long int> splitPoints(indicies.Length() * 2);
			
			for(unsigned long long int i = 0ULL; i < indicies.Length(); ++i){
				splitPoints[i*2] = indicies[i];
				splitPoints[i*2+1] = indicies[i] + segLength;
			}
			ArrayList<BitSequence> terms = Split(splitPoints);
			ArrayList<BitSequence> newTerms(splitPoints.Length() / 2 + 1);	// +1 is because you always have at least one term.
			for(unsigned long long int i = 0ULL; i < newTerms.Length(); ++i)
				newTerms[i] = terms[i*2];
			
			(*this) = Merge(newTerms);
		}

		/*The index provided is where the object data begins.*/
		template<class T>
		T ToObject(unsigned long long int index = 0ULL){
			unsigned long long int objSize = (sizeof(T)*bytesize > len) ? len : sizeof(T)*bytesize; 
			return *(reinterpret_cast<T*>(Read(index, objSize).data));
		}

		/*This thing doesn't check to make sure your object size is divisible, so be careful.  It'll leave the last value probably.*/
		template<class T>
		T * ToArray(){
			unsigned long long int arrayLength = len / sizeof(T) / bytesize;
			T * returnVal = new T[arrayLength];
			T * rawData = reinterpret_cast<T*>(data);
			for(unsigned long long int i = 0ULL; i < arrayLength; ++i)
				returnVal[i] = rawData[i];
			return returnVal;
		}

		char * ToCharArray(){		//Don't forget that the return needs to be garbage collected.
			char * result = new char[len+1];
			for(unsigned long long int i = 0ULL; i < len; ++i)
					result[i] = Read(i) ? '1' : '0';
			result[len]=0;
			return result;
		}

		bool BitwiseIsMinimized()
		{
			if(len)
				return Read(len-1);
			return true;
		}

		/*Reduces the BitSequence to the smallest size possible while preserving its numerical data. The last bit is always 1 unless Length() = 0*/
		void BitwiseMinimize()		//Tested
		{
			if(!len)	return;

			unsigned long long int * i = adr(len-1);
			while(i>data)
			{
				if(*i)
					break;
				i--;
			}

			unsigned char j = 63;
			for(j; j > 0; --j)
			{
				if(Integer::Read(*i, j))	break;
			}

			Length((i-data)*lintsize+j+1);
		}

		/*Compares the two sequences and returns true if a > b*/
		static bool BitwiseGreaterThan(BitSequence a, BitSequence b)	//Untested
		{
			if(!a.BitwiseIsMinimized())
				a.BitwiseMinimize();
			if(!b.BitwiseIsMinimized())
				b.BitwiseMinimize();

			if(a.len > b.len)
				return true;
			else if(a.len < b.len || a.len == 0)
				return false;
			
			//At this point, a and b are nonzero and have the same length.
			unsigned long long int * adata = a.adr(a.len-1);
			unsigned long long int * bdata = b.adr(b.len-1);

			while(adata > a.data)
			{
				if(*adata > *bdata)
					return true;
				else if(*adata < *bdata)
					return false;
				adata--;
				bdata--;
			}
			return false;
		}

		/*Compares the two sequences and returns true if a > b*/
		static bool BitwiseGreaterThanOrEqual(BitSequence a, BitSequence b)	//Untested
		{
			if(!a.BitwiseIsMinimized())
				a.BitwiseMinimize();
			if(!b.BitwiseIsMinimized())
				b.BitwiseMinimize();

			if(a.len > b.len)
				return true;
			else if(a.len < b.len || a.len == 0)
				return false;
			
			//At this point, a and b are nonzero and have the same length.
			unsigned long long int * adata = a.adr(a.len-1);
			unsigned long long int * bdata = b.adr(b.len-1);

			while(adata > a.data)
			{
				if(*adata > *bdata)
					return true;
				else if(*adata < *bdata)
					return false;
				adata--;
				bdata--;
			}
			return true;
		}

		/*Compares the two sequences and returns true if a < b*/
		static bool BitwiseLessThan(BitSequence a, BitSequence b)	//Untested
		{
			if(!a.BitwiseIsMinimized())
				a.BitwiseMinimize();
			if(!b.BitwiseIsMinimized())
				b.BitwiseMinimize();

			if(a.len < b.len)
				return true;
			else if(a.len > b.len || a.len == 0)
				return false;
			
			//At this point, a and b are nonzero and have the same length.
			unsigned long long int * adata = a.adr(a.len-1);
			unsigned long long int * bdata = b.adr(b.len-1);

			while(adata > a.data)
			{
				if(*adata < *bdata)
					return true;
				else if(*adata > *bdata)
					return false;
				adata--;
				bdata--;
			}
			return false;
		}

		/*Compares the two sequences and returns true if a <= b*/
		static bool BitwiseLessThanOrEqual(BitSequence a, BitSequence b)	//Untested
		{
			if(!a.BitwiseIsMinimized())
				a.BitwiseMinimize();
			if(!b.BitwiseIsMinimized())
				b.BitwiseMinimize();

			if(a.len < b.len)
				return true;
			else if(a.len > b.len || a.len == 0)
				return false;
			
			//At this point, a and b are nonzero and have the same length.
			unsigned long long int * adata = a.adr(a.len-1);
			unsigned long long int * bdata = b.adr(b.len-1);

			while(adata > a.data)
			{
				if(*adata < *bdata)
					return true;
				else if(*adata > *bdata)
					return false;
				adata--;
				bdata--;
			}
			return true;
		}

		static BitSequence BitwiseXOR(BitSequence a, BitSequence b){//UNTESTED
			BitSequence& greater = a.len>b.len?a:b;
			BitSequence& lesser = a.len>b.len?b:a;
			BitSequence retval(greater.len);

			for(unsigned long long int i = 0ULL; i < lesser.lengthLints(); ++i)
				retval.data[i] = greater.data[i]^lesser.data[i];

			for(unsigned long long int i = lesser.lengthLints(); i < greater.lengthLints(); ++i)
				retval.data[i] = greater.data[i];

			return retval;
		}

		/*Adds the two BitSequences and returns the shortest sequence that can accurately represent the integer.*/
		static BitSequence BitwiseAdd(BitSequence addend, BitSequence augend)							//tested
		{
			if(!augend.BitwiseIsMinimized())
				augend.BitwiseMinimize();
			if(!addend.BitwiseIsMinimized())
				addend.BitwiseMinimize();

			BitSequence& largerAddend = augend.len > addend.len ? augend : addend;
			BitSequence& smallerAddend = augend.len > addend.len ? addend : augend;

			BitSequence sum(largerAddend.len+1);
			bool overflow = 0;
			
			unsigned long long int i = 0;
			while(i < smallerAddend.lengthLints())
			{
				sum.data[i] = smallerAddend.data[i] + largerAddend.data[i] + overflow;
				overflow = largerAddend.data[i] > sum.data[i];	//Either the larger or smaller addend may be used.  Either will be greater in the case of an overflow.
				++i;
			}

			while(i < largerAddend.lengthLints())
			{
				sum.data[i] = largerAddend.data[i] + overflow;
				overflow = sum.data[i] == 0;
				++i;
			}

			if(i < sum.lengthLints())		//If the largerAddend is evenly divisible by 64, the last long integer won't be cleared, and this may result in an extra 1 being added as the last bit.
			{
				sum.data[i] = overflow;
			}

			sum.BitwiseMinimize();
			return sum;
		}

		/*Subtracts one sequence from another.  In the event of an overflow the result will take the length of the subtrahend*/
		static BitSequence BitwiseSubtract(BitSequence minuend, BitSequence subtrahend){			//untested
			unsigned long long int smallerLength;
			unsigned long long int largerLength;
			BitSequence difference;

			if(minuend.len > subtrahend.len){
				smallerLength = subtrahend.lengthLints();
				largerLength = minuend.lengthLints();
				difference.Length(minuend.len);
			}else{
				largerLength = subtrahend.lengthLints();
				smallerLength = minuend.lengthLints();
				difference.Length(subtrahend.len);
			}

			bool underflow = 0;
			for(unsigned long long int i = 0ULL; i < smallerLength; ++i)
			{
				difference.data[i] = minuend.data[i] - subtrahend.data[i] - underflow;
				underflow = difference.data[i] > minuend.data[i];
			}

			if(minuend.len > subtrahend.len){
				for(unsigned long long int i = smallerLength; i < largerLength; ++i)
				{
					difference.data[i] = minuend.data[i] - underflow;
					underflow = difference.data[i] > minuend.data[i];
				}
			}else if(subtrahend.len > minuend.len){
				for(unsigned long long int i = smallerLength; i < largerLength; ++i)
				{
					difference.data[i] = -subtrahend.data[i] - underflow;
					underflow = difference.data[i];		//There isn't necessarily an immediate underflow.  The following lints may be equal to 0.
				}
			}

			if(difference.len)
				difference.clearOutOfScopeBits();
			if(!difference.BitwiseIsMinimized())
				difference.BitwiseMinimize();

			return difference;
		}

		static BitSequence BitwiseMultiply(BitSequence multiplicand, BitSequence multiplier)	//TESTED
		{	//Note that int is used here, and is assumed to be 1/2 the size of lint.  If lintsize=intsize, short will most likely need to be used.

			if(!multiplicand.BitwiseIsMinimized())
				multiplicand.BitwiseMinimize();
			if(!multiplier.BitwiseIsMinimized())
				multiplier.BitwiseMinimize();

			BitSequence sum(lengthLints(multiplicand.Length() + multiplier.Length())*lintsize);
			sum.Clear();

			for(unsigned long long int i = 0ULL; i < multiplicand.lengthInts(); ++i)
			{
				for(unsigned long long int j = 0ULL; j < multiplier.lengthInts(); ++j)
				{
					unsigned long long int a = (unsigned long long int)((unsigned int*)multiplicand.data)[i];
					unsigned long long int b = (unsigned long long int)((unsigned int*)multiplier.data)[j];
					unsigned long long int * c = (unsigned long long int*)(((unsigned int*)sum.data) + j + i);
					*c += a * b;

					if (*c < a * b){	//if overflow
						(*++c)++;
						while (*c == 0)	//for as long as this thing overflows.
							(*c++)++;
					}
				}
			}
			sum.BitwiseMinimize();
			return sum;
		}

		static BitSequence BitwiseDivide(BitSequence dividend, BitSequence divisor)	//FINISHED
		{
			if(dividend.BitwiseIsMinimized())
				dividend.BitwiseMinimize();
			if(divisor.BitwiseIsMinimized())
				divisor.BitwiseMinimize();

			if(BitwiseLessThan(dividend.len, divisor.len))
				return Create(0, 1);

			unsigned long long int i = dividend.len-divisor.len+1;
			divisor.Length(dividend.len);
			divisor.ShiftUp(i-1);

			BitSequence quotient(i);
			quotient.Clear();

			while(i--)
			{
				if(BitwiseGreaterThanOrEqual(dividend, divisor))
				{
					quotient.Set(i);
					dividend = BitwiseSubtract(dividend, divisor);
					dividend.BitwiseMinimize();
				}
				divisor.ShiftDown(1);
				divisor.Length(divisor.Length() - 1);
			}

			return quotient;
		}

		static BitSequence BitwiseModulus(BitSequence dividend, BitSequence divisor)	//untested
		{
			if(dividend.BitwiseIsMinimized())
				dividend.BitwiseMinimize();
			if(divisor.BitwiseIsMinimized())
				divisor.BitwiseMinimize();

			if(BitwiseLessThan(dividend.len, divisor.len))
				return dividend;

			unsigned long long int i = dividend.len-divisor.len+1;
			divisor.Length(dividend.len);
			divisor.ShiftUp(i-1);
			
			while(i--)
			{
				if(BitwiseGreaterThanOrEqual(dividend, divisor))
				{
					dividend = BitwiseSubtract(dividend, divisor);
					dividend.BitwiseMinimize();
				}
				divisor.ShiftDown(1);
				divisor.BitwiseMinimize();
			}

			return dividend;
		}

		/*Creates and returns a BitSequence based upon the data provided.  Takes the place of a template constructor.*/
		template<class T>								//untested, but copied from the other ones.
		static BitSequence Create(T sourceObj, unsigned long long int segLength){
			return BitSequence(&sourceObj, segLength);
		}

		private:

			static unsigned long long int lengthLints(unsigned long long int lengthBits){
				return (lengthBits/lintsize)+(lengthBits%lintsize?1:0);
			}
			unsigned long long int lengthLints(){
				return (len/lintsize)+((len%lintsize)?1:0);
			}
			static unsigned long long lengthInts(unsigned long long int lengthBits)
			{
				return (lengthBits/intsize)+(lengthBits%intsize?1:0);
			}
			unsigned long long lengthInts()
			{
				return (len/intsize)+(len%intsize?1:0);
			}
			unsigned long long int lengthBytes(unsigned long long int lengthBits){
				return lengthBits/bytesize+(lengthBits%bytesize?1:0);
			}
			unsigned long long int lengthBytes(){
				return len/bytesize+(len%bytesize?1:0);
			}
			/*Returns a pointer to the actual data of the integer that the index refers to.*/
			unsigned long long int * adr(unsigned long long int index){
				return data+index/lintsize;
			}
			/*Returns the bit-level position of the index, after the 64 bit plane (aka mod64).*/
			static unsigned char pos(unsigned long long int index){
				return index%lintsize;
			}
			/*Clears the bits remaining in the final lint, data that is mallocated, but beyond the length of the sequence.
			NOTE:  Be certain that the length of the sequence is not zero, or this will bomb.*/
			void clearOutOfScopeBits()
			{
				data[lengthLints() - 1] = Integer::ClearNumTop(data[lengthLints() - 1], (lintsize-len%lintsize)%lintsize);
			}
	};
	#undef lintlog
	#undef lintsize
}
#endif