#ifndef CIDER_H
#define	CIDER_H

#include "BitSequence.h"
#include "Log.h"
#include <iostream>

namespace Juice{

	class Cider{
		
	public:
		/*Returns true if the compression was successful (I.E. Profitable)
		macroLimit The maximum size that this micro may reach, as specified in the macro file.  This reduces the size of the length of length of data and all its dependencies.*/
		static bool microPress(BitSequence& data, unsigned long long int macroLimit){	//Data was by reference, but it causes heap corruption somehow.
			ArrayList<unsigned long long int> indicies;

			BitSequence key;
			if(GetBestKey(data, key, indicies, macroLimit) <= 0)
				return false;	//If no profit, return false.

			unsigned long long int numAtEnd = 0ULL;
			unsigned long long int currentPointer = data.Length() - key.Length();
			unsigned long long int currentIndex = indicies.Length() - 1;
			while(currentPointer == indicies[currentIndex]){
				currentPointer -= key.Length();
				currentIndex--;
				numAtEnd++;
			}
			unsigned char lengthOfNumAtEnd = Integer::BinaryLog(numAtEnd);
			data.Remove(indicies, key.Length());					//Removes the keys, giving you the raw data.

			indicies.Length(indicies.Length() - numAtEnd + 1);		//Remove the indicies at the end.  The +1 is for the final pointer, which points to the end, not to a key.
			indicies[indicies.Length()-1] = data.Length()+key.Length()*(indicies.Length()-1);			//The final pointer always points to the end of the data. (plus keys)

			unsigned char lengthOfLengthOfData = Integer::BinaryLog(data.Length());
			unsigned char lengthOfLengthOfKey = Integer::BinaryLog(key.Length());	//This will be encoded as fewer than six bits if log(dataLength - 1) != log(dataLength) because keyMax = data / 2;
			
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

			ArrayList<BitSequence> parts(9);
			//Length of Length of Data
			parts[0] = BitSequence::Create<unsigned long long int>(lengthOfLengthOfData, Integer::BinaryLog(Integer::BinaryLog(macroLimit)));		//The length of length of data will have a length depending on its maximum allowed values.  (less than 32 is 5 bits, 16 is 4, etc..)
			//Length of Data
			parts[1] = BitSequence::Create<unsigned long long int>(data.Length(), lengthOfLengthOfData);
			//Data
			parts[2] = data;
			//LengthOfLengthOfKey
			parts[3] = BitSequence::Create<unsigned long long int>(lengthOfLengthOfKey, Integer::BinaryLog(lengthOfLengthOfData-1));		//The minus one is because the key can never be more than half the data.
			//LengthOfKey
			parts[4] = BitSequence::Create<unsigned long long int>(key.Length(), lengthOfLengthOfKey);
			//Key
			parts[5] = key;
			//Pointers
			parts[6] = BitSequence(totalPointerLength);
			unsigned long long int currentPos = 0ULL;
			for(unsigned long long int i = 0ULL; i < pointers.Length(); ++i){
				//cout << "cp   " << currentPos << "  tp  " << totalPointerLength << "  lp  " << (unsigned long long int)pointersLength[indicies.Length()] << endl;
				parts[6].Write(currentPos, BitSequence::Create<unsigned long long int>(pointers[i], pointersLength[i]));
				currentPos += pointersLength[i];
			}
			//LengthOfNumAtEnd
			/*unsigned long long int howManyCouldPossiblyFitAtTheEnd = 
				(macroLimit 
				- parts[0].Length() 
				- parts[1].Length() 
				- parts[2].Length() 
				- parts[3].Length() 
				- parts[4].Length()
				- parts[5].Length()
				- parts[6].Length()) / key.Length();*/
			//parts[7] = BitSequence::Create<unsigned long long int>(howManyCouldPossiblyFitAtTheEnd, Integer::BinaryLog(howManyCouldPossiblyFitAtTheEnd));
			parts[7] = BitSequence::Create<unsigned long long int>(Integer::BinaryLog(numAtEnd), 6);
			//parts[7] = Integer::BinaryLog(howManyCouldPossiblyFitAtTheEnd - Integer::BinaryLog(howManyCouldPossiblyFitAtTheEnd));
			//NumAtEnd
			parts[8] = BitSequence::Create<unsigned long long int>(numAtEnd, Integer::BinaryLog(numAtEnd));
			data = BitSequence::Merge(parts);
			//cout << "Data Merge:  " << endl << data.ToCharArray() << endl << endl;
			
			/*
			//Debug
			cout << "Part[0]:  " << parts[0].ToCharArray() << endl;
			cout << "Part[1]:  " << parts[1].ToCharArray() << endl;
			//cout << "Part[2]:  " << parts[2].ToCharArray() << endl;
			cout << "Part[3]:  " << parts[3].ToCharArray() << endl;
			cout << "Part[4]:  " << parts[4].ToCharArray() << endl;
			cout << "Part[5]:  " << parts[5].ToCharArray() << endl;
			cout << "Part[6]:  " << parts[6].ToCharArray() << endl;
			cout << "Part[7]:  " << parts[7].ToCharArray() << endl;
			cout << "Part[8]:  " << parts[8].ToCharArray() << endl;
			cout << endl;
			/**/


			//Profit checking
			/*cout << "Part[0].Length: " << parts[0].Length() << endl;
			cout << "Part[1].Length: " << parts[1].Length() << endl;
			cout << "Part[2].Length: " << parts[2].Length() << endl;
			cout << "Part[3].Length: " << parts[3].Length() << endl;
			cout << "Part[4].Length: " << parts[4].Length() << endl;
			cout << "Part[5].Length: " << parts[5].Length() << endl;
			cout << "Part[6].Length: " << parts[6].Length() << endl;
			cout << "Part[7].Length: " << parts[7].Length() << endl;
			cout << "Part[8].Length: " << parts[8].Length() << endl;

			/**/

			return true;
		}

		static void microDepress(BitSequence& source, unsigned long long int macroLimit){
			//Start at zero part.
			unsigned long long int binaryPosition = 0ULL;
			//Used to determine the length of the first part.
			unsigned char lengthOfLengthOfLengthOfData = Integer::BinaryLog(Integer::BinaryLog(macroLimit));
			//LengthOfLengthOfData
			unsigned char lengthOfLengthOfData = source.Read(0, lengthOfLengthOfLengthOfData).ToObject<unsigned char>();
			//DEBUG//	cout << "Part2[0]:  " << source.Read(0, lengthOfLengthOfLengthOfData).ToCharArray() << endl;
			//Move to part 1
			binaryPosition = lengthOfLengthOfLengthOfData;
			//LengthOfData
			unsigned long long int lengthOfData = source.Read(binaryPosition, lengthOfLengthOfData).ToObject<unsigned long long int>();
			//DEBUG//	cout << "Part2[1]:  " << source.Read(binaryPosition, lengthOfLengthOfData).ToCharArray() << endl;
			//Move to part 2
			binaryPosition += lengthOfLengthOfData;
			//Data
			BitSequence data = source.Read(binaryPosition, lengthOfData);
			//cout << "Part2[2]:  " << data.ToCharArray() << endl;
			//Move to part 3
			binaryPosition += lengthOfData; 
			//Determine the length of the length of the length the key.
			unsigned long long int lengthOfLengthOfLengthOfKey = Integer::BinaryLog(lengthOfLengthOfData-1);
			//LengthOfLengthOfKey
			unsigned char lengthOfLengthOfKey = source.Read(binaryPosition, lengthOfLengthOfLengthOfKey).ToObject<unsigned char>();
			//DEBUG//	cout << "Part2[3]:  " << source.Read(binaryPosition, lengthOfLengthOfLengthOfKey).ToCharArray() << endl;
			//Move to part 4
			binaryPosition += lengthOfLengthOfLengthOfKey;
			//LengthOfKey
			unsigned long long int lengthOfKey = source.Read(binaryPosition, lengthOfLengthOfKey).ToObject<unsigned long long int>();
			//DEBUG//	cout << "Part2[4]:  " << source.Read(binaryPosition, lengthOfLengthOfKey).ToCharArray() <<endl;
			//Move to part 5
			binaryPosition += lengthOfLengthOfKey;
			//Key
			BitSequence key = source.Read(binaryPosition, lengthOfKey);
			//DEBUG//	cout << "Part2[5]:  " << key.ToCharArray() <<endl;
			//Move to part 6
			binaryPosition += lengthOfKey;
			//Pointers
			unsigned long long int pointerPosition = 0ULL;
			unsigned long long int indexPointed = 0ULL;
			SinglyLinkedList<unsigned long long int> pointers;	//Should be some kind of vector, but whatever.
			unsigned long long int currentPointerLength;		//Opvar
			//DEBUG//	cout << "Part2[6]:  ";
			while(indexPointed < lengthOfData){
				currentPointerLength = Integer::BinaryLog(lengthOfData - indexPointed);
				pointers += source.Read(binaryPosition + pointerPosition, currentPointerLength).ToObject<unsigned long long int>();
				//DEBUG//	cout << source.Read(binaryPosition + pointerPosition, currentPointerLength).ToCharArray();
				indexPointed += pointers[0];
				pointerPosition += currentPointerLength;
			}
			//DEBUG//	cout << endl;
			pointers.ReverseOrder();
			ArrayList<unsigned long long int> pointerArray(pointers.Length()-1);	//It's one less because the last pointer points to the end, not to a key.
			for(unsigned long long int i = 0ULL; i < pointerArray.Length(); ++i)	//Convert the SinglyLinkedList into an ArrayList.
				pointerArray[i] = --pointers;

			unsigned long long int absoluteOffset = 0ULL;
			for(unsigned long long int i = 0ULL; i < pointerArray.Length(); ++i){
				pointerArray[i] += absoluteOffset;		//Don't add the key length in yet.  That will automatically be handled when the split occurs.
				absoluteOffset += pointerArray[i] - absoluteOffset;
			}//Now your pointers are all ready to go!
			
			//Move to part 7
			binaryPosition += pointerPosition;
			//LengthOfLengthOfNumAtEnd
			unsigned char lengthOfLengthOfNumAtEnd = 6; //Integer::BinaryLog((macroLimit - binaryPosition)/lengthOfKey);
			//LengthOfNumAtEnd
			unsigned char lengthOfNumAtEnd = source.Read(binaryPosition, lengthOfLengthOfNumAtEnd).ToObject<unsigned char>();
			//DEBUG//	cout << "Part2[7]:  " << source.Read(binaryPosition, lengthOfLengthOfNumAtEnd).ToCharArray() <<endl;
			//Move to part 8
			binaryPosition += lengthOfLengthOfNumAtEnd;
			//NumAtEnd
			unsigned long long int numAtEnd = lengthOfNumAtEnd ? source.Read(binaryPosition, lengthOfNumAtEnd).ToObject<unsigned long long int>() : 0;
			
			//DEBUG//	cout << "Part2[8]:  " << source.Read(binaryPosition, lengthOfNumAtEnd).ToCharArray() <<endl;
			//Fin.  We're done unpacking.  Now to start the real work.

			ArrayList<BitSequence> segments = data.Split(pointerArray);
			ArrayList<BitSequence> assembly(segments.Length() * 2 - 1 + numAtEnd);	//When you split, there's always one more segment on the outside than can fit inbetween any of the pieces.
			
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
			Branch * branch0;
			Branch * branch1;
			ArrayList<unsigned long long int> index;				//Doesn't have to be singly.  Can be any MutableList.
			unsigned long long int keyDepth;

			Branch *& operator[](bool select){
				return select ? branch1 : branch0;
			}
		};

		static long long int getProfit(BitSequence& source, ArrayList<unsigned long long int>& index, unsigned long long int keyLength, unsigned long long int macroLimit = ULLONG_MAX){	//Returns the profit for a particular term.
			unsigned long long int dataLength = source.Length() - keyLength * index.Length();
			long long int profit = 
				keyLength * index.Length()			//Total Area removed (in keys)
				- Integer::BinaryLog(dataLength)*2			//First Pointer + length of data (are the same)
				- keyLength							//Key
				- Integer::BinaryLog(keyLength)				//Size of key
				- Integer::BinaryLog(Integer::BinaryLog(dataLength)-1)//Size of size of key
				- Integer::BinaryLog(Integer::BinaryLog(macroLimit))	//Size of size of Data
				- 6;								//Size of size of NumAtEnd
			
			unsigned long long int numAtEnd = 0ULL;
			unsigned long long int currentPointer = source.Length() - keyLength;
			unsigned long long int currentIndex = index.Length() - 1;
			while(currentPointer == index[currentIndex]){
				currentPointer -= keyLength;
				currentIndex--;
				numAtEnd++;
			}	//Gets numAtEnd;
			profit -= Integer::BinaryLog(numAtEnd);	//NumAtEnd
			
			for(unsigned long long int i = 0ULL; i < index.Length() - numAtEnd; ++i)	//Size of pointers.
				profit -= Integer::BinaryLog(dataLength - (index[i] - i * keyLength));
			
			return profit;
		}
		
		/*Source is the raw data.  Indicies is actually a return value.*/
		static long long int GetBestKey(BitSequence& source, BitSequence& outputKey, ArrayList<unsigned long long int>& outputIndicies, unsigned long long int macroLimit = ULLONG_MAX){
			Branch * root = new Branch();
			root->index.Length(source.Length());
			root->keyDepth = 0;
			for(unsigned long long int i = 0ULL; i < source.Length(); ++i)
				root->index[i] = i;
			
			SinglyLinkedList<Branch*> unprocessed;
			unprocessed += root;

			long long int greatestProfit = LLONG_MIN;			//Establishes the most profitable term.
			Branch * bestBranch = new Branch();					//Can't be root because it'll be deleted on the first pass at the end.
			while(unprocessed.Length()){
				Branch& current = *(--unprocessed);

				current.branch0 = new Branch();					//Create two new buds.
				current.branch1 = new Branch();
																//Create the two new key depths for each respective branch.
				current.branch0->keyDepth = current.branch1->keyDepth = current.keyDepth + 1;

																//OPVAR's and indexes of each branch.
				ArrayList<unsigned long long int>& index0 = current.branch0->index;
				ArrayList<unsigned long long int>& index1 = current.branch1->index;
																//Initializes the indexes of each branch, setting it to the maximum possible size.
				index0.Length(current.index.Length());
				index1.Length(current.index.Length());

				unsigned long long int i0 = 0ULL;				//counter for index0 (records the size of the index0 array)
				unsigned long long int i1 = 0ULL;				//counter for index1 (records the size of the index1 array)

				unsigned long long int lastIndex = current.index.Length()-1;		//opvar
				unsigned long long int keyDepth = current.keyDepth+1;
				
				for(unsigned long long int j = 0ULL; j < lastIndex; ++j){			//Splits all of the index locations from the current branch and feeds it into the repspective buds.
					if(source.Read(current.index[j]+keyDepth)){
						if(i1==0 || (index1[i1-1] + keyDepth < current.index[j]))
							index1[i1++] = current.index[j];
					}else{
						if(i0==0 || (index0[i0-1] + keyDepth < current.index[j]))
							index0[i0++] = current.index[j];
					}
					//while(j<lastIndex && current.index[j] + keyDepth > current.index[j+1])			//This clause prevents terms from overlapping each other (oompaloompaloompaloompa).
						//++j;														//Technically I think you save more space by preserving as many end-terms as possible (smaller pointers)
				}
				
				if((current.index[lastIndex]+keyDepth < source.Length()) && //This condition only needs to be run on the last item in the list, to make sure it doesn't extend past the end.	
					(current.index.Length() == 1 || current.index[lastIndex] > current.index[lastIndex-1] + keyDepth)){	//Prevents the last term from overlapping the one that came before it.
					if(source.Read(current.index[lastIndex]+keyDepth))
						index1[i1++] = current.index[lastIndex];
					else
						index0[i0++] = current.index[lastIndex];
				}
															//Shrink the two indicies from the maximum to their actual size.
				
				/*If the term is non-unique, move the branches to be processed.  Otherwise, delete them.*/
				if(i0>1){
					unprocessed += current.branch0;
					index0.Length(i0);	//Shrink only after you know you might be keeping.
				}else
					delete current.branch0;

				if(i1>1){
					index1.Length(i1);	//Shrink only after you know you might be keeping.
					unprocessed += current.branch1;
				}else
					delete current.branch1;

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