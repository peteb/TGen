/*
 *  interpolatorscript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/16/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_UTILITIES_INTERPOLATORSCRIPT_H
#define _TGEN_ENGINE_UTILITIES_INTERPOLATORSCRIPT_H

#include <string>
#include "script/componentscript.h"

struct lua_State;

namespace TGen {
	namespace Engine {
		namespace Utilities {
			class Interpolator;
			
			class InterpolatorScript : public TGen::Engine::Script::ComponentScript {
			public:
				InterpolatorScript(const std::string & name, Interpolator & interpolator, TGen::Engine::Script::EntityScript * entityScript);
				
			private:
				static int luaSetSpeed(lua_State * vm);
				
				Interpolator & interpolator;
				TGen::Engine::Script::EntityScript * scriptEntity;
			};
			
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_INTERPOLATORSCRIPT_H

