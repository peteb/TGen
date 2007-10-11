/*
 *  textoutput.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_TEXTOUTPUT_H
#define _TGEN_ENGINE_TEXTOUTPUT_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class TextOutputer {
		public:
			virtual ~TextOutputer() {}
			
			virtual void outputText(const std::string & text, uint user) abstract;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TEXTOUTPUT_H
