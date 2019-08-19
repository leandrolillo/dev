#ifndef _CFUNCION_MULTIPLICACION_
	#define _CFUNCION_MULTIPLICACION_
	
	#include "BinaryFunction.h"

	class FunctionMultiplication : public BinaryFunction
	{
		public: 
			FunctionMultiplication(Function &op1, Function &op2) : BinaryFunction(op1, op2)
			{
			
			}
		
			virtual real evaluate(real x) const
			{
				return this->op1.evaluate(x) * this->op2.evaluate(x);
			}
			virtual std::string toString() const
			{
				std::string resultado;
				
				resultado = op1.toString();
				resultado += " * ";
				resultado += op2.toString();
				
				return resultado;
			}
	};
#endif
