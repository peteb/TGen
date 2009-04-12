/*
 *  scriptppc.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/10/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_SCRIPTPPC_H
#define _TGEN_ENGINE_SCRIPT_SCRIPTPPC_H

#include <string>
#include <vector>

namespace TGen {
	namespace Engine {
		namespace Script {
			class ScriptPreprocessor {
			public:
				std::string process(const std::string & input);
				
			private:
				std::string::size_type getFinalBracket(const std::string & input, std::string::size_type start);
				std::string parseBlock(const std::string & block);
				std::string convertBlock(const std::vector<std::string> & tokens);
				std::string getValueConverted(const std::string & value);
				bool goodBlock(const std::string & block);
				
			};
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPTPPC_H

