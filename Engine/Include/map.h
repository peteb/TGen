/*
 *  map.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/29/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MAP_H
#define _TGEN_ENGINE_MAP_H

#include <tgen_renderer.h>

namespace TGen {
	namespace Engine {
		class Map : public TGen::SceneNode {
		public:
			Map(const std::string & name, const TGen::Vector3 & origin);
			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAP_H
