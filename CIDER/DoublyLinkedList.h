#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "List.h"
#include "SinglyLinkedList.h"
#include "ArrayList.h"

namespace Juice{

	template <class T>
	class DoublyLinkedList : public SinglyLinkedList<T>{
	
	protected:

		virtual class Linker{
		public:
			Linker * next;
			Linker * prev;
			T data;
			Linker(T Data, Linker * Next = 0, Linker * Prev = 0){
				data = Data;
				next = Next;
				prev = Prev;
			}
			Linker(Linker * Next = 0, Linker * Prev = 0){
				next = Next;
				prev = Prev;
			}
			Linker(){}
		};

	public:

		//No default constructor necessary.  The superclass constructor handles it.

		/*Native Array constructor*/
		DoublyLinkedList(T * source, unsigned long long int arrayLength) : SinglyLinkedList(){
			if(arrayLength){
				Linker * init = foot = new Linker(source[0]);
				for(length = 1ULL; length < arrayLength; ++length){
					foot = new Linker(source[length], 0, foot);
					foot->prev->next = foot;
				}
				init->prev = foot;
				foot = foot->next = init;
			}
			//else{ Length should be set to zero, but base class handles that } 
		}
		
		/*Copy/Conversion Constructor*/
		DoublyLinkedList(List<T>& source) : SinglyLinkedList(){
			Append(source);
		}

		/*Equality Operator*/
		virtual DoublyLinkedList& operator=(DoublyLinkedList<T> source){	//NOT BY REFERENCE!  Allows for stack allocation (stack objects, which have no pointer, would otherwise do a shallow copy).
			/*First part deletes all data.*/
			Linker * current = foot;
			Linker * temp;
			while(length--){
				temp = current;
				current = current->next;
				delete temp;
			}
			length = source.Length();

			/*Second part copies in new data.*/
			if(length){
				Linker * sourceCurrent = source.foot;
				current = foot = new Linker(source[0]);
				for(unsigned long long int i = 1ULL; i < length; ++i)
					current = current->next = new Linker((sourceCurrent=sourceCurrent->next)->data);
			}
			return *this;
		}

		virtual void Prepend(T source) override{
			if(length)
				foot->next = new Linker(source, foot->next, foot);
			else{
				foot = new Linker(source);
				foot->prev = foot->next = foot;
			}
			++length;
		}

		virtual void Prepend(List<T>& source) override{
			unsigned long long int i;
			if(length)
				i = 0;
			else{
				foot = new Linker(source[0]);
				foot->prev = foot->next = foot;
				i = 1;
			}
			for(i; i < source.Length(); ++i)
				foot->prev = foot->prev->next = new Linker(source[source.Length() - 1 - i], foot, foot->prev);
			length+=source.Length();
		}

		virtual void Append(T source) override{
			if(length)
				foot->prev = foot->prev->next = new Linker(source, foot, foot->prev);
			else{
				foot = new Linker(source);
				foot->prev = foot->next = foot;
			}
			++length;
		}

		virtual void Append(List<T>& source) override{
			unsigned long long int i;
			if(!length){
				foot = new Linker(source[0]);
				foot->next = foot->prev = foot;
				i = 1;
			}else
				i= 0;

			for(i; i < source.Length(); ++i)
				foot->prev = foot->prev->next = new Linker(source[i], foot, foot->prev);
			length += source.Length();
		}

		virtual DoublyLinkedList& operator+=(T source) override{
			Append(source);
			return *this;
		}

		virtual DoublyLinkedList& operator+=(List<T>& source) override{
			Append(source);
			return *this;
		}

		virtual void Insert(T source, unsigned long long int index) override{
			if(length){
				Linker * temp = seekIndex(index);
				temp->prev = temp->prev->next = new Linker(source, temp, temp->prev);
				++length;
			}else{
				foot = new Linker(source);
				foot->next = foot->prev = foot;
				++length;
			}
		}

		virtual void Insert(List<T>& source, unsigned long long int index) override{
			if(length){
				Linker * current = seekIndex(index);
				Linker * last = current->next;
				for(unsigned long long int i = 0ULL; i < source.Length(); ++i)
					current = current->next = new Linker(source[i], 0, current);
				current->next = last;
				length+=source.Length();
			}else{
				foot = new Linker(source[0]);
				foot->next = foot->prev = foot;
				for(unsigned long long int i = 1ULL; i < source.Length(); ++i)
					foot->prev = new Linker(source[i], foot, foot->prev);
				length += source.Length();
			}
		}

		virtual unsigned long long int Remove(T compare) override{
			unsigned long long int numRemoved = 0ULL;
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if(current->data == compare){
					current->prev->next = current->next;
					current->next->prev = current->prev;
					Linker * temp = current;
					current = current->prev;	//Because of the command after this if statement.
					delete temp;
					++numRemoved;
				}
				current = current->next;
			}
			return numRemoved;
		}

		virtual unsigned long long int Remove(List<T>& compare) override{
			unsigned long long int numRemoved = 0ULL;
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if(compare.Contains(current->data)){
					current->next->prev = current->prev;
					current->prev->next = current->next;
					Linker * temp = current;
					current = current->prev;
					delete temp;
					++numRemoved;
				}
				current = current->next;
			}
			return numRemoved;
		}

		virtual T RemoveAt(unsigned long long int index) override{
			Linker * temp = seekIndex(index);
			temp->next->prev = temp->prev;
			temp->prev->next = temp->next;
			T data = temp->data;
			delete temp;
			--length;
			return data;
		}
		
		virtual ArrayList<T> RemoveAt(unsigned long long int index, unsigned long long int length) override{
			ArrayList<T> returnVal = ArrayList<T>(length);
			Linker * current = seekIndex(index);
			for(unsigned long long int i = 0ULL; i < length; ++i){
				returnVal[i] = current->data;
				Linker * temp = current;
				current->prev->next = current->next;
				current->next->prev = current->prev;
				current = current->next;
				delete temp;
			}
			return returnVal;
		}

		virtual ArrayList<T> RemoveAt(List<unsigned long long int>& indicies) override{
			unsigned long long int j = 0ULL;
			ArrayList<T> returnVal = ArrayList<T>(indicies.Length());
			Linker * current = foot;
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if(indicies.Contains(i)){
					returnVal[++j]=current->data;
					current->prev->next = current->next;
					current->next->prev = current->prev;
					Linker * temp = current;
					current = current->next;
					delete temp;
					if(j==indicies.Length())
						return returnVal;
				}
				current=current->next;
			}
		}

		virtual T operator--() override{	//Prefix
			Linker * temp = foot;
			T returnVal = foot->data;
			foot->prev->next = foot->next;
			foot->next->prev = foot->prev;
			foot = foot->next;
			delete temp;
			--length;
			return returnVal;
		}

		virtual T operator--(int) override{	//Postfix
			Linker * temp = foot->prev;
			T returnVal = temp->data;
			foot->prev->prev->next = foot;
			foot->prev = foot->prev->prev;
			delete temp;
			--length;
			return returnVal;
		}

		virtual ArrayList<unsigned long long int> Search(T searchTerm) override{
			Linker * current = foot->prev;	//Start @ last and count backwards (cuz of SinglyLinkedList)
			SinglyLinkedList<unsigned long long int> returnVal = SinglyLinkedList<unsigned long long int>();
			for(unsigned long long int i = 0ULL; i < length; ++i){
				if(searchTerm == current->data)
					returnVal.Prepend(length - 1 - i);
				current = current->prev;	//You move backwards because of prepend.
			}
			return ArrayList<unsigned long long int>(returnVal);
		}

	protected:
		virtual Linker * seekIndex(unsigned long long int index){
			Linker * current = foot;
			if(index < length / 2){
				for(unsigned long long int i = 1ULL; i < index; ++i)
					current = current->next;
			}else{
				for(unsigned long long int i = 0ULL; i > length - index; --i)
					current = current->prev;
			}
			return current;
		}

	};
}

#endif