#ifndef VARIABLEINTEGER_H
#define VARIABLEINTEGER_H

#include "BitSequence.h"

namespace Juice
{
	class VariableInteger
	{
	private:
		BitSequence data;

	public:

		VariableInteger(unsigned long long int size)
		{
			Size(size);
			data.Clear();
		}


		virtual VariableInteger operator+(VariableInteger first, VariableInteger second)
		{

		}


		/*Returns the size of the integer, in bits.*/
		void Size(unsigned long long int newSize)
		{
			unsigned long long int oldSize = data.Length();
			data.Length(newSize);
			if(newSize > oldSize)
				data.Clear(oldSize, newSize-oldSize);
		}

		unsigned long long int Size()
		{
			return data.Length();
		}


	};
}

#endif