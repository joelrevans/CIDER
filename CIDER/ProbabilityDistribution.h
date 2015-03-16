#ifndef PROBABILITYDISTRIBUTION_H
#define PROBABILITYDISTRIBUTION_H

#include "BitSequence.h"

namespace CIDER{

	class ProbabilityDistribution{

	private:
		unsigned long long int length;
		unsigned long long int domain;
		unsigned long long int range;
		BitSequence data;

	public:
		ProbabilityDistribution(unsigned long long int Domain, unsigned long long int Range){
			domain = Domain;
			range = Range;
			data = BitSequence(domain*range);
		}

		/*Adds an element to the distribution.*/
		void AddElement(unsigned long long int src){
			/*
			BitSequence snippet = data.Read(src*range, );
			BitSequence one = BitSequence::Create<char>(1, 1);
			BitSequence result = snippet.BitwiseAdd(snippet, one);
			result.Length(snippet.Length());
			data.Write(src*range, result);
			*/
		}

	};
}

#endif