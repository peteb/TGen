/*
 *  mapmodel.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "mapmodel.h"
#include <tgen_renderer.h>
#include "mapsurface.h"
#include "map.h"
#include "metacreator.h"
#include "mapportal.h"

TGen::Engine::MapModel::MapModel(const std::string & name, TGen::Engine::Map * map)
	: TGen::NewModel(name, "", "")
	, TGen::SceneNode(name)
	, name(name)
	, map(map)
	, instantiated(false)
{
	
}

std::string TGen::Engine::MapModel::getName() const {
	return name;
}

void TGen::Engine::MapModel::addSurface(TGen::Engine::MapSurface * surface) {
	surfaces.push_back(surface);
	//faces.push_back(new TGen::NewFace(surface, NULL, map));
}

/*bool TGen::Engine::MapModel::fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {
	for (int i = 0; i < faces.size(); ++i)
		list.addFace(*faces[i]);
	
	//throw TGen::RuntimeException("MapModel::fillFaces", "BARG");
	
	return true;
}

void TGen::Engine::MapModel::linkMaterials(TGen::MaterialSource & source) {
//	for (int i = 0; i < faces.size(); ++i)
	//	faces[i]->linkMaterial(source);
}*/

/*void TGen::Engine::MapModel::createVertexData(TGen::VertexDataSource & source) {
	for (int i = 0; i < surfaces.size(); ++i)
		surfaces[i]->createVertexData(source);
}*/

bool TGen::Engine::MapModel::isPureInstance() const {
	return false;
}

TGen::NewModelInstance * TGen::Engine::MapModel::instantiate(TGen::VertexDataSource & source) {
	if (instantiated)
		throw TGen::RuntimeException("MapMode::instantiate", "already instantiated!");
	
	std::cout << "MapModel::instantiate" << std::endl;
	std::cout << "surfaces: " << surfaces.size() << std::endl;
	
	instantiated = true;
	
	for (SurfaceList::iterator iter = surfaces.begin(); iter != surfaces.end(); ++iter) {
		(*iter)->instantiate(source);
	}
	
	return this;
}

void TGen::Engine::MapModel::linkMaterial(TGen::MaterialSource & source) {
	std::cout << "link material" << std::endl;
	
	for (SurfaceList::iterator iter = surfaces.begin(); iter != surfaces.end(); ++iter)
		(*iter)->linkMaterial(source);
}


void TGen::Engine::MapModel::unlinkMaterial() {
	for (SurfaceList::iterator iter = surfaces.begin(); iter != surfaces.end(); ++iter)
		(*iter)->unlinkMaterial();	
}

void TGen::Engine::MapModel::fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, const TGen::SceneNode * node) {
	for (SurfaceList::iterator iter = surfaces.begin(); iter != surfaces.end(); ++iter) {		
		list.addFace(TGen::NewFace(*iter, (overridingMaterial ? overridingMaterial : (*iter)->getMaterial()), node));
	}
}

void TGen::Engine::MapModel::writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream) {
	//if (metaType == TGen::MetaPortals) {   // TODO: varför funkar inte MetaPortals
		TGen::Engine::MetaCreator mc;
		//mc.writeAxes(transform, stream);
	
	for (PortalList::iterator iter = portals.begin(); iter != portals.end(); ++iter) {
		mc.writePolygon(transform, stream, (*iter)->getPoints(), (*iter)->getNumPoints(), ((*iter)->open ? TGen::Color::Green : TGen::Color::Red));
	}
	//}
}

void TGen::Engine::MapModel::addPortal(TGen::Engine::MapPortal * portal) {
	portals.push_back(portal);
}

int TGen::Engine::MapModel::getNumPortals() const {
	return portals.size();
}

TGen::Engine::MapPortal * TGen::Engine::MapModel::getPortal(int num) {
	return portals[num];
}

void TGen::Engine::MapModel::traverse(const TGen::SceneNode::Walker & walker) {	
	if (walker.pre(*this)) {
		for (SceneNodeList::iterator iter = children.begin(); iter != children.end(); ++iter) {
			(*iter)->traverse(walker);
		}
	}
	
	walker.post(*this);
}


