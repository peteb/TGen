/*
 *  generateline.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_GENERATELINE_H
#define _TGEN_ENGINE_GENERATELINE_H

#include <string>
#include <map>
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class GenerateLine {
		public:
			GenerateLine(const std::string & text);
			
			typedef std::map<std::string, std::string> ParameterMap;
			
			const std::string & getName() const;
			std::string getParameter(const std::string & name) const;
			std::string getParameter(const std::string & name, const std::string & def) const;
			
			const ParameterMap & getParameters() const;
			
		private:
			void parse(const std::string & text);

			class Tokenizer : public TGen::Tokenizer {
			public:
				Tokenizer();
			};
			
			std::string name;
			std::string lastValue;
			ParameterMap parameters;
		};
		
		
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_GENERATELINE_H
