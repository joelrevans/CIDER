#ifndef	MUTABLELIST_H
#define MUTABLELIST_H

#include "List.h"
#include "ArrayList.h"

namespace Juice{

	/* MUTABLE LISTS:
	*The list may be expanded or shrunk, and items may be inserted into or removed from the middle of the list.*/
	template <class T>
	class MutableList : public List<T>{
	public:

		virtual void Append(T) = 0;																//Append single
		virtual void Append(List<T>&) = 0;														//Append multiple

		virtual void Prepend(T) = 0;															//Prepend single
		virtual void Prepend(List<T>&) = 0;														//Prepend multiple

		virtual MutableList& operator+=(T) = 0;													//Add shorthand (may correspond to prepend or append, doesn't matter)
		virtual MutableList& operator+=(List<T>&) = 0;											//Add shorthand (may correspond to prepend or append, doesn't matter)

		virtual void Insert(T, unsigned long long int) = 0;										//Insert single
		virtual void Insert(List<T>&, unsigned long long int) = 0;								//Insert multiple

		virtual T operator--() = 0;																//Prefix remove foot
		virtual T operator--(int) = 0;															//Postfix remove head

		virtual unsigned long long int Remove(T) = 0;											//Remove single
		virtual unsigned long long int Remove(List<T>&) = 0;									//Remove multiple
		virtual MutableList& operator-=(T) = 0;													//Remove single shorthand
		virtual MutableList& operator-=(List<T>&) = 0;												//Remove multiple shorthand

		virtual T RemoveAt(unsigned long long int) = 0;											//RemoveAt single
		virtual ArrayList<T> RemoveAt(unsigned long long int, unsigned long long int) = 0;		//RemoveAt multiple	(consecutive items)
		virtual ArrayList<T> RemoveAt(List<unsigned long long int>&) = 0;						//RemoveAt multiple (individual indicies)
		/*There can be no shorthand for this, because if the list itself is of type <unsigned long long int>, it's ambigious with respect to Remove.*/

		virtual ArrayList<T> Get(unsigned long long int, unsigned long long int) = 0;
		virtual ArrayList<T> Get(List<unsigned long long int>&) = 0;
		virtual T& Get(unsigned long long int) = 0;
	};

}

#endif