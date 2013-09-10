#ifndef _CFUNCION_BINARIA_H_
	#define _CFUNCION_BINARIA_H_
	
	#include "Function.h"

	class BinaryFunction : public Function
	{
		public:
			BinaryFunction(Function &operator1, Function &operator2): op1(operator1), op2(operator2)
			{
				//this->setOperator1(operator1);
				//this->setOperator2(operator2);
			}
			
			const Function &getOperator1() const
			{
				return op1;
			}
			void setOperator1(const Function &operator1)
			{
				this->op1 = operator1;
			}
			
			const Function &getOperator2() const
			{
				return op2;
			}
			void setOperator2(const Function &operator2)
			{
				this->op2 = operator2;
			}
		protected:
			Function &op1, &op2;
	};

#endif
