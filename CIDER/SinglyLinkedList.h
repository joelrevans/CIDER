#ifndef SINGLYLINKEDLIST_CPP
#define SINGLYLINKEDLIST_CPP

#include "MutableList.h"
#include "ArrayList.h"

namespace Juice{

	/*This is a forward-linked list.  It is fast at accessing the beginning of the list, and slow towards the end.*/
	template<class T>
	class SinglyLinkedList : public MutableList<T>{

	protected:	//This all comes first because Linker has to be defined before it's referenced.

		/*Holds the length of the list.*/
		unsigned long long int length;

		virtual class Linker{
		public:
			Linker * next;
			T data;
			Linker(T Data, Linker * Next = 0){
				data = Data;
				next = Next;
			}   //This is like a constructor for a struct, so you can initialize on the heap.
		};

		/*A pointer to the first element in the list.*/
		Linker * foot;


	public:

		/*Default constructor.*/
		SinglyLinkedList(){
			length = 0;
		}

		/*Native array conversion constructor.*/
		SinglyLinkedList(T * source, unsigned long long int arrayLength){
			if(arrayLength){
				Linker * current = foot = new Linker(source[0]);
				for(unsigned long long int i = 1; i < arrayLength; ++i)
					current = current->next = new Linker(source[i]);
			}else
				length = 0;
		}

		/*Specialized copy constructor.*/
		SinglyLinkedList(SinglyLinkedList<T>& source){
			if(source.length){
				Linker * sourceCurrent = source.foot;
				Linker * current = foot = new Linker(sourceCurrent->data);
				for(length = 1ULL; length < source.length; ++length)
					current = current->next = new Linker((sourceCurrent = sourceCurrent->next)->data);
			}else
				length = 0;
		}

		/*Conversion operator (copy constructor).  Turns any List into a SinglyLinkedList.*/
		SinglyLinkedList(List<T>& source){
			Linker * current;
			foot = current = new Linker(source[0]);
			for(unsigned long long int i = 1ULL; i < source.Length(); ++i)
				current = current->next = new Linker(source[i]); //See insert for comments on this line.
			length = source.Length();
		}

		/*Equality Operator*/
		virtual SinglyLinkedList& operator=(SinglyLinkedList<T> source){
			/*First part deletes all data.*/
			Linker * current = foot;
			Linker * temp;
			while(length--){
				temp = current;
				current = current->next;
				delete temp;
			}
			length = source.length;

			/*Second part copies in new data.*/
			if(length){
				Linker * sourceCurrent = source.foot;
				current = foot = new Linker(source.foot->data);
				for(unsigned long long int i = 1ULL; i < length; ++i)
					current = current->next = new Linker((sourceCurrent=sourceCurrent->next)->data);
			}
			return *this;
		}

		/*Returns the length of the list.*/
		virtual unsigned long long int Length() override{
			return length;
		}

		T& Get(unsigned long long int index) override{														//Get single
			return seekIndex(index)->data;
		}

		/*Returns an array to items from the specified indicies.*/
		virtual ArrayList<T> Get(unsigned long long int index, unsigned long long int arrayLength) override{		//Get multiple
			ArrayList<T> returnVal = ArrayList<T>(arrayLength);
			Linker * current = seekIndex(index);
			for(unsigned long long int i = 0ULL; i < arrayLength; ++i){
				returnVal[i] = (*current).data;
				current = current->next;
			}
			return returnVal;
		}

		virtual ArrayList<T> Get(List<unsigned long long int>& indicies) override{
			ArrayList<T> returnVal = ArrayList<T>(indicies.Length());
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < indicies.Length(); ++i){
				for(unsigned long long int j = 0ULL; j < length; ++j){
					if(indicies.Contains(j)){						//You have to use contains because they aren't necessarily in order.
						returnVal[i] = current->data;
						current = current->next;
						break;										//Break here because the listed indicies aren't necessarily ordered.
					}
					current = current->next;
				}
			}
			return returnVal;
		}

		virtual T& operator[] (unsigned long long int index) override{												//Get single shorthand
			return Get(index);
		}

		virtual void Prepend(T source) override{																//Prepend single
			foot = new Linker(source, foot);
			++length;
		}

		/*Adds an array of items to the beginning of the list*/
		virtual void Prepend(List<T>& source) override{															//Prepend multiple shorthand
			for(unsigned long long int i = 0ULL; i < source.Length(); ++i)
				foot = new Linker(source[source.Length()-i-1], foot);
			length+=source.Length();
		}

		/*Copies an item and adds it to the end of the list.*/
		//NOTE: You can't have an append that accepts an object, because as a template class, you have no way to copy it.
		/*Adds an item to the end of the list.*/
		void Append(T item) override{
			if(length)
				seekIndex(length-1)->next = new Linker(item);
			else
				foot = new Linker(item);
			++length;
		}

		/*Adds an array of items to the end of the list.*/
		void Append(List<T>& items) override{
			Linker*last;
			if(length){
				last = seekIndex(length-1);
				for(unsigned long long int i = 0; i < items.Length(); ++i)
					last = last->next = new Linker(items[i]);
			}else if(items.Length()){
				last = foot = new Linker(items[0]);
				for(unsigned long long int i = 1; i < items.Length(); ++i)
					last = last->next = new Linker(items[i]);
			}
			length += items.Length();
		}

		/*Adds an item to the beginning of the array.*/
		virtual SinglyLinkedList& operator+=(T source) override{												//Prepend single shorthand
			Prepend(source);
			return *this;
		}

		virtual SinglyLinkedList& operator+=(List<T>& source) override{
			Prepend(source);
			return *this;
		}

		 /*Inserts an item into the list at the specified index.  Moves <item> and everything in a higher index up by one.*/
		virtual void Insert(T item, unsigned long long int index) override{						//Insert single
			Linker ** seek = getPointerToIndex(index);
			*seek = new Linker(item, *seek);
			++length;
		}

		/*Takes an array of items, and inserts them all into the array. Moves <item> and everything in a higher index up by <numElements>.*/
		virtual void Insert(List<T>& source, unsigned long long int index) override{					//Insert Multiple
			Linker ** nextPointer = getPointerToIndex(index);   //The first element in the array
			Linker * last = *nextPointer;                       //copies the data of the pointer (not the data it references)
			for(unsigned long long int i = 0ULL; i < source.Length(); ++i){
				*nextPointer = new Linker(source[i]);            //This sets the previous pointer (to the next obj) as well as defining the next object.
				nextPointer = &((**nextPointer).next);          //The pointer to the next object isn't yet known, so you need to set it afterwards.
			}
			(*nextPointer) = last;
			length += source.Length();
		}

		/*Removes all objects equal to the value presented.*/
		virtual unsigned long long int Remove(T target) override{
			unsigned long long int numRemoved = 0;
			while(foot->data == target){
				Linker * temp = foot;
				foot = foot->next;
				delete temp;
				++numRemoved;
			}

			Linker * current = foot;
			for(unsigned long long int i = 1ULL; i < length; ++i){
				if(current->next->data==target){
					Linker * temp = current->next;
					current->next = temp->next;
					delete temp;
					++numRemoved;
				}
			}
			length -= numRemoved;
			return numRemoved;
		}

		/*Removes all objects contained in the provided list.*/
		virtual unsigned long long int Remove(List<T>& targets) override{	//Untested
			unsigned long long int numRemoved = 0ULL;
			Linker ** current = &foot;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if(targets.Contains((*current)->data)){
					Linker * temp = *current;
					*current = temp->next;
					delete temp;
					++numRemoved;
				}
			}
			length -= numRemoved;
			return numRemoved;
		}

		virtual T RemoveAt(unsigned long long int index) override{	//RemoveAt single shorthand
			T returnRemoved;
			Linker * temp;
			if(index == 0){
				temp = (*foot).next;
				returnRemoved = foot->data;
				delete foot;
				foot = temp;
			}else{
				temp = seekIndex(index-1);
				Linker * skip = (*((*temp).next)).next;
				returnRemoved = temp->next->data;
				delete temp->next;
				(*temp).next = skip;
			}
			--length;
			return returnRemoved;
		}

		/*Removes a number of elements from the list.  The start index is included in the removal.*/
		virtual ArrayList<T> RemoveAt(unsigned long long int startIndex, unsigned long long int arraylength) override{		//RemoveAt multiple
			SinglyLinkedList<T> returnValues = SinglyLinkedList<T>();
			length -= arraylength;
			Linker ** start = getPointerToIndex(startIndex);
			Linker * current = *start;
			Linker * prev;
			do{
				prev = current;
				current = current->next;
				returnValues.Append(prev->data);
				delete prev;
			}while(--arraylength);
			*start = current;
			return ArrayList<T>(returnValues);
		}

		virtual ArrayList<T> RemoveAt(List<unsigned long long int>& indicies) override{
			SinglyLinkedList<T> returnValue = SinglyLinkedList();
			Linker ** current = &foot;
			unsigned long long int i = 0;
			unsigned long long int j = 0;
			for(unsigned long long int j = 0; j < indicies.Length(); ++j){
				while(i++ < indicies[j])
					current = &((*current)->next);
				returnValue.Append((**current).data);
				Linker * temp = *current;
				*current = temp->next;
				delete temp;
				++j;
			}
			return returnValue;
		}
		
		/*Removes and returns the first item.*/
		virtual T operator--() override{	//prefix operator
			T returnValue = foot->data;	//Store the data in the old foot temporarily
			Linker * temp = foot;
			foot = foot->next;			//Shift all linkers down, removing the foot.
			delete temp;				//Delete the old foot
			--length;
			return returnValue;
		}

		/*Removes and returns the last item.*/
		virtual T operator--(int) override{	//postfix operator
			Linker * last = seekIndex(length-1);
			T returnValue = last->data;
			delete last;
			--length;
			return returnValue;
		}

		/*Removes the item at the provided index from the list.*/
		virtual SinglyLinkedList& operator-=(T target) override{								//Remove single shorthand
			Remove(target);
			return *this;
		}

		virtual SinglyLinkedList& operator-=(List<T>& target) override{
			Remove(target);
			return *this;
		}

		virtual bool Contains(T searchTerm) override{	//untested
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if(current->data == searchTerm)
					return true;
				current = current->next;
			}
			return false;
		}

		virtual bool Contains(List<T>& searchTerms) override{	//untested
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				for(unsigned long long int j = 0ULL; j < searchTerms.Length(); ++j){
					if(current->data == searchTerms[j])
						return true;
				}
				current = current->next;
			}
			return false;
		}

		/*Reverses the order of elements in the list.*/
		void ReverseOrder(){
			Linker * next;
			Linker * current = foot;
			Linker * prev = 0;

			for(unsigned long long int i = 0ULL; i < length; ++i){
				next = current->next;
				current->next = prev;
				prev = current;
				current = next;
			}
			foot = prev;
		}

		/*Produces a copy of every element and returns the entire list as a native array*/
		virtual T * ToArray() override{
			T * returnVal = new T[length];
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				returnVal[i] = current->data;
				current = current->next;
			}
			return returnVal;
		}

		/*Searches the list for a copy of the data and, if found, returns the index of the first-matching object.  If no object is found, returns the length of the list.*/
		virtual unsigned long long int SearchSingle(T searchTerm) override{
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if(current->data == searchTerm)
					return i;
				current = current->next;
			}
			return length;
		}

		virtual ArrayList<unsigned long long int> Search(T searchTerm) override{		//untested
			SinglyLinkedList<unsigned long long int> results = SinglyLinkedList<unsigned long long int>();
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if(current->data == searchTerm)
					results.Append(i);
				current = current->next;
			}
			return ArrayList<unsigned long long int>(results);
		}

		virtual bool operator==(List<T>& compare) override{
			if(compare.Length() == length){
				Linker * current = foot;
				for(unsigned long long int i = 0ULL; i < compare.Length(); ++i){
					if(compare[i]!=current->data)
						return false;
					current=current->next;
				}
				return true;
			}
			return false;
		}

		/*The destructor for the list.  Clears the list and all elements.*/
		~SinglyLinkedList(){
			Linker * temp;
			while(length--){
				temp = foot;
				foot = (*foot).next;
				delete temp;
			}
		}


	protected:

		/*Returns the linker of the specified index.*/
		virtual Linker * seekIndex(unsigned long long int index){
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < index; ++i)
				current = current->next;
			return current;
		}

	private:
		/*Gets a pointer to the pointer of an index.*/
		Linker ** getPointerToIndex(unsigned long long int index){
			 if(index == 0)
				return &foot;

			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < index-1; ++i)
				current = current->next;    //uses a regular pointer to avoid unnecessary dereferences.
        
			return &(current->next);
		}
	};

}
#endif
