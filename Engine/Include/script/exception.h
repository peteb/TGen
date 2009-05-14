/*
 *  exception.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/30/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_EXCEPTION_H
#define _TGEN_ENGINE_SCRIPT_EXCEPTION_H


namespace TGen {
	namespace Engine {
		class ScriptException : public TGen::RuntimeException {
		public:
			ScriptException(const std::string & description)
				: TGen::RuntimeException("", description) {}
			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_EXCEPTION_H

