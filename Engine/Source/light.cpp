/*
 *  light.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "light.h"
#include "lightlist.h"

TGen::Engine::Light::Light(const std::string & name, const TGen::Vector3 & position, const TGen::Quaternion4 & orientation)
	: TGen::SceneNode(name, position, orientation)
	, material(NULL)
{
}

void TGen::Engine::Light::setMaterialName(const std::string & material) {
	materialName = material;
}

TGen::Light & TGen::Engine::Light::getLightProperties() {
	return light;
}

bool TGen::Engine::Light::fillUser(TGen::RenderList & list, const TGen::Camera & camera) const {
	light.position = orientation;	// TODO: globalOrientation perhaps
	//std::cout << std::string(TGen::Vector3(orientation)) << std::endl;
	list.addUser(static_cast<void *>(const_cast<TGen::Engine::Light *>(this)), TGen::Engine::UserTypeLight);  // fult knep
	return true;
}

TGen::Material * TGen::Engine::Light::getMaterial() {
	return material;
}

void TGen::Engine::Light::linkMaterial(TGen::MaterialSource & linker) {
	material = linker.getMaterial(materialName);
}
