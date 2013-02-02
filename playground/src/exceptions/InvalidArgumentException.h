#ifndef _INVALID_ARGUMENT_EXCEPTION_
	#define _INVALID_ARGUMENT_EXCEPTION_

	#include "Exception.h"

	class InvalidArgumentException : public Exception
	{
		public:
			InvalidArgumentException(std::string msg)
			{
				std::string message("Invalid ArgumentException");
				message += ": ";
				message += msg;
				
				this->setMessage(message);
			}
			
		private:
	};

#endif