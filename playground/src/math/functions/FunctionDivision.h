#ifndef _CFUNCION_DIVISION_
	#define _CFUNCION_DIVISION_
	
	#include "BinaryFunction.h"
	#include "exceptions/InvalidArgumentException.h"
	

	class FunctionDivision : public BinaryFunction
	{
		public: 
			FunctionDivision(Function &op1, Function &op2) : BinaryFunction(op1, op2)
			{
			
			}
			virtual real evaluate(real x) const
			{
				real denominador = this->op2.evaluate(x);
				
				if(!equalsZero(denominador))
					return this->op1.evaluate(x) / denominador;
				else throw InvalidArgumentException("Division by Zero! - FunctionDivision::evaluate(...)");
			}
			
			virtual std::string toString() const
			{
				std::string resultado;
				
				resultado = op1.toString();
				resultado += " / ";
				resultado += op2.toString();
				
				return resultado;
			}
	};
#endif
