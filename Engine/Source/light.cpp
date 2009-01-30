/*
 *  light.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "light.h"
#include "renderer/lightlist.h"

using TGen::uint;

TGen::Engine::Light::Light(const std::string & name, const TGen::Vector3 & position, const TGen::Rotation & orientation)
	: TGen::SceneNode(name, position, orientation)
	, material(NULL)
	, type(-1)
	, lightBoundingBoxCull(false)
	, directions(DirAll)
{
}

void TGen::Engine::Light::setMaterialName(const std::string & material) {
	materialName = material;
}

bool TGen::Engine::Light::fillUser(TGen::RenderList & list, const TGen::Camera & camera) const {
	//light.position = orientation * TGen::Vector3(0.0f, 0.0f, 1.0f);	// we're using the position field for direction
	// TODO: globalOrientation perhaps
	//std::cout << std::string(TGen::Vector3(orientation)) << std::endl;
	list.addUser(static_cast<void *>(const_cast<TGen::Engine::Light *>(this)), TGen::Engine::UserTypeLight);  // fult knep
	return true;
}

bool TGen::Engine::Light::fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {
	return false;
}

TGen::Material * TGen::Engine::Light::getMaterial() {
	return material;
}

void TGen::Engine::Light::linkMaterial(TGen::MaterialSource & linker) {
	material = linker.getMaterial(materialName);
	
	try {
		std::string typeName = material->getParameter("light_type").at(0);
	
		if (typeName == "directional")
			type = LightDirectional;
		else if (typeName == "positional")
			type = LightPositional;
		else if (typeName == "spotlight")
			type = LightSpotlight;
		
		if (type == LightPositional)
			lightBoundingBoxCull = TGen::lexical_cast<bool>(material->getParameter("light_bb_cull").at(0));
		
		bool shadowCaster = TGen::lexical_cast<bool>(material->getParameter("light_shadow").at(0));
		//std::string lightTexture = material->getParameter("light_texture").at(0);
		std::string lightDirs = material->getParameter("light_dirs").at(0);
		
		
		directions = ParseDirections(lightDirs);
		
	}
	catch (const std::exception & e) {
		std::cout << "LIGHT TYPE ERROR FOR " << materialName << ": " << e.what() << std::endl;
	}
}

int TGen::Engine::Light::getType() const {
	return type;
}

bool TGen::Engine::Light::isPointInsideBox(const TGen::Vector3 & point) const {
	return true;
}

void TGen::Engine::Light::setBoundingBox(const TGen::Vector3 & box) {
	bounding = box;
}

TGen::Vector3 TGen::Engine::Light::getBoundingBox() const {
	return bounding;
}

uint TGen::Engine::Light::getDirections() const {
	return directions;
}

void TGen::Engine::Light::setDirections(uint directions) {
	this->directions = directions;
}

void TGen::Engine::Light::setViewAngle(const TGen::Degree & angle) {
	viewAngle = angle;
}

const TGen::Degree & TGen::Engine::Light::getViewAngle() const {
	return viewAngle;
}

uint TGen::Engine::Light::ParseDirections(const std::string & desc) {
	uint ret = 0;
	
	//if (desc.empty())
		//return DirAll;
	
	if (desc.find("X") != std::string::npos)
		ret |= DirPosX;
	if (desc.find("x") != std::string::npos)
		ret |= DirNegX;
	if (desc.find("Y") != std::string::npos)
		ret |= DirPosY;
	if (desc.find("y") != std::string::npos)
		ret |= DirNegY;
	if (desc.find("Z") != std::string::npos)
		ret |= DirPosZ;
	if (desc.find("z") != std::string::npos)
		ret |= DirNegZ;
	
	
	return ret;
}

