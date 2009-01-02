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
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class IncludeCallback {
		public:
			virtual ~IncludeCallback() {}
			
			virtual std::string getIncludeContent(const std::string & identifier) abstract;
		};
		
		class TextPreprocessor {
		public:
			TextPreprocessor();
			
			std::string process(const std::string & contents, const std::string & parameters, bool parseIfs = false, bool hashDefs = true);
			
			std::string processIncludes(const std::string & contents, IncludeCallback & callback);
			
		private:
			typedef std::vector<std::pair<std::string, std::string> > ParameterList;

			void parseParameters(const std::string & parameters, ParameterList & out, bool hashDefs);
			std::string fixLoops(const std::string & input) const;
			std::string fixIfs(const std::string & input) const;
			
			bool defineNoValueParam;
		};

	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TEXTPREPROCESSOR_H
