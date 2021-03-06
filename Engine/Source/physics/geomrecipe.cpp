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

#include "physics/subsystem.h"
#include "physics/body.h"
#include "physics/ccylindergeom.h"
#include "physics/spheregeom.h"
#include "physics/boxgeom.h"
#include "physics/raygeom.h"
#include "physics/geomscript.h"

TGen::Engine::Physics::GeomRecipe::GeomRecipe(GeomRecipeType type, const std::string & name, dSpaceID space, TGen::Engine::Physics::Subsystem & subsystem)
	: TGen::Engine::ComponentRecipe(name)
	, type(type)
	, componentLinkNum(-1)
	, space(space)
	, subsystem(subsystem)
	, friction(1.0)
	, categoryBits(0)
	, collidesWith(0)
{
}


TGen::Engine::Component * TGen::Engine::Physics::GeomRecipe::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	TGen::Engine::Physics::Geom * ret = NULL;
	std::string name = "physGeom";

	if (type == SphereGeomType) {
		TGen::Engine::Physics::SphereGeom * newGeom = new TGen::Engine::Physics::SphereGeom(name, scalarValue1, space);
	
		ret = newGeom;
	}
	else if (type == CappedCylinderGeomType) {
		TGen::Engine::Physics::CCylinderGeom * newGeom = new TGen::Engine::Physics::CCylinderGeom(name, scalarValue1, scalarValue2, space);

		ret = newGeom;
	}
	else if (type == BoxGeomType) {
		TGen::Engine::Physics::BoxGeom * newGeom = new TGen::Engine::Physics::BoxGeom(name, TGen::Vector3(scalarValue1, scalarValue2, scalarValue3), space);
		
		ret = newGeom;
	}
	else if (type == RayGeomType) {
		TGen::Engine::Physics::RayGeom * newGeom = new TGen::Engine::Physics::RayGeom(name, scalarValue1, space);
		
		ret = newGeom;
	}
	else {
		throw TGen::RuntimeException("Physics::GeomRecipe::createComponent", "invalid geom type");
	}
	
	ret->setFriction(friction);
	ret->setCategory(categoryBits);
	ret->setCollidesWith(collidesWith);
	ret->setScriptInterface(new TGen::Engine::Physics::GeomScript(name, ret, constructing.getScriptInterface()));		// TODO: check that it works
									
	subsystem.addGeom(ret);

	return ret;
}

void TGen::Engine::Physics::GeomRecipe::link(const TGen::Engine::ComponentLinker & linker) {
	TGen::Engine::Physics::Geom * geom = dynamic_cast<TGen::Engine::Physics::Geom *>(linker.getComponent());		// created component
	TGen::Engine::Physics::Body * body = dynamic_cast<TGen::Engine::Physics::Body *>(linker.getComponent(componentLinkNum));
	
	if (body) {
		geom->setBody(body);
	}
	else {
		TGen::Engine::WorldObject * object = dynamic_cast<TGen::Engine::WorldObject *>(linker.getComponent(componentLinkNum));
		
		geom->setLink(object);
	}	
}

// TODO: link ska heta samma överallt i stort sett, linkName heter membern, void setLink(string), setLink(object)

void TGen::Engine::Physics::GeomRecipe::prelink(const TGen::Engine::ComponentLinker & linker) {
	if (linker.getEntityRecipe())
		componentLinkNum = linker.getEntityRecipe()->getComponentIndex(linkName);
}

void TGen::Engine::Physics::GeomRecipe::setScalarValue1(scalar value) {
	scalarValue1 = value;
}

void TGen::Engine::Physics::GeomRecipe::setScalarValue2(scalar value) {
	scalarValue2 = value;
}

void TGen::Engine::Physics::GeomRecipe::setScalarValue3(scalar value) {
	scalarValue3 = value;
}

void TGen::Engine::Physics::GeomRecipe::setLink(const std::string & link) {
	linkName = link;
}

void TGen::Engine::Physics::GeomRecipe::setFriction(scalar friction) {
	this->friction = friction;
}

void TGen::Engine::Physics::GeomRecipe::setCategory(uint category) {
	this->categoryBits = category;
}

void TGen::Engine::Physics::GeomRecipe::setCollidesWith(uint collidesWith) {
	this->collidesWith = collidesWith;
}

