/*
 *  mapnode.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "mapnode.h"
#include "map.h"

TGen::Engine::MapLinkNode::MapLinkNode(const TGen::Plane3 & plane)
	: plane(plane)
	, posChild(0)
	, negChild(0)
	, pos(NULL)
	, neg(NULL)
	, posLeaf(NULL)
	, negLeaf(NULL)
{
}

TGen::Engine::MapLinkNode::~MapLinkNode() {
	delete pos;
	delete neg;
	delete posLeaf;
	delete negLeaf;
}


TGen::Engine::MapLeafNode::MapLeafNode(int areaNumber)
	: areaNumber(areaNumber)
	, area(NULL)
{
}

TGen::Engine::MapLeafNode::~MapLeafNode() {
	
}

void TGen::Engine::MapLinkNode::setPosChild(int posChild) {
	this->posChild = posChild;
	this->pos = NULL;
}

void TGen::Engine::MapLinkNode::setNegChild(int negChild) {
	this->negChild = negChild;
	this->neg = NULL;
}

void TGen::Engine::MapLinkNode::setPosLeaf(TGen::Engine::MapLeafNode * pos) {
	delete pos;
	posLeaf = pos;
}

void TGen::Engine::MapLinkNode::setNegLeaf(TGen::Engine::MapLeafNode * neg) {
	delete negLeaf;
	negLeaf = neg;
}

void TGen::Engine::MapLinkNode::link(TGen::Engine::Map * map) {
	if (posChild > 0)
		pos = map->getNode(posChild);
	if (negChild > 0)
		neg = map->getNode(negChild);
	
	if (pos)
		pos->link(map);
	if (neg)
		neg->link(map);
	if (posLeaf)
		posLeaf->link(map);
	if (negLeaf)
		negLeaf->link(map);
}

void TGen::Engine::MapLeafNode::link(TGen::Engine::Map * map) {
	area = map->getModel("_area" + TGen::lexical_cast<std::string>(areaNumber));
}

const TGen::Engine::MapModel * TGen::Engine::MapLeafNode::getModel() const {
	return area;
}

TGen::Engine::MapModel * TGen::Engine::MapLeafNode::getModel() {
	return area;
}

