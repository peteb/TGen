/*
 *  light.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_LIGHT_H
#define _TGEN_ENGINE_LIGHT_H

#include <tgen_math.h>
#include <tgen_graphics.h>
#include <tgen_renderer.h>

namespace TGen {
	namespace Engine {
		enum LightType {
			LightDirectional = 0,
			LightPositional = 1,
			LightSpotlight = 2,
		};
		
		class Light : public TGen::SceneNode {
		public:
			Light(const std::string & name, const TGen::Vector3 & position, const TGen::Rotation & orientation = TGen::Rotation::Identity);
			
			// light type.... inte material :(    ändra! gör som alla scenenodes, en materialName som länkas..... 
			// ska vara en data-component på entity?
			
			void linkMaterial(TGen::MaterialSource & linker);
			int getType() const;
			
			void setMaterialName(const std::string & material);
			TGen::Light & getLightProperties();
			TGen::Material * getMaterial();
			bool fillUser(TGen::RenderList & list, const TGen::Camera & camera) const;
			bool fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const;
			
		private:
			std::string materialName;
			TGen::Material * material;
			
			int type;			
			bool clipBoundingBox;
			TGen::AABB boundingBox;
			mutable TGen::Light light;
		};
		
		// map har flera SceneLights. (children) om en SceneLight frågas efter addUser så lägger den till sig själv som user
		// map tar lights från synliga areas och lägger till, lika bra att lägga till hela scenenoden som user
		
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_LIGHT_H
