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
		
		enum RendererUserType {
			UserTypeLight = 1,
		};
		
		class LightList {
		public:
			LightList(int num);
			~LightList();
			
			typedef std::vector<TGen::Engine::Light *> LightArray;
			typedef std::map<TGen::Material *, LightArray *> LightMap;
			
			void addLight(TGen::Engine::Light * light);
			TGen::Engine::Light * getLight(int id) const;
			LightMap & getLightsByMaterial();
			
			int getNumLights() const;
			void clear();
			
		private:
			
			LightArray lights;
			LightMap lightsByMaterials;
		};
		
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_LIGHTLIST_H
