/*
 *  q3shaderconverter.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/10/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_Q3SHADERCONVERTER_H
#define _TGEN_ENGINE_Q3SHADERCONVERTER_H

#include <string>
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class Q3ShaderConverter {
		public:
			std::string convert(const std::string & data);
			
		private:
			std::string parseGlobalBlock();
			std::string parseMaterial(const std::string & name);
			std::string parsePass();
			std::string parseFunction();
			
			static std::string ConvertBlendComponent(const std::string & comp);
			
			void stepToken();
			
			TGen::TokenStream tokens;
			TGen::TokenStream::TokenList::iterator currentToken, endIter;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_Q3SHADERCONVERTER_H

