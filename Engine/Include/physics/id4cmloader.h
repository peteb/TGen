/*
 *  id4cmloader.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ID4CMLOADER_H
#define _TGEN_ENGINE_ID4CMLOADER_H

#include <string>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class Id4CMGeom;
			
			class Id4CMLoader {
			public:
				TGen::Engine::Physics::Id4CMGeom * createGeom(const std::string & path);
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ID4CMLOADER_H

