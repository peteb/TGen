/*
 *  geomrecipe.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "geomrecipe.h"
#include "entityrecipe.h"
#include "entity.h"
#include "componentinterfaces.h"

#include "physics/spheregeom.h"
#include "physics/subsystem.h"
#include "physics/body.h"

TGen::Engine::Physics::GeomRecipe::GeomRecipe(GeomRecipeType type, const std::string & name, dSpaceID space, TGen::Engine::Physics::Subsystem & subsystem)
	: TGen::Engine::ComponentRecipe(name)
	, type(type)
	, componentLinkNum(-1)
	, space(space)
	, subsystem(subsystem)
{
}


TGen::Engine::Component * TGen::Engine::Physics::GeomRecipe::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	TGen::Engine::Physics::Geom * ret = NULL;
	
	if (type == SphereGeomType) {
		TGen::Engine::Physics::SphereGeom * newGeom = new TGen::Engine::Physics::SphereGeom("physGeom", scalarValue1, space);
	
		ret = newGeom;
	}
	else if (type == CappedCylinderGeomType) {
		dGeomID newGeom = dCreateCCylinder(space, scalarValue1, scalarValue2);
		
		ret = new TGen::Engine::Physics::Geom("physGeom");
		ret->setGeomId(newGeom);
		
	}
	
	ret->setFriction(10.0);
	subsystem.addGeom(ret);
	
	return ret;
}

void TGen::Engine::Physics::GeomRecipe::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::EntityRecipe & prototype) {
	componentLinkNum = prototype.getComponentIndex(linkName);
}

void TGen::Engine::Physics::GeomRecipe::fastLinkConstructed(TGen::Engine::Component & constructed, TGen::Engine::Entity & entity) {
	TGen::Engine::Physics::Geom & geom = dynamic_cast<TGen::Engine::Physics::Geom &>(constructed);
	TGen::Engine::Physics::Body * body = dynamic_cast<TGen::Engine::Physics::Body *>(entity.getComponent(componentLinkNum));
	
	if (body) {
		geom.setBody(body);
	}
	else {
		TGen::Engine::WorldObject * object = dynamic_cast<TGen::Engine::WorldObject *>(entity.getComponent(componentLinkNum));
		
		if (object)
			geom.setLinkedComponent(object);
		else
			throw TGen::RuntimeException("Physics::GeomRecipe::linkGlobally", "failed to link id ") << componentLinkNum;
	}
}

void TGen::Engine::Physics::GeomRecipe::setScalarValue1(scalar value) {
	scalarValue1 = value;
}

void TGen::Engine::Physics::GeomRecipe::setScalarValue2(scalar value) {
	scalarValue2 = value;
}

void TGen::Engine::Physics::GeomRecipe::setLink(const std::string & link) {
	linkName = link;
}
