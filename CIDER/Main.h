using namespace std;

#include <iostream>

#include "BitPatternTree.h"
#include <fstream>
#include "testmethods.h"
#include "SpikedCider.h"

#include <direct.h>

#include "ProbabilityDistribution.h"

//#include <stdlib.h>

using namespace Juice;

int main(){

	//char currentPath[1000];

	//cout << _getcwd(currentPath, sizeof(currentPath) / sizeof(char)) << endl;

	//cout << (-1ULL>>64ULL) << endl << (-1ULL<<64ULL) << endl;
	//system("pause");


	//VerifyBitwiseCommunative();

	unsigned long long int test[] = 
	{
		0xfFE1E5670E56C170ULL, 0x1F150FD1B32A30EFULL, 0xBC44C5B466A4AE76ULL, 0xBC44C5B466A4AE76ULL
	};
	BitSequence a(test, 2);
	BitSequence b(test, 128);
	cout << BitSequence::BitwiseMultiply(a, b).ToCharArray() << endl << endl;
	



	cout << "rand max: " << RAND_MAX << endl;

	fstream fs;
	
	/*Test if it opens.*/
	fs.open("enwik");
	cout << "is_open:  " << fs.is_open() << endl;
	
	fs.seekg(0, ios::end);
	unsigned long long int byteLength = fs.tellg();

	char * enwik = new char[byteLength];
	fs.seekg(0, ios::beg);
	fs.read(enwik, byteLength);
	fs.close();
	
	cout << "Data copied.  Creating Sequence..." << endl;

	BitSequence enwikBin = BitSequence(enwik, byteLength*8);

	delete [] enwik;

	cout << "Pre-compression Length:  " << enwikBin.Length() << endl;
	BitSequence enwikCompare = enwikBin;	//Save the data for verification.
	
	SpikedCider::MacroPress(enwikBin, 11);	
	cout << "Post-compression length:  " << enwikBin.Length() << endl;

	SpikedCider::MacroDepress(enwikBin);

	cout << "Verifying data integrity..." << endl;
	cout << (enwikCompare==enwikBin?"Data Integrity Verified":"Failure")<<endl;

	system("pause");
    return 0;
}/**/
