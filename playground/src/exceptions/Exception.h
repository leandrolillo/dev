#ifndef _EXCEPTION_H_
	#define _EXCEPTION_H_

#include "core/javaLike.h"
#include<string>

	class Exception
	{
		protected:
			String msg;

		public:
			Exception() 
			{
				this->msg = "Unhandled Exception";
			}
			Exception(std::string msg) {
				this->setMessage(msg);
			}
			std::string toString() {
				return this->msg;
			}

			void setMessage(std::string msg) 
			{
				this->msg = msg;
			}
	};
#endif
