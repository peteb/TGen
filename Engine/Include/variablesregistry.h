/*
 *  variablesregistry.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_VARIABLESREGISTRY_H
#define _TGEN_ENGINE_VARIABLESREGISTRY_H

#include "variable.h"

namespace TGen {
	namespace Engine {
		class Filesystem;
		
		enum AddVariableFlags {
			AddNoThrow			= 0x0001,
			AddOverrideDefaults	= 0x0002,
			AddOverrideValue	= 0x0004,
		};
		
		class VariablesRegistry {
		public:
			VariablesRegistry();
			~VariablesRegistry();
			
			const TGen::Engine::Variable & getVariable(const std::string & name) const;
			TGen::Engine::Variable & getVariable(const std::string & name);
			TGen::Engine::VariablesRegistry & addVariable(const TGen::Engine::Variable & variable, uint flags = 0);

			const TGen::Engine::Variable & operator [] (const std::string & name) const;
			TGen::Engine::Variable & operator [] (const std::string & name);
			TGen::Engine::VariablesRegistry & operator += (const TGen::Engine::Variable & var);
			
			void getVariables(std::vector<std::string> & vars);
			void dumpVariables(TGen::Engine::Filesystem & fs);
			
		private:
			typedef std::map<std::string, TGen::Engine::Variable> VariableMap;
			VariableMap variables;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VARIABLESREGISTRY_H

