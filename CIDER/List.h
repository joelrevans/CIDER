#ifndef LIST_H
#define LIST_H

namespace Juice{

	template<class t>
	class ArrayList;	//Forward declaration

	/*CHARACTERISTICS OF LISTS:
	*Lists contain their own copy of each object, just like regular arrays do.  They don't "point" to anything.
	*They have a Length() attribute, representing the number of members.
	*Their Length may or may not be mutable.  The base list does not have mutable Length.
	*Members are ordered and may be accessed by index.*/
	template <class T>
	class List{
	public:

		virtual unsigned long long int Length() = 0;
		
		virtual T& operator [] (unsigned long long int) = 0;	//Access single shorthand

		virtual bool Contains(T) = 0;							//Contains
		virtual bool Contains(List<T>&) = 0;					//Contains any

		virtual unsigned long long int SearchSingle(T) = 0;			//Search for a single result
		virtual ArrayList<unsigned long long int> Search(T) = 0;	//Search for all

		virtual bool operator==(List<T>& compare) = 0;			//Equality operator

		virtual T * ToArray() = 0;								//Converts to native array (needs to be garbagecollected!)
	};

}

#endif