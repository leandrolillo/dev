#ifndef _CFUNCION_RESTA_
	#define _CFUNCION_RESTA_
	
	#include "BinaryFunction.h"

	class FunctionSubstraction : public BinaryFunction
	{
		public: 
			FunctionSubstraction(Function &op1, Function &op2) : BinaryFunction(op1, op2)
			{
			
			}
		
			virtual real Evaluar(real x) const
			{
				return this->op1.evaluate(x) - this->op2.evaluate(x);
			}
			virtual std::string toString() const
			{
				std::string resultado;
				
				resultado = op1.toString();
				resultado += " - ";
				resultado += op2.toString();
				
				return resultado;
			}
	};
#endif
