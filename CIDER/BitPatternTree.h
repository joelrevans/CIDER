#ifndef	BITPATTERNTREE_H
#define	BITPATTERNTREE_H

#include "BitSequence.h"
#include "MutableList.h"
#include "ArrayList.h"
#include "SinglyLinkedList.h"
#include <limits.h>

namespace Juice{

	class BitPatternTree{
	private:

		class Branch{
			public:
				Branch * branch0;
				Branch * branch1;
				ArrayList<unsigned long long int> index;	//The locations at which the current branch key exists.
				
				Branch *& operator[](bool select){
					return select ? branch1 : branch0;
				}
		};

		/*The source sequence.*/
		BitSequence source;
		Branch * root;

	public:

		BitPatternTree(BitSequence& src){
			source = src;
			root = new Branch();
			root->index.Length(source.Length());

			for(unsigned long long int i = 0ULL; i < source.Length(); ++i)	//Populates the root (with all possibilities for empty search terms)
				root->index[i] = i;

			SinglyLinkedList<Branch*> branches;
			SinglyLinkedList<Branch*> buds;

			buds+=root;

			unsigned long long int depth = 0ULL;
 			while(buds.Length()){
				branches = buds;
				buds = SinglyLinkedList<Branch*>();
				while(branches.Length()){
					Branch& current = *(--branches);				//The branch currently being operated on
					
					current.branch0 = new Branch();					//Create two new buds.
					current.branch1 = new Branch();

					ArrayList<unsigned long long int>& index0 = current.branch0->index;
					ArrayList<unsigned long long int>& index1 = current.branch1->index;
					index0.Length(current.index.Length());
					index1.Length(current.index.Length());
					unsigned long long int i0 = 0ULL;	//counter for index0
					unsigned long long int i1 = 0ULL;	//counter for index1

					unsigned long long int lastIndex = current.index.Length()-1;	//opvar
					for(unsigned long long int j = 0ULL; j < lastIndex; ++j){		//Splits all of the index locations from the current branch and feeds it into the repspective buds.
						if(source.Read(current.index[j]+depth))
							index1[i1++] = current.index[j];
						else
							index0[i0++] = current.index[j];
					}
					
					/*	The indicies are one bit longer than the parent branches indicies.  The last item will overflow if the last parent item index ends at the end of the src.*/
					if(current.index[lastIndex]+depth < source.Length()){											
						if(source.Read(current.index[lastIndex]+depth))
							index1[i1++] = current.index[lastIndex];
						else
							index0[i0++] = current.index[lastIndex];
					}

					index0.Length(i0);
					index1.Length(i1);

					//Add each respective branch to the processing queue if it contains more than one index (can be broken into further sub-branches).
					if(i0>1)
						buds.Prepend(current.branch0);
					if(i1>1)
						buds.Prepend(current.branch1);
				}
				++depth;
			}
		}
		
		ArrayList<unsigned long long int> Search(BitSequence key){
			Branch * current = root;

			for(unsigned long long int i = 0ULL; i < key.Length(); ++i){
				if(current->index.Length()<2){								//Branches end when only one or zero buds occur.
					if(current->index.Length()){							//If current.index==1
						if(source.Read(current->index[0], key.Length())==key)
							break;											//Break and return this list
						else
							return ArrayList<unsigned long long int>(0);	//Empty list
					}else													//If current.index==0
						return ArrayList<unsigned long long int>(0);		//Empty list
				}
				current = (*current)[key.Read(i)];
			}
			return current->index;
		}
	};
}

#endif