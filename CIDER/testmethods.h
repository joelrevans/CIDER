#include <dos.h>
#include <iostream>
#include "BitSequence.h"

using namespace std;
using namespace Juice;


void TestEndianness(){
	unsigned int * test = new unsigned int();
    cout << hex << *test << endl;
    unsigned char * test2;
    test2 = (unsigned char*)test;
    *test2 = 0xCF;
    cout << hex << (unsigned int) *test2 << endl;
    cout << hex << *test << endl;

    *test |= 0x00100000;
    cout << "Test2+0:  " << hex << (unsigned int) *(test2+0) << endl;
    cout << "Test2+1:  "<< hex << (unsigned int) *(test2+1) << endl;
    cout << "Test2+2:  "<< hex << (unsigned int) *(test2+2) << endl;
    cout << "Test2+3:  "<< hex << (unsigned int) *(test2+3) << endl;
    cout << "Test2+0:  " << hex << *test << endl;

    *test2 = 0xFF;
    *test2>>=2;
    cout << hex << (unsigned int) *test2 << endl;
    cout << hex << *test << endl;

    *test2 = 0xFC;
    *(test2+1) = 0x3;
    *test>>=2;
    cout << hex << (unsigned int) *test2 << endl;
    cout << hex << *test << endl;
}

void TestBitSequence(){	//Tests the second constructor
	unsigned long long int a[4] = {0xF0F0F0F0F0F0F0F0ULL,0xF0F0F0F0F0F0F0F0ULL,0xF0F0F0F0F0F0F0F0ULL,0xF0F0F0F0F0F0F0F0ULL};
	cout << hex << a[0] << endl;
    {
		BitSequence b(a, 0, 0);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 0, 24);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 0, 63);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 0, 64);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 0, 65);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 0, 127);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 0, 128);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 0, 129);
		cout << b.ToCharArray() << endl;
    }

	{
		BitSequence b(a, 5, 127);
		cout << b.ToCharArray() << endl;
    }
	{
		BitSequence b(a, 6, 127);
		cout << b.ToCharArray() << endl;
    }
	{
		BitSequence b(a, 7, 127);
		cout << b.ToCharArray() << endl;
    }

	{
		BitSequence b(a, 5, 128);
		cout << b.ToCharArray() << endl;
    }
	{
		BitSequence b(a, 6, 128);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 7, 128);
		cout << b.ToCharArray() << endl;
    }
    
	{
		BitSequence b(a, 5, 129);
		cout << b.ToCharArray() << endl;
    }
	{
		BitSequence b(a, 6, 129);
		cout << b.ToCharArray() << endl;
    }
	{
		BitSequence b(a, 7, 129);
		cout << b.ToCharArray() << endl;
    }
	
    //Starts testing the second constructor here.
    {
		BitSequence b(a, 0);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 24);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 63);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 64);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 65);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 127);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 128);
		cout << b.ToCharArray() << endl;
    }
    {
		BitSequence b(a, 129);
		cout << b.ToCharArray() << endl;
    }
}

void TestRead(){    //single bit
    unsigned long long int a[4] = {0xF0F0F0F0F0F0F0F0ULL,0xF0F0F0F0F0F0F0F0ULL,0xF0F0F0F0F0F0F0F0ULL,0xF0F0F0F0F0F0F0F0ULL};
	cout << hex << a[0] << endl;

    unsigned int length = 129;

    BitSequence c(a, 0, length);
    for(unsigned int i = 0; i < length; ++i){
        cout << c.Read(i);
    }
    cout << endl;
}

void TestSetClear(){  //Aight
    unsigned long long int a[4] = {0ULL, 0ULL, 0ULL, 0ULL};

	unsigned int blen = 3000;
    BitSequence b(a, 0, blen);

    for(unsigned int i = 0; i < blen; i+=2)
        b.Set(i);
    cout << b.ToCharArray() << endl << endl;

    for(unsigned int i = 1; i < blen; i+=2)
        b.Set(i);
    cout << b.ToCharArray() << endl << endl;

    for(unsigned int i = 0; i < blen; i+=2)
        b.Clear(i);
    cout << b.ToCharArray() << endl << endl;

    for(unsigned int i = 1; i < blen; i+=2)
        b.Clear(i);
    cout << b.ToCharArray() << endl << endl;

}

void TestShiftUp(){
    unsigned long long int a[4] = {0x0F0F0F0F0F0F0F0FULL,0x0F0F0F0F0F0F0F0FULL,0x0F0F0F0F0F0F0F0FULL,0x0F0F0F0F0F0F0F0FULL};

	for(int i = 0; i < 257; ++i)
    {
        BitSequence b(a, 0, 256);
        b.ShiftUp(i);
        cout << b.Read(186, 70).ToCharArray() << "  " << (i) << endl;
    }

	for(int i = 0; i < 70; ++i)
    {
        BitSequence b(a, 0, 256);
        b.ShiftUp(i);
        cout << b.Read(0, 70).ToCharArray() << "  " << (i) << endl;
    }
}

void TestShiftDown(){
    unsigned long long int a[4] = {0xF0F0F0F0F0F0F0F0ULL,0xF0F0F0F0F0F0F0F0ULL,0xF0F0F0F0F0F0F0F0ULL,0xF0F0F0F0F0F0F0F0ULL};

	for(int i = 0; i < 256; ++i)
    {
        BitSequence b(a, 0, 256);
        b.ShiftDown(i);
        cout << b.Read(0, 70).ToCharArray() << "  " << (i) << endl;
    }

	for(int i = 0; i < 70; ++i)
    {
        BitSequence b(a, 0, 256);
        b.ShiftDown(i);
        cout << b.Read(186, 70).ToCharArray() << "  " << (i) << endl;
    }
}

void TestSet(){
	unsigned long long int a[4] = {0ULL, 0ULL, 0ULL, 0ULL};

	for(int i = 0; i < 256; i+=8){
		BitSequence b(a, 0, 256);
		b.Set(i, 256-i);
		cout << b.ToCharArray() << endl;
    }
    for(int i = 0; i < 256; i+=8){
		BitSequence b(a, 0, 256);
		b.Set(0, i);
		cout << b.ToCharArray() << endl;
    }
}

void TestClear(){
	unsigned long long int a[4] = {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL};

	for(int i = 0; i < 256; i+=8){
		BitSequence b(a, 0, 256);
		b.Clear(i, 256-i);
		cout << b.ToCharArray() << endl;
    }
    for(int i = 0; i < 256; i+=8){
		BitSequence b(a, 0, 256);
		b.Clear(0, i);
		cout << b.ToCharArray() << endl;
    }
}

void TestRead2(){    //Multiple bits.
    unsigned long long int a[4] = {0x3030303030303030ULL,0x3030303030303030ULL,0x3030303030303030ULL,0x3030303030303030ULL};
    //unsigned long long int a[4] = {0xFFFFFFFFFFFFFFFFULL,0xFFFFFFFFFFFFFFFFULL,0xFFFFFFFFFFFFFFFFULL,0xFFFFFFFFFFFFFFFFULL};

    BitSequence b(a, 0, 256);
    for(int i = 0; i < 256; i+=25){
		cout << b.Read(0, i).ToCharArray() << endl;
    }
	for(int i = 0; i+72 < 256; i+=21){
		cout << b.Read(i, 72).ToCharArray() << endl;
    }
    for(int i = 0; i < 11; i++){
		cout << b.Read(0, i).ToCharArray() << endl;
    }
	for(int i = 0; i < 10; i++){
		cout << b.Read(i, 20).ToCharArray() << endl;
    }
	for(int i = 0; i < 30; i++){
		cout << b.Read(i, i).ToCharArray() << endl;
    }
}

void TestWrite(){
    unsigned long long int a[] = {0ULL, 0ULL, 0ULL, 0ULL, 0ULL};
    //unsigned long long int a[4] = {0x3030303030303030ULL,0x3030303030303030ULL,0x3030303030303030ULL,0x3030303030303030ULL};
    unsigned long long int c[] = {-1ULL, -1ULL, -1ULL, -1ULL, -1ULL};
	
	for(int i = 0; i < 54; i+=4){
		BitSequence b(a, 63);
		BitSequence d(c, 10);
		b.Write(i, d);
		cout << b.ToCharArray() << endl;
	}
	cout << endl;

    for(int i = 0; i+126 < 192; i+=7){
        BitSequence b(a, 192);
        BitSequence d(c, 126);
        b.Write(i, d);
        cout << b.ToCharArray() << endl;
    }
	cout << endl;

	for(int i = 0; i+126 < 193; i+=7){
        BitSequence b(a, 193);
        BitSequence d(c, 126);
        b.Write(i, d);
        cout << b.ToCharArray() << endl;
    }
	cout << endl;
}

void TestSplitMerge(){
	char source[] = {'T', 'H','I','S',' ','I','S',' ','A',' ','S','T','I','C','K','U','P',};
	BitSequence b(source, 80);
	ArrayList<unsigned long long int> c(10);
	c[0] = 0; c[1] = 8; c[2] = 16; c[3] = 24; c[4] = 32; c[5] = 40; c[6] = 48; c[7] = 56; c[8] = 64; c[9] = 72;
	ArrayList<BitSequence> d = b.Split(c);
	for(unsigned long long int i = 0; i < d.Length(); ++i)
		//cout << d[i].ToCharArray() << endl;
		cout << d[i].ToObject<char>(0) << endl;

	cout << BitSequence::Merge(d).ToArray<char>() << endl;
}

void TestReplaceRemove(){
	//Replace
	char source[] = {'T','H','I','S',' ','I','S',' ','A',' ','S','T','I','C','K','U','X'};
	BitSequence b(source, 128);
	ArrayList<unsigned long long int> c(8);
	c[0] = 0; c[1] = 16; c[2] = 32; c[3] = 48; c[4] = 64; c[5] = 80; c[6] = 96; c[7] = 112;
	BitSequence d(source+16, 8);
	b.Replace(c, 8, d);
	cout << b.ToArray<char>() << endl;
	//Remove
	b = BitSequence(source, 128);
	b.Remove(c, 8);
	cout << b.ToArray<char>() << endl;
}

void TestBitwiseMinimize()
{
	unsigned long long int source[] = {0x3333333333333330ULL, 0x3333333333333333ULL, 0x3333333333333333ULL, 0x3333333333333333ULL};
	
	for(int i = 1; i < 64; i++)
	{
		BitSequence b(((unsigned char*)source)+(i/8), i%8, i*3);
		cout << b.ToCharArray() << endl;
		b.BitwiseMinimize();
		cout << b.ToCharArray() << endl;
	}
}

void VerifyBitwiseCommunative(){
	unsigned long long int source[4];
	unsigned long long int source2[4];
	
	unsigned long long int multiply=0ULL, add=0ULL, xor=0ULL, equals=0ULL;
	while(1)
	{
		BitSequence a(source, rand()%257);
		BitSequence b(source2, rand()%257);

		if(BitSequence::BitwiseMultiply(a, b)!=BitSequence::BitwiseMultiply(b, a))
		{
			cout << BitSequence::BitwiseXOR(BitSequence::BitwiseMultiply(a, b),BitSequence::BitwiseMultiply(b, a)).ToCharArray() << endl << endl;
			cout << "Bitwise Multiply Failure" << endl;
			break;
		}
		else 
			if(BitSequence::BitwiseAdd(a, b)!=BitSequence::BitwiseAdd(b, a))
		{
			cout << "Bitwise Add Failure" << endl;
			break;
		}
		else if(BitSequence::BitwiseXOR(a, b)!=BitSequence::BitwiseXOR(b, a))
		{
			cout << "Bitwise XOR Failure" << endl;
			break;
		}
		else if((a==b&&b!=a)||(b==a&&a!=b))
		{
			cout << "Bitwise Equals/NotEquals Failure" << endl;
			break;
		}
		multiply++;
		add++;
		xor++;
		equals++;
		system("cls");
		cout << "multiply:  " << multiply << endl;
		cout << "add:  " << add << endl;
		cout << "xor:  " << xor << endl;
		cout << "equals:  " << equals << endl;
	}	
	system("pause");
}

void TestBitwiseMultiply()
{
	unsigned long long int source0[] = {0x0000000000000001ULL, 0x0000000000000000ULL};
	unsigned long long int source1[] = {0xF0F0F0F038911876ULL, 0x00182a9df8c87698ULL};
	BitSequence a(source0, 128);
	BitSequence b(source1, 128);

	cout << b.ToCharArray() << endl << endl;							//1xb
	cout << BitSequence::BitwiseMultiply(a, b).ToCharArray() << endl << endl;

	cout << b.ToCharArray() << endl << endl;							//3xb
	a.Set(1); a.Clear(0);
	cout << BitSequence::BitwiseMultiply(a, b).ToCharArray() << endl << endl;

	cout << b.ToCharArray() << endl << endl;
	a.Clear(1); a.Set(63);												//Shift up by 64.
	cout << BitSequence::BitwiseMultiply(a, b).ToCharArray() << endl << endl;
}

void TestBitwiseDivide()
{
	unsigned long long int source[] = {0x3D2711CB4FB21LL, 0x941CD72F254E4ULL, 0x3529FD89F7CA6ULL, 0xAFD471C9B4FE3ULL};
	unsigned long long int source2[] = {0xFCA95CAF2B5BBC16ULL, 0xA59E67CB29DEE48FULL, 0xB742EAFAB782F6DAULL, 0xBC687167DA3C1987ULL};

	BitSequence a(source, 256);
	BitSequence b(source2, 256);

	a.BitwiseMinimize();
	b.BitwiseMinimize();

	cout << BitSequence::BitwiseDivide(a, a).ToCharArray();
	cout << endl << endl;

	cout << a.ToCharArray() << endl << endl;
	cout << BitSequence::BitwiseDivide(a, BitSequence::Create(2, 2)).ToCharArray();
	cout << endl << endl;
	cout << BitSequence::BitwiseDivide(a, BitSequence::Create(8, 4)).ToCharArray();
	cout << endl << endl;

	cout << a.ToCharArray() << endl << endl;
	cout << BitSequence::BitwiseDivide(BitSequence::BitwiseMultiply(a, b), b).ToCharArray()<<endl;
	cout << endl << endl << endl;

	cout << b.ToCharArray() << endl << endl;
	cout << BitSequence::BitwiseDivide(BitSequence::BitwiseMultiply(a, b), a).ToCharArray()<<endl<<endl;
	cout << BitSequence::BitwiseDivide(BitSequence::BitwiseMultiply(b, a), a).ToCharArray()<<endl;
	cout << endl << endl << endl;

	cout << a.ToCharArray() << endl << endl;
	cout << BitSequence::BitwiseMultiply(BitSequence::BitwiseDivide(a, b), b).ToCharArray()<<endl;
	cout << endl << endl << endl;

	cout << b.ToCharArray() << endl << endl;
	cout << BitSequence::BitwiseMultiply(BitSequence::BitwiseDivide(b, a), a).ToCharArray()<<endl;
	cout << endl << endl << endl;

	a.Length(200);
	b.Length(200);
}

void VerifyBitwiseModulus(){
	unsigned long long int source[4];
	unsigned long long int source2[4];
	
	unsigned long long int modulusCount = 0ULL;
	while(1)
	{
		BitSequence a(source, rand()%257);
		BitSequence b(source2, rand()%257);

		a.BitwiseMinimize();
		b.BitwiseMinimize();

		BitSequence modulus = BitSequence::BitwiseModulus(a, b);
		BitSequence quotient = BitSequence::BitwiseDivide(a, b);

		BitSequence start = BitSequence::BitwiseAdd(BitSequence::BitwiseMultiply(quotient, b),modulus);
		start.BitwiseMinimize();
		
		cout << BitSequence::BitwiseXOR(start, a).ToCharArray() << endl << endl << endl;

		if(start==a)
		{
			modulusCount++;
		}
		else
		{
			cout << "Bitwise Modulus Failure" << endl;
			break;
		}

		system("cls");
		cout << "Modulo Random Ops:  " << modulusCount << endl;
	}	
	system("pause");
}