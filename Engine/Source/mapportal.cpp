/*
 *  mapportal.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/8/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "mapportal.h"
#include "map.h"

TGen::Engine::MapPortal::MapPortal(int posAreaId, int negAreaId)
	: posAreaId(posAreaId)
	, negAreaId(negAreaId)
	, posArea(NULL)
	, negArea(NULL)
	, open(false)
{
	points.reserve(4);
}

void TGen::Engine::MapPortal::addPoint(const TGen::Vector3 & point) {
	points.push_back(point);
}

int TGen::Engine::MapPortal::getNumPoints() const {
	return points.size();
}

const TGen::Vector3 & TGen::Engine::MapPortal::getPoint(int num) const {
	return points[num];
}

void TGen::Engine::MapPortal::link(TGen::Engine::Map * map) {
	posArea = map->getModel("_area" + TGen::lexical_cast<std::string>(posAreaId));
	negArea = map->getModel("_area" + TGen::lexical_cast<std::string>(negAreaId));
	
	if (points.size() >= 3)
		plane = TGen::Plane3(points[0], points[1], points[2]);
	else
		throw TGen::RuntimeException("MapPortal::link", "not enough points to create plane");
}

TGen::Engine::MapModel * TGen::Engine::MapPortal::getPosArea() {
	return posArea;
}

TGen::Engine::MapModel * TGen::Engine::MapPortal::getNegArea() {
	return negArea;
}

const TGen::Vector3 * TGen::Engine::MapPortal::getPoints() const {
	return &points[0];
}

const TGen::Plane3 & TGen::Engine::MapPortal::getPlane() const {
	return plane;
}
