#ifndef LISTBOUNDINGEXCEPTION_H
#define	LISTBOUNDINGEXCEPTION_H

#include "Exception.h"	//You're supposed to inherit from this

using namespace std;

namespace Juice{

	class ListBoundingException{
	
	public:

		ListBoundingException(unsigned long long int listLength, unsigned long long int index){
			cout << endl;
			cout << "A list item was out of bounds, yo." << endl;
			cout << "ListLength:  " << listLength << endl;
			cout << "Attempted Access Index:  " << index << endl;
			cout << endl;
		}

		static void Validate(unsigned long long int listLength, unsigned long long int testIndex){
			if(testIndex >= listLength)
				throw ListBoundingException(listLength, testIndex);
		}
	};
}

#endif