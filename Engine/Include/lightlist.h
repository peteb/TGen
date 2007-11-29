/*
 *  lightlist.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_LIGHTLIST_H
#define _TGEN_ENGINE_LIGHTLIST_H

#include <tgen_graphics.h>

namespace TGen {
	namespace Engine {
		class Light;
		
		class LightList {
		public:
			LightList(int num);
			
			void addLight(TGen::Engine::Light * light);
			TGen::Engine::Light * getLight(int id) const;
			int getNumLights() const;
			void clear();
			
		private:
			std::vector<TGen::Engine::Light *> lights;
		};
		
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_LIGHTLIST_H
