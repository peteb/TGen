/*
 *  cast.h
 *  TGen Core
 *
 *  Created by Peter Backman on 7/17/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_CAST_H
#define _TGEN_CAST_H

#include "stringman.h"
#include <iostream>
#include <iomanip>

namespace TGen {
	class bad_lexical_cast : public std::bad_cast {
	public:
		//bad_lexical_cast(const char * message = "bad lexical cast") : std::bad_cast(message) {}
		bad_lexical_cast() throw() {}
		bad_lexical_cast(const bad_lexical_cast & other) : std::bad_cast(other) {}
		virtual ~bad_lexical_cast() throw() {}		
	};
	
	class lexstreamer {
	public:	
		lexstreamer() : isBool(false) {
			//ss << std::boolalpha;
		}
		
		template<typename from>
		lexstreamer & operator << (const from & _from) {
			ss << _from;
				  
			return *this;
		}
		
		lexstreamer & operator << (const bool & _from) {
			ss << _from;
			isBool = true;

			return *this;
		}
		
		template<typename to>
		lexstreamer & operator >> (to & _to) {
			ss >> _to;
				  
			return *this;
		}
		
		lexstreamer & operator >> (std::string & _to) {
			if (isBool) {
				if (ss.str() == "1")
					_to = "true";
				else
					_to = "false";
			}
			else {
				ss >> _to;				
			}
			
			return *this;
		}
		
		lexstreamer & operator >> (bool & _to) {
			std::string lower = TGen::toLower(ss.str());
			if (lower == "true" || lower == "yes" || lower == "on") {
				_to = true;
				return *this;
			}
			else if (lower == "false" || lower == "no" || lower == "off") {
				_to = false;
				return *this;
			}
			
			ss >> _to;
			
			return *this;
		}
		
		bool operator ! () const {
			return !ss;
		}
		
	private:
		std::stringstream ss;
		bool isBool;
	};
	
	
	template<typename to, typename from>
	to lexical_cast(const from & _from) {
		to ret;
		lexstreamer ls;

		if (!(ls << _from) || !(ls >> ret))
			throw bad_lexical_cast();
		
		return ret;
	}
} // !Tgen

#endif // !_TGEN_CAST_H
