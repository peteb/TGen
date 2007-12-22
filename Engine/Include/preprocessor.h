/*
 *  textpreprocessor.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_TEXTPREPROCESSOR_H
#define _TGEN_ENGINE_TEXTPREPROCESSOR_H

#include <string>
#include <vector>

namespace TGen {
	namespace Engine {
		class TextPreprocessor {
		public:
			TextPreprocessor();
			
			std::string process(const std::string & contents, const std::string & parameters);
			
		private:
			typedef std::vector<std::pair<std::string, std::string> > ParameterList;

			void parseParameters(const std::string & parameters, ParameterList & out);
			
			bool defineNoValueParam;
		};

	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TEXTPREPROCESSOR_H
