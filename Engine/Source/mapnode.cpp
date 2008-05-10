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
	std::cout << "CREATED AREA LEAF " << areaNumber << std::endl;
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
	delete this->posLeaf;
	posLeaf = pos;
}

void TGen::Engine::MapLinkNode::setNegLeaf(TGen::Engine::MapLeafNode * neg) {
	delete this->negLeaf;
	negLeaf = neg;
}

void TGen::Engine::MapLinkNode::link(TGen::Engine::Map * map) {
	if (!posLeaf && posChild > 0) {
		pos = map->getNode(posChild);
		pos->link(map);
	}
	else if (posLeaf) {
		posLeaf->link(map);
	}
			  
	if (!negLeaf && negChild > 0) {
		neg = map->getNode(negChild);
		neg->link(map);
	}
	else if (negLeaf) {
		negLeaf->link(map);
	}
}

void TGen::Engine::MapLeafNode::link(TGen::Engine::Map * map) {
	std::string modelName = "_area" + TGen::lexical_cast<std::string>(areaNumber);
	
	area = map->getModel(modelName);
}

const TGen::Engine::MapModel * TGen::Engine::MapLeafNode::getModel() const {
	return area;
}

TGen::Engine::MapModel * TGen::Engine::MapLeafNode::getModel() {
	return area;
}

void TGen::Engine::MapLinkNode::print(int level) {
	std::string sep;
	for (int i = 0; i < level; ++i)
		sep += "   ";
	
	std::cout << sep << "LINK" << std::endl;
	
	if (pos) {
		std::cout << sep << "posChild: (" << pos << ")" << std::endl;
		pos->print(level + 1);
	}

	if (neg) {
		std::cout << sep << "negChild: (" << neg << ")" << std::endl;
		neg->print(level + 1);		
	}
	
	if (posLeaf) {
		std::cout << sep << "posLeaf: (" << posLeaf << ")" << std::endl;
		posLeaf->print(level + 1);
	}

	if (negLeaf) {
		std::cout << sep << "negLeaf: (" << negLeaf << ")" << std::endl;
		negLeaf->print(level + 1);
	}
	
	std::cout << sep << "END LINK" << std::endl;
}

void TGen::Engine::MapLeafNode::print(int level) {
	std::string sep;
	for (int i = 0; i < level; ++i)
		sep += "   ";

	std::cout << sep << "LEAF" << std::endl;
	std::cout << sep << "areaNumber: " << areaNumber << std::endl;
	std::cout << sep << "linkedModel: " << area << std::endl;
	std::cout << sep << "END LEAF" << std::endl;
}


