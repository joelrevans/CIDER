#define EXCEPTIONS_ENABLED
#ifdef EXCEPTIONS_ENABLED

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>

using namespace std;

namespace Juice{
	class Exception{
	public:

		Exception();

		Exception(char * output){
			cout << output << endl;
		}
	};
}

#endif

#endif