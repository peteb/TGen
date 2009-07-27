/*
 *  q3map.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_Q3BSP_Q3MAP_H
#define _TGEN_ENGINE_Q3BSP_Q3MAP_H

#include <tgen_renderer.h>

namespace TGen {
	namespace Engine {
		class Q3MapModel;
		
		class Q3Map : public TGen::SceneNode {
		public:	
			Q3Map(const std::string & name);
			
			bool fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const;
			void addModel(TGen::Engine::Q3MapModel * model);
			void linkMaterial(TGen::MaterialSource & source);
			void instantiate(TGen::VertexDataSource & source);
			
		private:
			std::vector<TGen::Engine::Q3MapModel *> models;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_Q3MAP_H

