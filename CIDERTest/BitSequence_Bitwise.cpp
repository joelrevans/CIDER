
#include "stdafx.h"
#include "CppUnitTest.h"
#include "BitSequence.h"
#include "CppUnitTestLogger.h"
#include "time.h"
#include <stdlib.h>
#include <exception>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Juice;
using namespace std;

//The number of iterations to run each test.  (most tests use random numbers).
#define numIterations 20000ULL

namespace TestBitwise
{		
	TEST_CLASS(JuiceTest)
	{
	public:

		TEST_CLASS_INITIALIZE(constructor){
			srand(time(NULL));	//Seed the random number generator with a random time.
		}

		TEST_METHOD(TestBitwiseAdd){
			int source[8];
			int source2[8];
			char * csource = (char*)source;
			char * csource2 = (char*)source2;

			unsigned long long int count = numIterations;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i)
				{
					csource[i] = rand();
					csource2[i] = rand();
				}

				BitSequence a(source, rand() % 257);
				BitSequence b(source2, rand() % 257);
				BitSequence computed((a.Length() > b.Length() ? a.Length() : b.Length()) + 1);

				int i = 0, carry = 0;
				while (i < computed.Length()){
					if (i < a.Length() && a.Read(i)){
						carry++;
					}
					if (i < b.Length() && b.Read(i)){
						carry++;
					}

					if (carry & 1)
						computed.Set(i);
					else
						computed.Clear(i);

					if (carry >= 2)
						carry = 1;
					else
						carry = 0;
					i++;
				}

				computed.BitwiseMinimize();

				//If the length of A is zero, the computed output will be 0, but the output has a length of at least one digit, even if zero.
				if (computed != BitSequence::BitwiseAdd(a, b)){
					Logger::WriteMessage("Bitwise addition disagrees with alternate calculation.");
					Logger::WriteMessage("\nNumber of random iterations: ");
					Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
					Logger::WriteMessage("\nAugend:\n");
					Logger::WriteMessage(a.ToCharArray());
					Logger::WriteMessage("\nAddend:\n");
					Logger::WriteMessage(b.ToCharArray());
					Logger::WriteMessage("\nOutput:\n");
					Logger::WriteMessage(BitSequence::BitwiseAdd(a, b).ToCharArray());
					Logger::WriteMessage("\nComputed:\n");
					Logger::WriteMessage(computed.ToCharArray());
					Assert::Fail();
				}
			}

			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestBitwiseSubtractAgainstAdd)
		{			
			int source[8];
			int source2[8];
			char * csource = (char*)source;
			char * csource2 = (char*)source2;

			unsigned long long int count = numIterations;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i)
				{
					csource[i] = rand();
					csource2[i] = rand();
				}

				BitSequence a(source, rand() % 257);
				BitSequence b(source2, rand() % 257);

				BitSequence computed = BitSequence::BitwiseSubtract(BitSequence::BitwiseAdd(a, b), b);
				computed.BitwiseMinimize();
				a.BitwiseMinimize();

				//If the length of A is zero, the computed output will be 0, but the output has a length of at least one digit, even if zero.
				if (a != computed && a.Length() != 0){
					Logger::WriteMessage("Bitwise addition and subtraction failed to invert.");
					Logger::WriteMessage("\nNumber of random iterations:\n");
					Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
					Logger::WriteMessage("\nMinuend:\n");
					Logger::WriteMessage(a.ToCharArray());
					Logger::WriteMessage("\nSubtrahend:\n");
					Logger::WriteMessage(b.ToCharArray());
					Logger::WriteMessage("\nOutput:\n");
					Logger::WriteMessage(computed.ToCharArray());
					Assert::Fail();
				}
			}

			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestClear0){
			int source[8];
			char * csource = (char*)source;


			unsigned long long int count = numIterations;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i)
					csource[i] = rand();

				BitSequence a(source, rand() % 257);
				a.Clear();
				for (int j = 0; j < a.Length(); ++j){
					if (a.Read(j)){
						Logger::WriteMessage("BitSequence failed to clear.");
						Logger::WriteMessage("\nOutput:\n");
						Logger::WriteMessage(a.ToCharArray());
						Logger::WriteMessage("Number of random iterations: ");
						Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
						Assert::Fail();
					}
				}
			}

			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestClear1){
			int source[8];
			char * csource = (char*)source;

			unsigned long long int count = numIterations;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i)
					csource[i] = rand();

				BitSequence a(source, rand() % 257);
				BitSequence b(source, a.Length());

				int position = 0;
				if (a.Length() == 0)
					continue;				
			
				//Check to make sure that every other bit is different
				for (int i = 0; i < a.Length(); ++i){

					//select a random bit to clear.
					position = (position + rand()) % a.Length();
					a.Clear(position);	//Clear a random bit.

					//check the bit that has been cleared.
					if (i == position)
					{
						if (a.Read(i))
						{
							Logger::WriteMessage("BitSequence failed to clear.");
							Logger::WriteMessage("\nOutput:\n");
							Logger::WriteMessage(a.ToCharArray());
							Logger::WriteMessage("\nNumber of random iterations:\n");
							Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
							Assert::Fail();
						}
					}	//Make sure that none of the other bits have changed.
					else if (a.Read(i) != b.Read(i))
					{
						Logger::WriteMessage("BitSequence failed to clear.");
						Logger::WriteMessage("\nOutput:\n");
						Logger::WriteMessage(a.ToCharArray());
						Logger::WriteMessage("\nNumber of random iterations:\n");
						Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
						Assert::Fail();
					}

					b.Clear(position);	//align b with a.
				}
			}

			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestClear2){
			int source[8];
			char * csource = (char*)source;

			unsigned long long int count = numIterations;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i){
					csource[i] = rand();
				}

				BitSequence a(source, rand() % 257);
				BitSequence b(source, a.Length());

				if (a.Length() == 0)
					continue;

				int position = rand() % a.Length();
				int clength = (a.Length() == position) ? 0 : rand() % (a.Length() - position);

				//Check to make sure that every other bit is different
				for (int i = 0; i < clength; ++i)
					a.Clear(position + i);
				b.Clear(position, clength);

				if ((a == b) == false){
					Logger::WriteMessage("BitSequence failed to clear.");
					Logger::WriteMessage("\nOutput:\n");
					Logger::WriteMessage(b.ToCharArray());
					Logger::WriteMessage("\nComputed:\n");
					Logger::WriteMessage(a.ToCharArray());
					Assert::Fail();
				}
			}

			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestSet0){
			int source[8];
			char * csource = (char*)source;

			unsigned long long int count = numIterations;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i)
					csource[i] = rand();

				BitSequence a(source, rand() % 257);
				BitSequence b(source, a.Length());

				int position = 0;
				if (a.Length() == 0)
					continue;

				//Check to make sure that every other bit is different
				for (int i = 0; i < a.Length(); ++i){

					//select a random bit to clear.
					position = (position + rand()) % a.Length();
					a.Set(position);	//Set a random bit.

					//check the bit that has been cleared.
					if (i == position)
					{
						if (a.Read(i) == false)
						{
							Logger::WriteMessage("BitSequence failed to clear.");
							Logger::WriteMessage("\nOutput:\n");
							Logger::WriteMessage(a.ToCharArray());
							Logger::WriteMessage("\nNumber of random iterations:\n");
							Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
							Assert::Fail();
						}
					}	//Make sure that none of the other bits have changed.
					else if (a.Read(i) != b.Read(i))
					{
						Logger::WriteMessage("BitSequence failed to clear.");
						Logger::WriteMessage("\nOutput:\n");
						Logger::WriteMessage(a.ToCharArray());
						Logger::WriteMessage("\nNumber of random iterations:\n");
						Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
						Assert::Fail();
					}

					b.Set(position);	//align b with a.
				}
			}

			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestSet1){
			int source[8];
			char * csource = (char*)source;

			unsigned long long int count = numIterations;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i){
					csource[i] = rand();
				}

				BitSequence a(source, rand() % 257);
				BitSequence b(source, a.Length());

				if (a.Length() == 0)
					continue;

				int position = rand() % a.Length();
				int clength = (a.Length() == position) ? 0 : rand() % (a.Length() - position);

				//Check to make sure that every other bit is different
				for (int i = 0; i < clength; ++i)
					a.Set(position + i);
				b.Set(position, clength);

				if ((a == b) == false){
					Logger::WriteMessage("BitSequence failed to clear.");
					Logger::WriteMessage("\nOutput:\n");
					Logger::WriteMessage(b.ToCharArray());
					Logger::WriteMessage("\nComputed:\n");
					Logger::WriteMessage(a.ToCharArray());
					Assert::Fail();
				}
			}

			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestShiftUp){
			int source[8];
			char * csource = (char*)source;

			unsigned long long int count = numIterations;

			BitSequence t1;
			BitSequence t2;
			int slen, j;;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i)
					csource[i] = rand();

				BitSequence a(source, rand() % 257);
				BitSequence b(source, a.Length());
					
				t1 = a;
				t2 = b;					

				int shiftlen = 0;

				if (a.Length() != 0)
					shiftlen = rand() % a.Length() * 4 / 3;	//Lets the shift exceed the length by over one full long int.

				slen = shiftlen;

				for (int i = 0; i + shiftlen < a.Length(); ++i)
					a.Read(i) ? a.Set(i + shiftlen) : a.Clear(i + shiftlen);
				if (shiftlen > a.Length())
					a.Clear();
				else
					a.Clear(0, shiftlen);

				b.ShiftUp(shiftlen);
			}


			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestShiftDown){
			int source[8];
			char * csource = (char*)source;

			unsigned long long int count = numIterations;

			BitSequence t1;
			BitSequence t2;
			int slen, j;;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i)
					csource[i] = rand();

				BitSequence a(source, rand() % 257);
				BitSequence b(source, a.Length());

				t1 = a;
				t2 = b;

				int shiftlen = 0;

				if (a.Length() != 0)
					shiftlen = rand() % a.Length() * 4 / 3;	//Lets the shift exceed the length by over one full long int.

				slen = shiftlen;

				for (int i = 0; i + shiftlen < a.Length(); ++i)
					a.Read(i + shiftlen) ? a.Set(i) : a.Clear(i);
				if (shiftlen > a.Length())
					a.Clear();
				else
					a.Clear(a.Length() - shiftlen, shiftlen);
				b.ShiftDown(shiftlen);
			}


			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestBitwiseMultiply){	//This test is really slow.  Might want to drop the number of iterations.
			int source[8];
			int source2[8];
			char * csource = (char*)source;
			char * csource2 = (char*)source2;

			unsigned long long int count = numIterations;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i)
				{
					csource[i] = rand();
					csource2[i] = rand();
				}

				BitSequence a(source, rand() % 257);
				BitSequence b(source2, rand() % 257);

				BitSequence computed(a.Length() + b.Length());
				computed.Clear();

				for (unsigned long long int x = 0ULL; x < a.Length(); ++x)
				{
					if (a.Read(x))
					{
						for (unsigned long long int y = 0ULL; y < b.Length(); ++y)
						{
							if (b.Read(y))
							{
								unsigned long long int i = 0ULL;
								while (computed.Read(x + y + i))
								{
									computed.Clear(x + y + i);
									++i;
								}
								computed.Set(x + y + i);
							}
						}
					}
				}

				computed.BitwiseMinimize();

				//If the length of A is zero, the computed output will be 0, but the output has a length of at least one digit, even if zero.
				if (computed != BitSequence::BitwiseMultiply(a, b)){
					Logger::WriteMessage("Bitwise multiplication disagrees with alternate calculation.");
					Logger::WriteMessage("\nNumber of random iterations: ");
					Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
					Logger::WriteMessage("\nMultiplicand:\n");
					Logger::WriteMessage(a.ToCharArray());
					Logger::WriteMessage("\nMultiplier:\n");
					Logger::WriteMessage(b.ToCharArray());
					Logger::WriteMessage("\nOutput:\n");
					Logger::WriteMessage(BitSequence::BitwiseMultiply(a, b).ToCharArray());
					Logger::WriteMessage("\nComputed:\n");
					Logger::WriteMessage(computed.ToCharArray());
					Assert::Fail();
				}
			}

			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

		TEST_METHOD(TestBitwiseDivideAgainstMultiply){
			int source[8];
			int source2[8];
			char * csource = (char*)source;
			char * csource2 = (char*)source2;

			unsigned long long int count = numIterations;

			while (--count)
			{
				for (int i = 0; i < 8 * sizeof(int) / sizeof(char); ++i)
				{
					csource[i] = rand();
					csource2[i] = rand();
				}

				BitSequence a(source, rand() % 257);
				BitSequence b(source2, rand() % 257);

				BitSequence computed = BitSequence::BitwiseDivide(BitSequence::BitwiseMultiply(a, b), b);
				computed.BitwiseMinimize();
				a.BitwiseMinimize();

				//If the length of A is zero, the computed output will be 0, but the output has a length of at least one digit, even if zero.
				if (a != computed && b.Length() != 0)
				{
					Logger::WriteMessage("Bitwise addition and subtraction failed to invert.");
					Logger::WriteMessage("\nNumber of random iterations:\n");
					Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
					Logger::WriteMessage("\nMultiplicand:\n");
					Logger::WriteMessage(a.ToCharArray());
					Logger::WriteMessage("\nMultiplier:\n");
					Logger::WriteMessage(b.ToCharArray());
					Logger::WriteMessage("\nOutput:\n");
					Logger::WriteMessage(computed.ToCharArray());
					Assert::Fail();
				}
			}

			Logger::WriteMessage("Number of random iterations: ");
			Logger::WriteMessage(Integer::ToCharArray(numIterations - count));
		}

	};
}