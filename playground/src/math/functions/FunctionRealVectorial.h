/*
 * FunctionRealVectorial.h
 *
 *  Created on: 05/10/2013
 *      Author: Belén Cabeza
 */

#ifndef FUNCTIONREALVECTORIAL_H_
#define FUNCTIONREALVECTORIAL_H_

#include "Function.h"

class FunctionRealVectorial {
public:
	virtual vectorN evaluate(real t) const = 0;

	virtual vectorN operator ()(real t) const
	{
		return this->evaluate(t);
	}

	virtual String toString() const = 0;

	virtual ~FunctionRealVectorial() {}
};


#endif /* FUNCTIONREALVECTORIAL_H_ */
