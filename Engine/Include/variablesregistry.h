/*
 *  variablesregistry.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SETTINGSREGISTRY_H
#define _TGEN_ENGINE_SETTINGSREGISTRY_H

#include "variable.h"

namespace TGen {
	namespace Engine {
		class VariablesRegistry {
		public:
			VariablesRegistry();
			~VariablesRegistry();
			
			const TGen::Engine::Variable & getVariable(const std::string & name) const;
			TGen::Engine::Variable & getVariable(const std::string & name);
			void addVariable(const TGen::Engine::Variable & variable);

		private:
			typedef std::map<std::string, TGen::Engine::Variable> VariableMap;
			VariableMap variables;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SETTINGSREGISTRY_H
