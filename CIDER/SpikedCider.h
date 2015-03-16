/*Spiked Cider stores key locations as a series of pointers to determine the appropriate locations at which to insert a key.*/

#ifndef CIDER_H
#define	CIDER_H

#include "BitSequence.h"
#include "Integer.h"
#include <iostream>

namespace Juice{
	
	class SpikedCider{
		
	public:
		static void MacroPress(BitSequence& source, unsigned char divSize){			
			/*Not all of the data can be compressed at once (memory & speed is exponential with length), so we break large files down into smaller chunks that can be more easily compressed.*/

			/*Macro limit is the size of each of these chunks.  It can be expressed in powers of two only, because there is no reason to be really precise.  Precision just takes up space.*/
			unsigned long long macroLimit = 1 << divSize;		

			/*Create a list to hold the chunks that the source data will be broken into.*/
			ArrayList<BitSequence> chunks(source.Length() / macroLimit + (source.Length()%macroLimit?1:0));

			/*Read off each chunk into its respective list index.*/
			for(unsigned long long int i = 0ULL; (i+1)*macroLimit < source.Length(); ++i)
				chunks[i] = source.Read(i*macroLimit, macroLimit);
			
			/*The last chunk may have an unusual size, because the source is not necessarily evenly divisible by the standard chunk size.*/
			if(source.Length()%macroLimit)	//A special chunk will only be created if the source is not evenly divisible (there is a remainder).
				chunks[chunks.Length()-1] = source.Read((chunks.Length()-1)*macroLimit, source.Length()%macroLimit);
			

			/*Each chunk is then individually compressed.  The micropress function may be used recursively, and it may take many passes to fully-compress data.
			The function will return the data savings of each microPress until it stops compressing data.  It will then return a negative value, to indicate that
			further compression attempts will only increase the file size.*/
			for(unsigned long long int i = 0ULL; i < chunks.Length(); ++i)
				while(microPress(chunks[i], macroLimit)>0);	
			
			/*The assembly list is created to store each compressed chunk, as well as three additional pieces of meta-data about the file.*/
			ArrayList<BitSequence> assembly(chunks.Length()*2+3);	//The first two are reserved, and there's a 1-bit one that comes at the end.

			/*The first piece of meta data is a five-bit number that expresses the chunk size, in powers of two.  You can have a max chunk-size of up to 2^31 bits.*/
			unsigned char limitLength = divSize;
			assembly[0] = BitSequence::Create(divSize, 5);

			/*The second piece of meta data lets us know the size of the last chunk, since it is not necessarily the same size as the other chunks.*/
			assembly[1] = BitSequence::Create(source.Length()%macroLimit, limitLength);

			/*Record the compressed length of each chunk, and store it before the chunk itself.*/
			for(unsigned long long int i = 0ULL; i < chunks.Length(); ++i)
				assembly[i+2] = BitSequence::Create(chunks[i].Length()-1, limitLength);	//the -1 is there because the numbers are encoded one lower (because the actual size can never be zero)

			/*Each compressed chunk is then recorded.*/
			for(unsigned long long int i = 0ULL; i < chunks.Length(); ++i)
				assembly[i+2+chunks.Length()] = chunks[i];
			
			/*We add a one at the end, to indicate the end-of-file.*/
			(assembly[assembly.Length()-1] = BitSequence(1)).Set(0);	//Puts a 1 at the end.
			
			/*Finally, merge the assembly into a single sequence!*/
			source = BitSequence::Merge(assembly);
		}

		static void MacroDepress(BitSequence& source){
			
			/*Because we work in bits, but the file is stored in bytes, we have to end the file with a '1', and clear out any bits that come after the 1 in the last stored byte.*/
			for(unsigned char i = 1ULL; i < 8; ++i){	//Gets the correct source length;
				if(source.Read(source.Length() - i)){
					source.Length(source.Length()-i);
					break;
				}
			}

			/*The limit length tells us the number of bits required to express the macroLimit value in standard binary.*/
			unsigned char limitLength = source.Read(0, 5).ToObject<unsigned long long int>();

			/*The macro limit is a 5-bit number that stores the the standard size of a decompressed data-chunk.  It is recorded in powers of two only, to conserve space.*/
			unsigned long long int macroLimit = (1 << limitLength);
			
			/*The decompressed length of the last chunk can be different from the rest, so we store it separately as a standard binary value.*/
			unsigned long long int lengthOfLast = source.Read(5, limitLength).ToObject<unsigned long long int>();

			/*As we read into the file, curPos stores our location.  It will be used to iterate through the compressed length of each chunk.*/
			unsigned long long int curPos = 5 + limitLength;

			/*As we read through the length of each chunk, SourceLeft records these lengths and we use it to determine if any chunks are unaccounted for.*/
			unsigned long long int sourceLeft = source.Length() - curPos;

			/*Create a list to store the size of each compressed chunk.*/
			SinglyLinkedList<unsigned long long int> lengths;

			/*Iterate through the chunk length data.  As we read more and more chunk length data, 
			the number of chunks unaccounted for has to get smaller and smaller, until SourceLeft is zero.*/
			while(sourceLeft){
				lengths += source.Read(curPos, limitLength).ToObject<unsigned long long int>()+1;	//the +1 is there because the numbers are encoded one lower (because the actual size can never be zero)
				sourceLeft -= lengths[0] + limitLength;
				curPos+=limitLength;
			}
			lengths.ReverseOrder();	//We reverse the order because of how the SinglyLinkedList appends data.

			/*The parts list will hold the actual compressed chunk data.*/
			ArrayList<BitSequence> parts(lengths.Length());

			/*Now that you know the size of each chunk, you can iterate through the remaining data and separate the chunks.*/
			for(unsigned long long int i = 0ULL; lengths.Length(); ++i){
					parts[i] = source.Read(curPos, (--lengths));
					curPos += parts[i].Length();
			}
			
			/*Decompress the standard chunks until the size of each one is equal to the macroLimit.*/
			for(unsigned long long int i = 0ULL; i < parts.Length()-1; ++i){
				while(parts[i].Length() < macroLimit)
					microDepress(parts[i], macroLimit);
			}

			/*The last, non-standard sized chunk is decompressed separately.*/
			while(parts[parts.Length()-1].Length() < lengthOfLast)
				microDepress(parts[parts.Length()-1], macroLimit);
			
			/*Merge the decompressed chunks back together to get your original file.*/
			source = BitSequence::Merge(parts);
		}

		/*Returns true if the compression was successful (I.E. Profitable)
		macroLimit The maximum size that this micro may reach, as specified in the macro file.  This reduces the size of the length of length of data and all its dependencies.*/
		static bool microPress(BitSequence& data, unsigned long long int macroLimit){	//Data was by reference, but it causes heap corruption somehow.
			ArrayList<unsigned long long int> indicies;

			BitSequence key;
			if(GetBestKey(data, key, indicies, macroLimit) <= 0)
				return false;	//If no profit, return false.

			data.Remove(indicies, key.Length());					//Removes the keys, giving you the raw data.

			indicies.Length(indicies.Length() + 1);		//Remove the indicies at the end.  The +1 is for the final pointer, which points to the end, not to a key.
			indicies[indicies.Length()-1] = data.Length()+key.Length()*(indicies.Length()-1);			//The final pointer always points to the end of the data. (plus keys)

			unsigned char lengthOfLengthOfData = Integer::BinaryLog(data.Length());
			
			ArrayList<unsigned char> pointersLength(indicies.Length());
			ArrayList<unsigned long long int> pointers(indicies.Length());

			unsigned long long int totalPointerLength = pointersLength[0] = lengthOfLengthOfData;	//Yup.
			pointers[0] = indicies[0];
			unsigned long long int currentPointerIndex = data.Length() - pointers[0];

			for(unsigned long long int i = 1ULL; i < pointers.Length(); ++i){
				pointersLength[i] = Integer::BinaryLog(currentPointerIndex);
				totalPointerLength += pointersLength[i];
				pointers[i] = (indicies[i] - indicies[i-1]) - key.Length();	//Key.Length is there because (i*key.Length - (i-1)*key.Length)
				currentPointerIndex -= pointers[i];
			}

			ArrayList<BitSequence> parts(5);
			//Length of Length of Data
			unsigned long long int lengthOfLengthOfLengthOfData = Integer::BinaryLog(Integer::BinaryLog(macroLimit-1));
			lengthOfLengthOfLengthOfData = Integer::BinaryLog(Integer::BinaryLog(macroLimit-1-lengthOfLengthOfData));

			parts[0] = BitSequence::Create<unsigned long long int>(lengthOfLengthOfData, lengthOfLengthOfLengthOfData);		//The length of length of data will have a length depending on its maximum allowed values.  (less than 32 is 5 bits, 16 is 4, etc..)
			//Length of Data
			parts[1] = BitSequence::Create<unsigned long long int>(data.Length(), lengthOfLengthOfData);
			//Data
			parts[2] = data;
			//Pointers
			parts[3] = BitSequence(totalPointerLength);
			unsigned long long int currentPos = 0ULL;
			for(unsigned long long int i = 0ULL; i < pointers.Length(); ++i){
				//cout << "cp   " << currentPos << "  tp  " << totalPointerLength << "  lp  " << (unsigned long long int)pointersLength[indicies.Length()] << endl;
				parts[3].Write(currentPos, BitSequence::Create<unsigned long long int>(pointers[i], pointersLength[i]));
				currentPos += pointersLength[i];
			}
			//Key
			parts[4] = key;

			//cout << "Data Merge:  " << endl << data.ToCharArray() << endl << endl;
			data = BitSequence::Merge(parts);
			
			//Debug
			//cout << "Part[0]:  " << parts[0].ToCharArray() << endl;
			//cout << "Part[1]:  " << parts[1].ToCharArray() << endl;
			//cout << "Part[2]:  " << parts[2].ToCharArray() << endl;
			//cout << "Part[3]:  " << parts[3].ToCharArray() << endl;
			//cout << "Part[4]:  " << parts[4].ToCharArray() << endl;
			//cout << endl;
			/**/


			/*//Profit checking
			cout << "Part[0].Length: " << parts[0].Length() << endl;
			cout << "Part[1].Length: " << parts[1].Length() << endl;
			cout << "Part[2].Length: " << parts[2].Length() << endl;
			cout << "Part[3].Length: " << parts[3].Length() << endl;
			cout << "Part[4].Length: " << parts[4].Length() << endl;
			/**/

			return true;
		}

		static void microDepress(BitSequence& source, unsigned long long int macroLimit){
			//Start at zero part.
			unsigned long long int binaryPosition = 0ULL;

			unsigned char lengthOfLengthOfLengthOfData = Integer::BinaryLog(Integer::BinaryLog(macroLimit-1));	
			lengthOfLengthOfLengthOfData = Integer::BinaryLog(Integer::BinaryLog(macroLimit-1-lengthOfLengthOfLengthOfData));
			
			unsigned char lengthOfLengthOfData = source.Read(0, lengthOfLengthOfLengthOfData).ToObject<unsigned char>();
			
			binaryPosition = lengthOfLengthOfLengthOfData;
			
			unsigned long long int lengthOfData = source.Read(binaryPosition, lengthOfLengthOfData).ToObject<unsigned long long int>();
			
			binaryPosition += lengthOfLengthOfData;
			
			BitSequence data = source.Read(binaryPosition, lengthOfData);
			
			binaryPosition += lengthOfData; 
			
			unsigned long long int pointerPosition = 0ULL;
			unsigned long long int indexPointed = 0ULL;
			SinglyLinkedList<unsigned long long int> pointers;	//Should be some kind of vector, but whatever.
			unsigned long long int currentPointerLength;		//Opvar
			while(indexPointed < lengthOfData){
				currentPointerLength = Integer::BinaryLog(lengthOfData - indexPointed);
				pointers += source.Read(binaryPosition + pointerPosition, currentPointerLength).ToObject<unsigned long long int>();
				indexPointed += pointers[0];
				pointerPosition += currentPointerLength;
			}
			pointers.ReverseOrder();
			ArrayList<unsigned long long int> pointerArray(pointers.Length()-1);	//It's one less because the last pointer points to the end, not to a key.
			for(unsigned long long int i = 0ULL; i < pointerArray.Length(); ++i)	//Convert the SinglyLinkedList into an ArrayList.
				pointerArray[i] = --pointers;

			unsigned long long int absoluteOffset = 0ULL;
			for(unsigned long long int i = 0ULL; i < pointerArray.Length(); ++i){
				pointerArray[i] += absoluteOffset;		//Don't add the key length in yet.  That will automatically be handled when the split occurs.
				absoluteOffset += pointerArray[i] - absoluteOffset;
			}//Now your pointers are all ready to go!

			binaryPosition += pointerPosition;
			BitSequence key = source.Read(binaryPosition, source.Length() - binaryPosition);
			//Fin.  We're done unpacking.  Now to start the real work.
			ArrayList<BitSequence> segments = data.Split(pointerArray);
			ArrayList<BitSequence> assembly(segments.Length() * 2 - 1);	//When you split, there's always one more segment on the outside than can fit inbetween any of the pieces.
			
			unsigned long long int lastIndex = segments.Length() - 1;	//Opvar
			for(unsigned long long int i = 0ULL; i < lastIndex; ++i){
				assembly[i*2] = segments[i];
				assembly[i*2+1] = key;
			}
			assembly[lastIndex*2] = segments[lastIndex];	//The last one has to come separately because there's no key after it (not until the next part);
			
			for(unsigned long long int i = lastIndex*2+1; i < assembly.Length(); ++i)
				assembly[i] = key;

			source = BitSequence::Merge(assembly);
		}

		private:

		class Branch{
			public:
			ArrayList<unsigned long long int> index;				//Doesn't have to be singly.  Can be any MutableList.
			unsigned long long int keyDepth;
		};

		static long long int getProfit(BitSequence& source, ArrayList<unsigned long long int>& index, unsigned long long int keyLength, unsigned long long int macroLimit = ULLONG_MAX){	//Returns the profit for a particular term.
			unsigned long long int dataLength = source.Length() - keyLength * index.Length();
			
			unsigned long long int lengthOfLengthOfLengthOfData = Integer::BinaryLog(Integer::BinaryLog(macroLimit-1));
			lengthOfLengthOfLengthOfData = Integer::BinaryLog(Integer::BinaryLog(macroLimit-1-lengthOfLengthOfLengthOfData));
			
			long long int profit = 
				keyLength * index.Length()			//Total Area removed (in keys)
				- Integer::BinaryLog(dataLength)*2			//First Pointer + length of data (are the same)
				- keyLength							//Key
				- lengthOfLengthOfLengthOfData;	//Size of size of Data
			
			for(unsigned long long int i = 0ULL; i < index.Length(); ++i)	//Size of pointers.
				profit -= Integer::BinaryLog(dataLength - (index[i] - i * keyLength));
			
			return profit;
		}
		
		/*Source is the raw data.  Indicies is actually a return value.*/
		static long long int GetBestKey(BitSequence& source, BitSequence& outputKey, ArrayList<unsigned long long int>& outputIndicies, unsigned long long int macroLimit = ULLONG_MAX){
			if(!source.Length())
				return LLONG_MIN;

			auto processBranch = [&](Branch& current, Branch*branch, bool bit) -> bool{
				unsigned long long int i = 0ULL;
				ArrayList<unsigned long long int>& index = branch->index;
				index.Length(current.index.Length());

				unsigned long long int lastIndex = current.index.Length()-1;		//opvar
				if(current.index[lastIndex]+current.keyDepth < source.Length() - 1)	//The -1 is because a term musn't occur on the last index.  Incidentally, split also can't handle it.
					lastIndex++;
					
				for(unsigned long long int j = 0ULL; j < lastIndex; ++j){
					if(source.Read(current.index[j]+current.keyDepth)==bit){
						index[i] = current.index[j];
						do{
							++j;
						}while(j < lastIndex && index[i]+current.keyDepth >= current.index[j]);
						--j;	//There is a j++ in the for loop, we need to roll that back.
						i++;
					}
				}

				if(i>1){
					index.Length(i);	//Shrink only after you know you might be keeping.
					branch->keyDepth = current.keyDepth+1;
					return true;
				}
				return false;
			};

			Branch * bestBranch = new Branch();
			long long int greatestProfit = LLONG_MIN;
			SinglyLinkedList<Branch*> unprocessed;

			{	//Scope.
				Branch * root = new Branch();
				ArrayList<unsigned long long int>& rootIndex = root->index;
				rootIndex.Length(source.Length());
				for(unsigned long long int i = 0ULL; i < source.Length(); ++i)
					rootIndex[i] = i;
				root->keyDepth = 0;
				
				unprocessed += root;
			}

			while(unprocessed.Length()){
				Branch& current = *(--unprocessed);

				Branch * branch0 = new Branch();					//Create two new buds.
				if(processBranch(current, branch0, 0))
					unprocessed += branch0;
				else
					delete branch0;

				Branch * branch1 = new Branch();		
				if(processBranch(current, branch1, 1))
					unprocessed += branch1;
				else
					delete branch1;

				long long int profit = getProfit(source, current.index, current.keyDepth, macroLimit);
				
				if(profit > greatestProfit){
					greatestProfit = profit;
					delete bestBranch;
					bestBranch = &current;
				}else	//If not more profitable than the most profitable branch, delete this one.
					delete &current;
			}

			outputIndicies = bestBranch->index;
			outputKey = source.Read(bestBranch->index[0], bestBranch->keyDepth);
			delete bestBranch;
			return greatestProfit;
		}

	};
	
}
#endif