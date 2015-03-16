#ifndef CIDER_H
#define	CIDER_H

#include "BitSequence.h"
#include "Log.h"
#include <iostream>

namespace Juice{
	
	class KeySubCider{
		
	public:
		static void MacroPress(BitSequence& source, unsigned char divSize){			
			unsigned long long macroLimit = 1 << divSize;
			//cout << "MacroPrePre:  " << endl << source.Read(0, 1024).ToCharArray() << endl << endl;

			ArrayList<BitSequence> chunks(source.Length() / macroLimit + (source.Length()%macroLimit?1:0));
			for(unsigned long long int i = 0ULL; (i+1)*macroLimit < source.Length(); ++i){
				chunks[i] = source.Read(i*macroLimit, macroLimit);
				//cout << "(" << i << ")" << endl << chunks[i].ToCharArray() << endl;
			}
			//cout << endl << "MacroP:  " << endl << chunks[0].ToCharArray() << endl << endl;
			if(source.Length()%macroLimit)
				chunks[chunks.Length()-1] = source.Read((chunks.Length()-1)*macroLimit, source.Length()%macroLimit);
			
			for(unsigned long long int i = 0ULL; i < chunks.Length(); ++i)
				while(microPress(chunks[i], macroLimit)>0);
			//cout << "MacroPost:  " << endl << chunks[0].ToCharArray() << endl << endl;
			ArrayList<BitSequence> assembly(chunks.Length()*2+3);	//The first two are reserved, and there's a 1-bit one that comes at the end.

			//The macro limit.
			unsigned char limitLength = divSize;
			assembly[0] = BitSequence::Create(divSize, 5);
			assembly[1] = BitSequence::Create(source.Length()%macroLimit, limitLength);

			for(unsigned long long int i = 0ULL; i < chunks.Length(); ++i)
				assembly[i+2] = BitSequence::Create(chunks[i].Length()-1, limitLength);	//the -1 is there because the numbers are encoded one lower (because the actual size can never be zero)

			for(unsigned long long int i = 0ULL; i < chunks.Length(); ++i)
				assembly[i+2+chunks.Length()] = chunks[i];
				//cout << "[" << i << "]" << endl << assembly[2+chunks.Length()+i].ToCharArray() << endl;
			
			(assembly[assembly.Length()-1] = BitSequence(1)).Set(0);	//Puts a 1 at the end.
			//for(unsigned int i = 2; i < 
			source = BitSequence::Merge(assembly);
			//cout << endl << endl << source.ToCharArray() << endl;
		}

		static void MacroDepress(BitSequence& source){
			//cout << endl << endl << source.ToCharArray() << endl;
			
			for(unsigned char i = 1ULL; i < 8; ++i){	//Gets the correct source length;
				if(source.Read(source.Length() - i)){
					source.Length(source.Length()-i);
					break;
				}
			}
			//cout << endl << source.Read(0, 5).ToCharArray() << endl << endl;
			unsigned long long int macroLimit = 1 << source.Read(0, 5).ToObject<unsigned long long int>();

			unsigned char limitLength = Integer::BinaryLog(macroLimit-1);
			unsigned long long int lengthOfLast = source.Read(5, limitLength).ToObject<unsigned long long int>();

			unsigned long long int curPos = 5 + limitLength;
			unsigned long long int sourceLeft = source.Length() - curPos;

			SinglyLinkedList<unsigned long long int> lengths;

			while(sourceLeft){
				lengths += source.Read(curPos, limitLength).ToObject<unsigned long long int>()+1;	//the +1 is there because the numbers are encoded one lower (because the actual size can never be zero)
				sourceLeft -= lengths[0] + limitLength;
				curPos+=limitLength;
			}

			lengths.ReverseOrder();
			ArrayList<BitSequence> parts(lengths.Length());
			{	//introduced some additional scope here to contain i.
				unsigned long long int i = 0ULL;
				while(lengths.Length()){
					parts[i] = source.Read(curPos, (--lengths));
					curPos += parts[i++].Length();
				}
			}
			
			for(unsigned long long int i = 0ULL; i < parts.Length()-1; ++i){
				//cout << "[" << i << "]" << endl << parts[i].ToCharArray() << endl;
				while(parts[i].Length() < macroLimit)
					microDepress(parts[i], macroLimit);
				//cout << "(" << i << ")" << endl << parts[i].ToCharArray() << endl;
			}

			while(parts[parts.Length()-1].Length() < lengthOfLast)
				microDepress(parts[parts.Length()-1], macroLimit);
			
			source = BitSequence::Merge(parts);
		}

		/*Returns true if the compression was successful (I.E. Profitable)
		macroLimit The maximum size that this micro may reach, as specified in the macro file.  This reduces the size of the length of length of data and all its dependencies.*/
		static bool microPress(BitSequence& data, unsigned long long int macroLimit){
			ArrayList<unsigned long long int> indicies;

			BitSequence key;
			if(GetBestKey(data, key, smallKey, indicies, macroLimit) <= 0)
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
		static long long int GetBestKey(BitSequence& source, BitSequence& outputKey, BitSequence& smallKey, ArrayList<unsigned long long int>& outputIndicies, unsigned long long int macroLimit = ULLONG_MAX){
			if(!source.Length())
				return LLONG_MIN;

			auto processBranch = [&](Branch& current, Branch*branch, bool bit) -> bool{	//lambda function
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