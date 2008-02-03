/*
 *  deferredrenderervars.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_DEFERREDRENDERERVARS_H
#define _TGEN_ENGINE_DEFERREDRENDERERVARS_H

#include "variable.h"

namespace TGen {
	namespace Engine {
		class App;
		
		// TODO: fs_log, dir för logs. skriv loggar där!!!!!! omg
		
		class DeferredRendererVars : public TGen::Engine::VariableObserver {
		public:
			DeferredRendererVars(TGen::Engine::App & app);
			
			void postVariableChange(const TGen::Engine::Variable & variable);
			void onVariableRemoved(const TGen::Engine::Variable & variable);
			
			void loadVariables();
			
			bool forceBinaryMRT;
			bool postBloom, postProcessing, lumTrace;
			int bloomBlurPasses, bloomDownsampling;
			float lumMin, lumMultiplier;
			
		private:
			TGen::Engine::App & app;			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_DEFERREDRENDERERVARS_H
