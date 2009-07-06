/*
 *  interpolatorscript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/16/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_CONTROLLER_INTERPOLATORSCRIPT_H
#define _TGEN_ENGINE_CONTROLLER_INTERPOLATORSCRIPT_H

#include <string>
#include "script/componentscript.h"

struct lua_State;

namespace TGen {
	namespace Engine {
		namespace Controller {
			class Interpolator;
			
			class InterpolatorScript : public TGen::Engine::Script::ComponentScript {
			public:
				InterpolatorScript(const std::string & name, Interpolator & interpolator, TGen::Engine::Script::EntityScript * entityScript);
				
				void onReachedEnd();
				void setEnabled(bool enabled);
				bool getEnabled() const;
				
			private:
				static int luaSetSpeed(lua_State * vm);
				
				Interpolator & interpolator;
				TGen::Engine::Script::EntityScript * scriptEntity;
			};
			
		} // !Controller
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CONTROLLER_INTERPOLATORSCRIPT_H

