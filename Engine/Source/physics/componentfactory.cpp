/*
 *  componentfactory.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/componentfactory.h"
#include "physics/subsystem.h"
#include "physics/body.h"
#include "physics/joint.h"
#include "physics/planegeom.h"
#include "physics/spheregeom.h"
#include "physics/boxgeom.h"
#include "physics/geom.h"
#include "physics/bipedalgeom.h"
#include "physics/meshgeom.h"
#include "physics/id4cmgeom.h"
#include "physics/id4cmloader.h"
#include "physics/id3cmloader.h"
#include "physics/id3cmgeom.h"

#include "physics/geomrecipe.h"
#include "physics/bodyrecipe.h"
#include "physics/geomscript.h"
#include "physics/bodyscript.h"

#include "generateline.h"
#include "transformerfactory.h"

#include <tgen_renderer.h>

using TGen::uint;

TGen::Engine::Physics::ComponentFactory::ComponentFactory(TGen::Engine::Physics::Subsystem & subsystem, TGen::Engine::Filesystem & filesystem)
	: subsystem(subsystem)
	, filesystem(filesystem)
{
}


TGen::Engine::ComponentRecipe * TGen::Engine::Physics::ComponentFactory::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	TGen::Engine::ComponentRecipe * ret = NULL;
	
	dSpaceID mainSpace = subsystem.getSpaceId();
	
	if (properties.getName() == "physGeom") {
		std::string type = properties.getProperty("type", "unknown");
		
		TGen::Engine::Physics::GeomRecipe * newRecipe = NULL; 
		
		if (type == "sphere") {
			newRecipe = new TGen::Engine::Physics::GeomRecipe(TGen::Engine::Physics::SphereGeomType, name, mainSpace, subsystem);
			newRecipe->setScalarValue1(TGen::lexical_cast<scalar>(properties.getProperty("radius", "1.0")));
		}
		else if (type == "ccylinder") {
			newRecipe = new TGen::Engine::Physics::GeomRecipe(TGen::Engine::Physics::CappedCylinderGeomType, name, mainSpace, subsystem);
			newRecipe->setScalarValue1(TGen::lexical_cast<scalar>(properties.getProperty("radius", "1.0")));
			newRecipe->setScalarValue2(TGen::lexical_cast<scalar>(properties.getProperty("length", "1.0")));
		}
		else if (type == "box") {
			newRecipe = new TGen::Engine::Physics::GeomRecipe(TGen::Engine::Physics::BoxGeomType, name, mainSpace, subsystem);
			TGen::Vector3 dimensions = TGen::Vector3::Parse(properties.getProperty("dimensions", "1 1 1"));
			
			newRecipe->setScalarValue1(dimensions.x);
			newRecipe->setScalarValue2(dimensions.y);
			newRecipe->setScalarValue3(dimensions.z);
		}
		else if (type == "ray") {
			newRecipe = new TGen::Engine::Physics::GeomRecipe(TGen::Engine::Physics::RayGeomType, name, mainSpace, subsystem);
			newRecipe->setScalarValue1(TGen::lexical_cast<scalar>(properties.getProperty("length", "1.0")));
			
		}
		else {
			throw TGen::RuntimeException("Physics::ComponentFactory::createComponentRecipe", "invalid geom type: " + type);
		}
		
		newRecipe->setLink(properties.getProperty("link", "sceneNode"));
		newRecipe->setFriction(TGen::lexical_cast<scalar>(properties.getProperty("friction", "10")));
		uint collideWith = ~getCategoryBits(properties.getProperty("noCollide", ""));
		
		newRecipe->setCategory(getCategoryBits(properties.getProperty("category", "default")));
		newRecipe->setCollidesWith(collideWith);
		
		ret = newRecipe;
	}
	else if (properties.getName() == "physBody") {
		TGen::Engine::Physics::BodyRecipe * newRecipe = new TGen::Engine::Physics::BodyRecipe(name, mainSpace, getMass(properties), subsystem);
		
		newRecipe->setLink(properties.getProperty("link", ""));
		newRecipe->setMaxAngularSpeed(TGen::lexical_cast<scalar>(properties.getProperty("maxAngularSpeed", "-1.0")));
		newRecipe->setKillTorque(TGen::lexical_cast<bool>(properties.getProperty("killTorque", "false")));
		newRecipe->setLinearDamping(TGen::lexical_cast<scalar>(properties.getProperty("linearDamping", "0.0")));
		newRecipe->setFakeGravity(TGen::lexical_cast<scalar>(properties.getProperty("fakeGrav", "-2.0")));
		newRecipe->setAutoDisable(TGen::lexical_cast<bool>(properties.getProperty("autoDisable", "true")));
		newRecipe->setApplyGravity(TGen::lexical_cast<bool>(properties.getProperty("gravity", "true")));
		
		
		ret = newRecipe;
	}
	else {
		throw TGen::RuntimeException("Physics::ComponentFactory::createComponentRecipe", "invalid component type: " + properties.getName());
	}

	
	return ret;
}



dMass TGen::Engine::Physics::ComponentFactory::getMass(const TGen::PropertyTree & properties) {
	dMass ret;
	std::string massType = properties.getProperty("massType", "unknown");
	
	if (massType == "box") {
		scalar totalMass = TGen::lexical_cast<scalar>(properties.getProperty("massTotal", "1.0"));
		TGen::Vector3 dimensions = TGen::Vector3::Parse(properties.getProperty("massDimensions", "1.0 1.0 1.0"));
		
		dMassSetBox(&ret, totalMass, dimensions.x, dimensions.y, dimensions.z);
	}
	else if (massType == "sphere") {
		scalar totalMass = TGen::lexical_cast<scalar>(properties.getProperty("massTotal", "1.0"));
		scalar radius = TGen::lexical_cast<scalar>(properties.getProperty("massRadius", "1.0"));
		
		dMassSetSphere(&ret, totalMass, radius);
	}
	else {
		throw TGen::RuntimeException("Physics::Subsystem::getMass", "invalid mass type '" + massType + "'");
	}
	
	return ret;
}


uint TGen::Engine::Physics::ComponentFactory::getCategoryBits(const std::string & name) {
	static TGen::SymbolTable symbols;
	
	if (name.empty())
		return 0;
	
	uint ret = 0;
	uint pos = 0;
	
	while (1) {
		int nextPos = name.find(" ", pos);
		std::string fixedName = name;
		
		if (nextPos == std::string::npos)
			fixedName = name.substr(pos);
		else
			fixedName = name.substr(pos, nextPos - pos);
		
		int symbolNumber = symbols[name];
		
		ret |= 1 << symbolNumber;
		
		if (nextPos == std::string::npos)
			break;
		
		pos = nextPos + 1;
	}
	
	return ret;
}


TGen::Engine::Physics::Body * TGen::Engine::Physics::ComponentFactory::createBody(const std::string & name, const TGen::PropertyTree & properties, dWorldID worldId, dSpaceID spaceId, TGen::Engine::Entity & entity) {
	TGen::Vector3 position = TGen::Vector3::Parse(properties.getProperty("position", "0 0 0"));
	
	dBodyID newBodyId = dBodyCreate(worldId);
	
	try {
		dMass mass = getMass(properties);
		
		dBodySetMass(newBodyId, &mass);
	}
	catch (...) {
		// No mass defined
	}
	
	//dBodySetLinearDamping(newBodyId, 0.07);
	
	TGen::Engine::Physics::Body * newBody = new TGen::Engine::Physics::Body(name, newBodyId, worldId, spaceId);
	
	dBodySetData(newBodyId, reinterpret_cast<void *>(newBody));

	newBody->setAutoDisable(TGen::lexical_cast<bool>(properties.getProperty("autoDisable", "true")));
	newBody->setApplyGravity(TGen::lexical_cast<bool>(properties.getProperty("gravity", "true")));
	newBody->setPosition(position);
	newBody->setTurnHeadwise(TGen::lexical_cast<bool>(properties.getProperty("turnHead", "false")));
	newBody->setMaxAngularSpeed(TGen::lexical_cast<scalar>(properties.getProperty("maxAngularSpeed", "-1.0")));
	newBody->setLink(properties.getProperty("link", "sceneNode"));
	newBody->setKillTorque(TGen::lexical_cast<bool>(properties.getProperty("killTorque", "false")));
	newBody->setLinearDamping(TGen::lexical_cast<scalar>(properties.getProperty("linearDamping", "0.0")));
	newBody->setFakeGravity(TGen::lexical_cast<scalar>(properties.getProperty("fakeGrav", "-2.0")));
	newBody->setScriptInterface(new TGen::Engine::Physics::BodyScript(name, newBody, entity.getScriptInterface()));

	return newBody;
}


TGen::Engine::Physics::Geom * TGen::Engine::Physics::ComponentFactory::createGeom(const std::string & name, const TGen::PropertyTree & properties, dSpaceID mainSpace, TGen::Engine::Entity & entity) {
	TGen::auto_ptr<TGen::Engine::Physics::Geom> newGeom;
	std::string geomType = properties.getProperty("type", "none");
	
	if (geomType == "plane") {
		TGen::Vector3 normal = TGen::Vector3::Parse(properties.getProperty("orientation", "0 1 0"));
		scalar distance = TGen::lexical_cast<scalar>(properties.getProperty("distance", "0"));
		
		newGeom = new TGen::Engine::Physics::PlaneGeom(name, TGen::Plane3(normal, distance), mainSpace);
	}
	else if (geomType == "sphere") {
		scalar radius = TGen::lexical_cast<scalar>(properties.getProperty("radius", "1.0"));
		
		newGeom = new TGen::Engine::Physics::SphereGeom(name, radius, mainSpace);
	}
	else if (geomType == "box") {
		TGen::Vector3 dimensions = TGen::Vector3::Parse(properties.getProperty("dimensions", "1 1 1"));
		
		newGeom = new TGen::Engine::Physics::BoxGeom(name, dimensions, mainSpace);
	}
	else if (geomType == "bipedal") {
		newGeom = new TGen::Engine::Physics::BipedalGeom(name, mainSpace, 
																			  TGen::lexical_cast<scalar>(properties.getProperty("capRadius", "1.0")), 
																			  TGen::lexical_cast<scalar>(properties.getProperty("length", "1.0")));
	}
	else if (geomType == "mesh") {
		TGen::PropertyTree props(properties);
		
		if (!props.hasNode("normals"))
			props.addNode(TGen::PropertyTree("normals"));
		
		newGeom = new TGen::Engine::Physics::MeshGeom(name, mainSpace, props.getNode("vertices"), 
																		  props.getNode("indices"), 
																		  props.getNode("normals"));
	}
	else if (geomType == "id4cm") {
		TGen::Engine::Physics::Id4CMLoader loader(filesystem);
		TGen::Engine::GenerateLine line("gen:" + properties.getProperty("model", ""));
		TGen::Engine::TransformerFactory transFactory;
		
		TGen::VertexTransformList transformers;
		transformers.addTransformer(transFactory.createTransformers(line));
		
		newGeom = loader.createGeom(name, line.getName(), transformers, mainSpace);
	}
	else if (geomType == "id3cm") {
		// id3cm = multicomponent, component with children.
		newGeom = new TGen::Engine::Physics::Geom(name);
		
	/*	TGen::Engine::Physics::Id3CMLoader loader(filesystem);
		TGen::Engine::GenerateLine line("gen:" + properties.getProperty("model", ""));
		TGen::Engine::TransformerFactory transFactory;
		
		TGen::VertexTransformList transformers;
		transformers.addTransformer(transFactory.createTransformers(line));
		
		newGeom.reset(loader.createGeom(name, line.getName(), transformers, mainSpace));
		*/
	}
	else {
		throw TGen::RuntimeException("Physics::Subsystem::createGeom", "invalid geom type '" + geomType + "'!");
	}
	
	if (!newGeom.get())
		throw TGen::RuntimeException("Physics::Subsystem::createGeom", "failed to create geom of type '" + geomType + "'!");
	
	newGeom->setFriction(TGen::lexical_cast<float>(properties.getProperty("friction", "1.0")));
	newGeom->setLink(properties.getProperty("link", ""));
	newGeom->setAffectsOthers(TGen::lexical_cast<bool>(properties.getProperty("affectsOthers", "true")));
	newGeom->setPosition(TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));
	newGeom->setCalculateVelocity(TGen::lexical_cast<bool>(properties.getProperty("calculateVelocity", "false")));
	
	newGeom->setScriptInterface(new TGen::Engine::Physics::GeomScript(name, newGeom.get(), entity.getScriptInterface()));
	newGeom->collisionForceThreshold = TGen::lexical_cast<scalar>(properties.getProperty("collisionForceThreshold", "3.0"));
	newGeom->collisionForceScale = TGen::lexical_cast<scalar>(properties.getProperty("collisionForceScale", "1.0"));
	
	
	uint collideWith = ~getCategoryBits(properties.getProperty("noCollide", ""));
	
	newGeom->setCategory(getCategoryBits(properties.getProperty("category", "default")));
	newGeom->setCollidesWith(collideWith);
	
	
	if (geomType == "id3cm") {	// fill component with childgeometries
		TGen::Engine::Physics::Id3CMLoader loader(filesystem);
		TGen::Engine::GenerateLine line("gen:" + properties.getProperty("model", ""));
		TGen::Engine::TransformerFactory transFactory;
		
		TGen::VertexTransformList transformers;
		transformers.addTransformer(transFactory.createTransformers(line));

		std::vector<TGen::Engine::Physics::Geom *> geomsCreated;
		
		geomsCreated = loader.fillGeoms(name, line.getName(), transformers, mainSpace);
		
		for (int i = 0; i < geomsCreated.size(); ++i) {
			TGen::Engine::Physics::Geom * component = geomsCreated[i];

			newGeom->addSubcomponent(component);

			
			///////////////////////////////////////////////////
			// exact same code as above
			
			component->setFriction(TGen::lexical_cast<float>(properties.getProperty("friction", "1.0")));
			component->setLink(properties.getProperty("link", ""));
			component->setAffectsOthers(TGen::lexical_cast<bool>(properties.getProperty("affectsOthers", "true")));
			component->setPosition(TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));
			component->setCalculateVelocity(TGen::lexical_cast<bool>(properties.getProperty("calculateVelocity", "false")));
			
			component->setScriptInterface(new TGen::Engine::Physics::GeomScript(component->getName(), component, entity.getScriptInterface()));
			
			component->collisionForceThreshold = TGen::lexical_cast<scalar>(properties.getProperty("collisionForceThreshold", "3.0"));
			component->collisionForceScale = TGen::lexical_cast<scalar>(properties.getProperty("collisionForceScale", "1.0"));
			
			
			uint collideWith = ~getCategoryBits(properties.getProperty("noCollide", ""));
			
			component->setCategory(getCategoryBits(properties.getProperty("category", "default")));
			component->setCollidesWith(collideWith);
			
			///////////////////////////////////////////////////
			
		}

	}
	
	return newGeom.release();
}




TGen::Engine::Physics::Joint * TGen::Engine::Physics::ComponentFactory::createJoint(const std::string & name, const TGen::PropertyTree & properties, dWorldID worldId) {
	if (properties.getNumAttributes() == 0)
		throw TGen::RuntimeException("PhysicsSubsystem::createJoint", "no attributes, plz give some");
	
	std::string jointType = properties.getAttribute(0);
	
	dJointID newJointId;
	
	if (jointType == "ball")
		newJointId = dJointCreateBall(worldId, 0);
	else
		throw TGen::RuntimeException("PhysicsSubsystem::createJoint", "joint type '" + jointType + "' invalid");
	
	TGen::Engine::Physics::Joint * newComponent = new TGen::Engine::Physics::Joint(name, newJointId);
	newComponent->setLink1(properties.getProperty("attach1", ""));
	newComponent->setLink2(properties.getProperty("attach2", ""));
	newComponent->setAnchor(TGen::Vector3::Parse(properties.getProperty("anchor", "0 0 0")));
	
	return newComponent;
}
