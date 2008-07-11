/*
 *  geomrecipe.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_GEOMRECIPE_H
#define _TGEN_ENGINE_GEOMRECIPE_H

#include "componentrecipe.h"
#include <ODE/ODE.h>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class Subsystem;
			
			enum GeomRecipeType {
				SphereGeomType,
				CappedCylinderGeomType,
				BoxGeomType,
				RayGeomType,
			};
			
			class GeomRecipe : public TGen::Engine::ComponentRecipe {
			public:
				GeomRecipe(GeomRecipeType type, const std::string & name, dSpaceID space, TGen::Engine::Physics::Subsystem & subsystem);
				
				void fastLinkConstructed(TGen::Engine::Component & constructed, TGen::Engine::Entity & entity);
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::EntityRecipe & entity);

				TGen::Engine::Component * createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing);
				
				void setScalarValue1(scalar value);
				void setScalarValue2(scalar value);
				void setScalarValue3(scalar value);
				
				void setFriction(scalar friction);
				void setLink(const std::string & link);
				void setCategory(uint category);
				void setCollidesWith(uint collidesWith);
				
			private:
				TGen::Engine::Physics::Subsystem & subsystem;
				
				scalar scalarValue1, scalarValue2, scalarValue3;
				scalar friction;
				
				std::string linkName;
				int componentLinkNum;
				dSpaceID space;
				GeomRecipeType type;
				uint categoryBits, collidesWith;
				
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_GEOMRECIPE_H

