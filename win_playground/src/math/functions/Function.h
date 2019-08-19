/*
 * Function.h
 *
 *  Created on: 09/09/2013
 *      Author: Lean
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

	#include<vector>
	#include "math/MathConstants.h"


	class FunctionMultiplication;
	class FunctionDivision;
	class FunctionAdd;
	class FunctionSubstract;

	class Function
	{
		public:
			virtual real evaluate(real x) const = 0;

			virtual real operator ()(real x) const
			{
				return this->evaluate(x);
			}
			virtual std::string toString() const = 0;

			FunctionMultiplication operator *(Function &operator1);
			FunctionDivision operator /(Function &operator1);
			FunctionAdd operator +(Function &operator1);
			FunctionSubstract operator -(Function &operator1);

			virtual ~Function() {}
	};

#endif /* FUNCTION_H_ */
