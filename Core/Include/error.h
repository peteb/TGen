/*
 *  error.h
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_CORE_ERROR_H
#define _TGEN_CORE_ERROR_H

#include <string>
#include <exception>
#include <sstream>



namespace TGen {
	class RuntimeException : public std::exception {
	public:
		RuntimeException(const std::string & where, const std::string & description = "");
		virtual ~RuntimeException() throw();
		
		virtual const char* what() const throw();
		
		std::string getWhere() const;
		std::string getDescription() const;
		
		template<typename T>
		TGen::RuntimeException & operator << (const T & value) {
			std::stringstream ss;
			ss << value;
			description += ss.str();
			return *this;
		}
		
	private:
		mutable std::string temp;
		std::string where, description;
	};
	
	class NotImplemented : public TGen::RuntimeException {
	public:
		NotImplemented(const std::string & where, const std::string & description = "") : TGen::RuntimeException(where, description) {}
		
	};
	
} // !TGen


#endif // !_TGEN_CORE_ERROR_H

