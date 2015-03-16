#ifndef ARRAYLIST_CPP
#define ARRAYLIST_CPP

#include "List.h"

namespace Juice{

	template <class T>
	class ArrayList : public List<T>{

	private:

		T * data;
		unsigned long long int length;

	public:

		ArrayList(){	//Default Constructor
			data = new T[length = 0ULL];
		}

		ArrayList(unsigned long long int listLength){
			data = new T[length = listLength];
		}

		ArrayList(T * source, unsigned long long int arrayLength){	//Array constructor (copies & converts from native array)
			data = new T[length = arrayLength];
			for(unsigned long long int i = 0ULL; i < arrayLength; ++i)
				data[i] = source[i];
		}

		ArrayList(ArrayList<T>& source){			//Actual Copy Constructor
			data = new T[length = source.Length()];
			for(unsigned long long int i = 0; i < source.Length(); ++i)
				data[i] = source[i];
		}

		ArrayList(List<T>& source){				//Generic Conversion constructor (Copy constructor)
			data = new T[length = source.Length()];
			for(unsigned long long int i = 0; i < source.Length(); ++i)
				data[i] = source[i];
		}

		ArrayList& operator=(List<T>& source){	//Assignment operator
			delete [] data;		//Clear out the old data
			data = new T[length = source.Length()];
			for(unsigned long long int i = 0ULL; i < length; ++i)
				data[i] = source[i];
			return *this;
		}

		ArrayList& operator=(ArrayList<T> source){	//Assignment operator2	/*NOTE:  This is NOT by reference, because in some cases (obj = ArrayList), a reference is never created!*/
			delete [] data;		//Clear out the old data
			data = new T[length = source.Length()];
			for(unsigned long long int i = 0ULL; i < length; ++i)
				data[i] = source[i];
			return *this;
		}

		virtual T& operator[](unsigned long long int index) override{
			return data[index];
		}

		virtual unsigned long long int Length() override{
			return length;
		}

		virtual void Length(unsigned long long int newLength){							//This function is the equivalent of realloc
			unsigned long long int maxCount = (newLength > length) ? length : newLength;	//The number of items copied will be the shorter of the two.
			T * newData = new T[length = newLength];
			for(unsigned long long int i = 0ULL; i < maxCount; ++i)
				newData[i] = data[i];
			delete [] data;																//Delete old data
			data = newData;																//Reassign location off new data
		}

		virtual bool Contains(T value) override{
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if((*this)[i] == value)
					return true;
			}
			return false;
		}

		virtual bool Contains(List<T>& value) override{
			for(unsigned long long int i = 0ULL; i < length; ++i){
				for(unsigned long long int j = 0ULL; j < length; ++j){
					if((*this)[i] == value[i])
						return true;
				}
			}
			return false;
		}

		virtual unsigned long long int SearchSingle(T value) override{
			unsigned long long int i = 0ULL;
			while((*this)[i]!=value)
				++i;
			return i;
		}
		virtual ArrayList<unsigned long long int> Search(T value) override{
			ArrayList<unsigned long long int> results(Length());
			unsigned long long int currentPos = 0;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if((*this)[i] == value)
					results[currentPos++] = i;
			}
			results.Length(currentPos);
			return results;
		}

		virtual bool operator==(List<T>& compare) override{
			if(compare.Length() == length){
				for(unsigned long long int i = 0ULL; i < compare.Length(); ++i){
					if(compare[i] != (*this)[i])
						return false;
				}
				return true;
			}
			return false;
		}
		
		virtual T * ToArray() override{
			T * returnVal = new T[length];
			for(unsigned long long int i = 0ULL; i < length; ++i)
				returnVal[i] = data[i];
			return returnVal;
		}
	
		~ArrayList(){
			delete [] data;
		}
	};

}

#endif
