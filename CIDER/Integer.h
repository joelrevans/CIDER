#ifndef INTEGER_CPP
#define INTEGER_CPP

namespace Juice{
	
	class Integer{
		public:
		/*Returns the value of a bit from a binary number as either 0 or 1, with the least significant bit corresponding to index=0.*/
		static bool Read(unsigned long long int value, unsigned char index)
		{
			return (value >> index) & 1ULL;
		}
		static bool Read(long long int value, unsigned char index)
		{
			return (value >> index) & 1LL;
		}
		static bool Read(unsigned int value, unsigned char index)
		{
			return (value >> index) & 1;
		}
		static bool Read(int value, unsigned char index)
		{
			return (value >> index) & 1;
		}
		static bool Read(unsigned short value, unsigned char index)
		{
			return (value >> index) & 1;
		}
		static bool Read(short value, unsigned char index)
		{
			return (value >> index) & 1;
		}
		static bool Read(unsigned char value, unsigned char index)
		{
			return (value >> index) & 1;
		}
		static bool Read(char value, unsigned char index)
		{
			return (value >> index) & 1;
		}

		/*Rewrites the value of a bit from a binary number as a 1, with the least significant bit corresponding to index=0.*/
		static unsigned long long int Set(unsigned long long int value, unsigned char index)
		{
			return value | (1ULL << index);
		}
		static long long int Set(long long int value, unsigned char index)
		{
			return value | (1LL << index);
		}
		static unsigned int Set(unsigned int value, unsigned char index)
		{
			return value | (1 << index);
		}
		static int Set(int value, unsigned char index)
		{
			return value | (1 << index);
		}
		static unsigned short Set(unsigned short value, unsigned char index)
		{
			return value | (1 << index);
		}
		static short Set(short value, unsigned char index)
		{
			return value | (1 << index);
		}
		static unsigned char Set(unsigned char value, unsigned char index)
		{
			return value | (1 << index);
		}
		static char Set(char value, unsigned char index)
		{
			return value | (1 << index);
		}

		/*Rewrites the value of a bit from a binary number as a 1, with the least significant bit corresponding to index=0.*/
		static unsigned long long int Clear(unsigned long long int value, unsigned char index)
		{
			return value & ~(1ULL << index);
		}
		static long long int Clear(long long int value, unsigned char index)
		{
			return value & ~(1LL << index);
		}
		static unsigned int Clear(unsigned int value, unsigned char index)
		{
			return value & ~(1 << index);
		}
		static int Clear(int value, unsigned char index)
		{
			return value & ~(1 << index);
		}
		static unsigned short Clear(unsigned short value, unsigned char index)
		{
			return value & ~(1 << index);
		}
		static short Clear(short value, unsigned char index)
		{
			return value & ~(1 << index);
		}
		static unsigned char Clear(unsigned char value, unsigned char index)
		{
			return value & ~(1 << index);
		}
		static char Clear(char value, unsigned char index)
		{
			return value & ~(1 << index);
		}

		/*Clears n number of the most significant bits of the integer.*/
		static unsigned long long int ClearNumTop(unsigned long long int value, unsigned char n)
		{
			return value & (0xFFFFFFFFFFFFFFFFULL>>n);
		}
		static long long int ClearNumTop(long long int value, unsigned char n)
		{
			return value & (0xFFFFFFFFFFFFFFFFLL>>n);
		}
		static unsigned int ClearNumTop(unsigned int value, unsigned char n)
		{
			return value & (0xFFFFFFFFULL>>n);
		}
		static int ClearNumTop(int value, unsigned char n)
		{
			return value & (0xFFFFFFFFULL>>n);
		}
		static unsigned short ClearNumTop(unsigned short value, unsigned char n)
		{
			return value & (0xFFFFULL>>n);
		}
		static short ClearNumTop(short value, unsigned char n)
		{
			return value & (0xFFFFULL>>n);
		}
		static unsigned char ClearNumTop(unsigned char value, unsigned char n)
		{
			return value & (0xFFULL>>n);
		}
		static char ClearNumTop(char value, unsigned char n)
		{
			return value & (0xFFULL>>n);
		}

		/*Clears n number of the least significant bits of the integer.*/
		static unsigned long long int ClearNumBot(unsigned long long int value, unsigned char n)
		{
			return value & (0xFFFFFFFFFFFFFFFFULL<<n);
		}
		static long long int ClearNumBot(long long int value, unsigned char n)
		{
			return value & (0xFFFFFFFFFFFFFFFFLL<<n);
		}
		static unsigned int ClearNumBot(unsigned int value, unsigned char n)
		{
			return value & (0xFFFFFFFFULL<<n);
		}
		static int ClearNumBot(int value, unsigned char n)
		{
			return value & (0xFFFFFFFFULL<<n);
		}
		static unsigned short ClearNumBot(unsigned short value, unsigned char n)
		{
			return value & (0xFFFFULL<<n);
		}
		static short ClearNumBot(short value, unsigned char n)
		{
			return value & (0xFFFFULL<<n);
		}
		static unsigned char ClearNumBot(unsigned char value, unsigned char n)
		{
			return value & (0xFFULL<<n);
		}
		static char ClearNumBot(char value, unsigned char n)
		{
			return value & (0xFFULL<<n);
		}

		/*Clears the bit at the specified index and all bits to the left of it.*/
		static unsigned long long int ClearTopIncl(unsigned long long int value, unsigned char index){
			return value & ((1ULL << index)-1);
		}
		static long long int ClearTopIncl(long long int value, unsigned char index){
			return value & ((1LL << index)-1);
		}
		static unsigned int ClearTopIncl(unsigned int value, unsigned char index){
			return value & ((1 << index)-1);
		}
		static int ClearTopIncl(int value, unsigned char index){
			return value & ((1 << index)-1);
		}
		static unsigned short ClearTopIncl(unsigned short value, unsigned char index){
			return value & ((1 << index)-1);
		}
		static short ClearTopIncl(short value, unsigned char index){
			return value & ((1 << index)-1);
		}
		static unsigned char ClearTopIncl(unsigned char value, unsigned char index){
			return value & ((1 << index)-1);
		}
		static char ClearTopIncl(char value, unsigned char index){
			return value & ((1 << index)-1);
		}

		/*Clears all of the bits to the left of the specified bit index, but does not clear the index bit.*/
		static unsigned long long int ClearTopExcl(unsigned long long int value, unsigned char index){
			return value & ~(0xFFFFFFFFFFFFFFFEULL<<index);
		}
		static long long int ClearTopExcl(long long int value, unsigned char index){
			return value & ~(0xFFFFFFFFFFFFFFFELL<<index);
		}
		static unsigned int ClearTopExcl(unsigned int value, unsigned char index){
			return value & ~(0xFFFFFFFE<<index);
		}
		static int ClearTopExcl(int value, unsigned char index){
			return value & ~(0xFFFFFFFE<<index);
		}
		static unsigned short ClearTopExcl(unsigned short value, unsigned char index){
			return value & ~(0xFFFE<<index);
		}
		static short ClearTopExcl(short value, unsigned char index){
			return value & ~(0xFFFE<<index);
		}
		static unsigned char ClearTopExcl(unsigned char value, unsigned char index){
			return value & ~(0xFE<<index);
		}
		static char ClearTopExcl(char value, unsigned char index){
			return value & ~(0xFE<<index);
		}

		/*Clears the bit at the specified index and all bits to the right of it.*/
		static unsigned long long int ClearBotIncl(unsigned long long int value, unsigned char index){
			return value & (0xFFFFFFFFFFFFFFFEULL<<index);
		}
		static long long int ClearBotIncl(long long int value, unsigned char index){
			return value & (0xFFFFFFFFFFFFFFFELL<<index);
		}
		static unsigned int ClearBotIncl(unsigned int value, unsigned char index){
			return value & (0xFFFFFFFE<<index);
		}
		static int ClearBotIncl(int value, unsigned char index){
			return value & (0xFFFFFFFE<<index);
		}
		static unsigned short ClearBotIncl(unsigned short value, unsigned char index){
			return value & (0xFFFE<<index);
		}
		static short ClearBotIncl(short value, unsigned char index){
			return value & (0xFFFE<<index);
		}
		static unsigned char ClearBotIncl(unsigned char value, unsigned char index){
			return value & (0xFE<<index);
		}
		static char ClearBotIncl(char value, unsigned char index){
			return value & (0xFE<<index);
		}

		/*Clears all of the bits to the right of the specified bit index, but does not clear the index bit.*/
		static unsigned long long int ClearBotExcl(unsigned long long int value, unsigned char index){
			return value & (0xFFFFFFFFFFFFFFFFULL<<index);
		}
		static long long int ClearBotExcl(long long int value, unsigned char index){
			return value & (0xFFFFFFFFFFFFFFFFLL<<index);
		}
		static unsigned int ClearBotExcl(unsigned int value, unsigned char index){
			return value & (0xFFFFFFFF<<index);
		}
		static int ClearBotExcl(int value, unsigned char index){
			return value & (0xFFFFFFFF<<index);
		}
		static unsigned short ClearBotExcl(unsigned short value, unsigned char index){
			return value & (0xFFFF<<index);
		}
		static short ClearBotExcl(short value, unsigned char index){
			return value & (0xFFFF<<index);
		}
		static unsigned char ClearBotExcl(unsigned char value, unsigned char index){
			return value & (0xFF<<index);
		}
		static char ClearBotExcl(char value, unsigned char index){
			return value & (0xFF<<index);
		}

		/*Sets n number of the most significant bits of the integer.*/
		static unsigned long long int SetNumTop(unsigned long long int value, unsigned char n)
		{
			return value | ~(0xFFFFFFFFFFFFFFFFULL>>n);
		}
		static long long int SetNumTop(long long int value, unsigned char n)
		{
			return value | ~(0xFFFFFFFFFFFFFFFFLL>>n);
		}
		static unsigned int SetNumTop(unsigned int value, unsigned char n)
		{
			return value | ~(0xFFFFFFFF>>n);
		}
		static int SetNumTop(int value, unsigned char n)
		{
			return value | ~(0xFFFFFFFF>>n);
		}
		static unsigned short SetNumTop(unsigned short value, unsigned char n)
		{
			return value | ~(0xFFFF>>n);
		}
		static short SetNumTop(short value, unsigned char n)
		{
			return value | ~(0xFFFF>>n);
		}
		static unsigned char SetNumTop(unsigned char value, unsigned char n)
		{
			return value | ~(0xFF>>n);
		}
		static char SetNumTop(char value, unsigned char n)
		{
			return value | ~(0xFF>>n);
		}

		/*Sets n number of the least significant bits of the integer.*/
		static unsigned long long int SetNumBot(unsigned long long int value, unsigned char n)
		{
			return value | ~(0xFFFFFFFFFFFFFFFFULL<<n);
		}
		static long long int SetNumBot(long long int value, unsigned char n)
		{
			return value | ~(0xFFFFFFFFFFFFFFFFLL<<n);
		}
		static unsigned int SetNumBot(unsigned int value, unsigned char n)
		{
			return value | ~(0xFFFFFFFF<<n);
		}
		static int SetNumBot(int value, unsigned char n)
		{
			return value | ~(0xFFFFFFFF<<n);
		}
		static unsigned short SetNumBot(unsigned short value, unsigned char n)
		{
			return value | ~(0xFFFFU<<n);
		}
		static short SetNumBot(short value, unsigned char n)
		{
			return value | ~(0xFFFF<<n);
		}
		static unsigned char SetNumBot(unsigned char value, unsigned char n)
		{
			return value | ~(0xFF<<n);
		}
		static char SetNumBot(char value, unsigned char n)
		{
			return value | ~(0xFF<<n);
		}

		/*Sets the bit at the specified index and all bits to the left of it.*/
		static unsigned long long int SetTopIncl(unsigned long long int value, unsigned char index){
			return value | ~((1ULL << index)-1);
		}
		static long long int SetTopIncl(long long int value, unsigned char index){
			return value | ~((1LL << index)-1);
		}
		static unsigned int SetTopIncl(unsigned int value, unsigned char index){
			return value | ~((1 << index)-1);
		}
		static int SetTopIncl(int value, unsigned char index){
			return value | ~((1 << index)-1);
		}
		static unsigned short SetTopIncl(unsigned short value, unsigned char index){
			return value | ~((1 << index)-1);
		}
		static short SetTopIncl(short value, unsigned char index){
			return value | ~((1 << index)-1);
		}
		static unsigned char SetTopIncl(unsigned char value, unsigned char index){
			return value | ~((1 << index)-1);
		}
		static char SetTopIncl(char value, unsigned char index){
			return value | ~((1 << index)-1);
		}

		/*Sets all of the bits to the left of the specified bit index, but does not set the index bit.*/
		static unsigned long long int SetTopExcl(unsigned long long int value, unsigned char index){
			return value | (0xFFFFFFFFFFFFFFFEULL<<index);
		}
		static long long int SetTopExcl(long long int value, unsigned char index){
			return value | (0xFFFFFFFFFFFFFFFEULL<<index);
		}
		static unsigned int SetTopExcl(unsigned int value, unsigned char index){
			return value | (0xFFFFFFFE<<index);
		}
		static int SetTopExcl(int value, unsigned char index){
			return value | (0xFFFFFFFE<<index);
		}
		static unsigned short SetTopExcl(unsigned short value, unsigned char index){
			return value | (0xFFFE<<index);
		}
		static short SetTopExcl(short value, unsigned char index){
			return value | (0xFFFE<<index);
		}
		static unsigned char SetTopExcl(unsigned char value, unsigned char index){
			return value | (0xFE<<index);
		}
		static char SetTopExcl(char value, unsigned char index){
			return value | (0xFE<<index);
		}

		/*Sets the bit at the specified index and all bits to the right of it.*/
		static unsigned long long int SetBotIncl(unsigned long long int value, unsigned char index){
			return value | ~(0xFFFFFFFFFFFFFFFEULL<<index);
		}
		static long long int SetBotIncl(long long int value, unsigned char index){
			return value | ~(0xFFFFFFFFFFFFFFFEULL<<index);
		}
		static unsigned int SetBotIncl(unsigned int value, unsigned char index){
			return value | ~(0xFFFFFFFE<<index);
		}
		static int SetBotIncl(int value, unsigned char index){
			return value | ~(0xFFFFFFFE<<index);
		}
		static unsigned short SetBotIncl(unsigned short value, unsigned char index){
			return value | ~(0xFFFE<<index);
		}
		static short SetBotIncl(short value, unsigned char index){
			return value | ~(0xFFFE<<index);
		}
		static unsigned char SetBotIncl(unsigned char value, unsigned char index){
			return value | ~(0xFE<<index);
		}
		static char SetBotIncl(char value, unsigned char index){
			return value | ~(0xFE<<index);
		}

		/*Sets all of the bits to the right of the specified bit index, but does not set the index bit.*/
		static unsigned long long int SetBotExcl(unsigned long long int value, unsigned char index){
			return value | ~(0xFFFFFFFFFFFFFFFFULL<<index);
		}
		static long long int SetBotExcl(long long int value, unsigned char index){
			return value | ~(0xFFFFFFFFFFFFFFFFULL<<index);
		}
		static unsigned int SetBotExcl(unsigned int value, unsigned char index){
			return value | ~(0xFFFFFFFF<<index);
		}
		static int SetBotExcl(int value, unsigned char index){
			return value | ~(0xFFFFFFFF<<index);
		}
		static unsigned short SetBotExcl(unsigned short value, unsigned char index){
			return value | ~(0xFFFF<<index);
		}
		static short SetBotExcl(short value, unsigned char index){
			return value | ~(0xFFFF<<index);
		}
		static unsigned char SetBotExcl(unsigned char value, unsigned char index){
			return value | ~(0xFF<<index);
		}
		static char SetBotExcl(char value, unsigned char index){
			return value | ~(0xFF<<index);
		}

		/*Returns the integer value of the logarithm, base 2, of a number.*/
		static unsigned char BinaryLog(unsigned long long int number){	//Tested well enough.  It runs CIDER 
			unsigned char answer = 0;
			if(number & 0xFFFFFFFF00000000ULL){
				answer|=32;
				number>>=32;
			}
			if(number & 0xFFFF0000ULL){
				answer|=16;
				number>>=16;
			}
			if(number & 0xFF00ULL){
				answer|=8;
				number>>=8;
			}
			if(number & 0xF0ULL){
				answer|=4;
				number>>=4;
			}
			if(number & 0xCULL){
				answer|=2;
				number>>=2;
			}
			if(number & 0x2ULL){
				answer|=1;
				number>>=1;
			}
			if(number)
				++answer;	//Dere aint no way to represent 64 in 5 bits, so you gots the plus.
			return answer;
		}
		static unsigned char BinaryLog(long long int number){
			unsigned char answer = 0;
			if(number & 0xFFFFFFFF00000000ULL){
				answer|=32;
				number>>=32;
			}
			if(number & 0xFFFF0000ULL){
				answer|=16;
				number>>=16;
			}
			if(number & 0xFF00ULL){
				answer|=8;
				number>>=8;
			}
			if(number & 0xF0ULL){
				answer|=4;
				number>>=4;
			}
			if(number & 0xCULL){
				answer|=2;
				number>>=2;
			}
			if(number & 0x2ULL){
				answer|=1;
				number>>=1;
			}
			if(number)
				++answer;
			return answer;
		}
		static unsigned char BinaryLog(unsigned int number){
			unsigned char answer = 0;
			if(number & 0xFFFF0000){
				answer|=16;
				number>>=16;
			}
			if(number & 0xFF00){
				answer|=8;
				number>>=8;
			}
			if(number & 0xF0){
				answer|=4;
				number>>=4;
			}
			if(number & 0xC){
				answer|=2;
				number>>=2;
			}
			if(number & 0x2){
				answer|=1;
				number>>=1;
			}
			if(number)
				++answer;
			return answer;
		}
		static unsigned char BinaryLog(int number){
			unsigned char answer = 0;
			if(number & 0xFFFF0000){
				answer|=16;
				number>>=16;
			}
			if(number & 0xFF00){
				answer|=8;
				number>>=8;
			}
			if(number & 0xF0){
				answer|=4;
				number>>=4;
			}
			if(number & 0xC){
				answer|=2;
				number>>=2;
			}
			if(number & 0x2){
				answer|=1;
				number>>=1;
			}
			if(number)
				++answer;
			return answer;
		}
		static unsigned char BinaryLog(unsigned short number){
			unsigned char answer = 0;
			if(number & 0xFF00){
				answer|=8;
				number>>=8;
			}
			if(number & 0xF0){
				answer|=4;
				number>>=4;
			}
			if(number & 0xC){
				answer|=2;
				number>>=2;
			}
			if(number & 0x2){
				answer|=1;
				number>>=1;
			}
			if(number)
				++answer;
			return answer;
		}
		static unsigned char BinaryLog(short number){
			unsigned char answer = 0;
			if(number & 0xFF00){
				answer|=8;
				number>>=8;
			}
			if(number & 0xF0){
				answer|=4;
				number>>=4;
			}
			if(number & 0xC){
				answer|=2;
				number>>=2;
			}
			if(number & 0x2){
				answer|=1;
				number>>=1;
			}
			if(number)
				++answer;
			return answer;
		}
		static unsigned char BinaryLog(unsigned char number){
			unsigned char answer = 0;
			if(number & 0xFF00){
				answer|=8;
				number>>=8;
			}
			if(number & 0xF0){
				answer|=4;
				number>>=4;
			}
			if(number & 0xC){
				answer|=2;
				number>>=2;
			}
			if(number & 0x2){
				answer|=1;
				number>>=1;
			}
			if(number)
				++answer;
			return answer;
		}
		static unsigned char BinaryLog(char number){
			unsigned char answer = 0;
			if(number & 0xFF00){
				answer|=8;
				number>>=8;
			}
			if(number & 0xF0){
				answer|=4;
				number>>=4;
			}
			if(number & 0xC){
				answer|=2;
				number>>=2;
			}
			if(number & 0x2){
				answer|=1;
				number>>=1;
			}
			if(number)
				++answer;
			return answer;
		}

		/*Prints the number out as a character array of ones and zeroes. (MSB is left)
		**Do not forget to delete the array when you're done, it's on the heap.** */
		static char * ToCharArray(unsigned long long int value){
			unsigned int i = sizeof(value)<<3;    //<<3 is equivalent to *8
			char * array = new char[i+1];
			do{										//The |48 is because 1 is 49, and 0 is 48 in ascii.
				array[--i] = (value&1)|48;           //Output a one or a zero.
				value >>= 1;
			}while(i);
			array[sizeof(value)<<3]=0;              //<<3 is equivalent to *8
			return array;
		}
		static char * ToCharArray(long long int value){
			unsigned int i = sizeof(value)<<3;
			char * array = new char[i+1];
			do{
				array[--i] = (value&1)|48;
				value >>= 1;
			}while(i);
			array[sizeof(value)<<3]=0;
			return array;
		}
		static char * ToCharArray(unsigned int value){
			unsigned int i = sizeof(value)<<3;
			char * array = new char[i+1];
			do{
				array[--i] = (value&1)|48;
				value >>= 1;
			}while(i);
			array[sizeof(value)<<3]=0;
			return array;
		}
		static char * ToCharArray(int value){
			unsigned int i = sizeof(value)<<3;
			char * array = new char[i+1];
			do{
				array[--i] = (value&1)|48;
				value >>= 1;
			}while(i);
			array[sizeof(value)<<3]=0;
			return array;
		}
		static char * ToCharArray(unsigned short value){
			unsigned int i = sizeof(value)<<3;
			char * array = new char[i+1];
			do{
				array[--i] = (value&1)|48;
				value >>= 1;
			}while(i);
			array[sizeof(value)<<3]=0;
			return array;
		}
		static char * ToCharArray(short value){
			unsigned int i = sizeof(value)<<3;
			char * array = new char[i+1];
			do{
				array[--i] = (value&1)|48;
				value >>= 1;
			}while(i);
			array[sizeof(value)<<3]=0;
			return array;
		}
		static char * ToCharArray(unsigned char value){
			unsigned int i = sizeof(value)<<3;
			char * array = new char[i+1];
			do{
				array[--i] = (value&1)|48;
				value >>= 1;
			}while(i);
			array[sizeof(value)<<3]=0;
			return array;
		}
		static char * ToCharArray(char value){
			unsigned int i = sizeof(value)<<3;
			char * array = new char[i+1];
			do{
				array[--i] = (value&1)|48;
				value >>= 1;
			}while(i);
			array[sizeof(value)<<3]=0;
			return array;
		}
	};
}

#endif
