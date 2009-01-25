/*
 *  renderervars.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/25/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_FORWARD_RENDERERVARS_H
#define _TGEN_ENGINE_FORWARD_RENDERERVARS_H

namespace TGen {
	namespace Engine {
		class VariableRegister;
		
		struct ForwardRendererSettings {
			bool optShadowScissor;			
		};
		
		class ForwardRendererVars {
		public:
			ForwardRendererVars(TGen::Engine::VariableRegister & variables);
			
			const ForwardRendererSettings & getSettings() const;
		
		private:
			void loadSettings();
			
			TGen::Engine::VariableRegister & variables;			
			ForwardRendererSettings settings;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FORWARD_RENDERERVARS_H

