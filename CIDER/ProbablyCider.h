#ifndef PROBABLYCIDER_H
#define PROBABLYCIDER_H

#include "ArrayList.h"
#include "BitSequence.h"

namespace Juice{
	class ProbablyCider{
	public:
		/*The context is the data that comes prior to an element; past data that may help to predict future data.*/
		void Press(BitSequence& source, unsigned char elementSize, unsigned short contextSize){
			unsigned long long int numEl = (1<<elementSize<<contextSize)>>1;
			ArrayList<unsigned long long int> elements(numEl);
			
			unsigned long long int currentPos = 0ULL;
			while(currentPos + elementSize < source.Length())
			{
				elements[source.Read(currentPos, elementSize).ToObject<unsigned long long int>()]++;
				currentPos += elementSize;
			}
			BitSequence lastSlice = source.Read(currentPos, source.Length() - currentPos);

			unsigned long long int numSlices = source.Length() / elementSize;

			for(unsigned long long int i = 0; i < elements.Length(); ++i)
			{
				
			}
		}

		void Depress(BitSequence& source, unsigned char elementSize){

		}
	};
}

#endif