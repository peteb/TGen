/*
 *  equipmentnode.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "scene/equipmentnode.h"
#include "scene/equipmentdata.h"
#include "inventory/weapon.h"

TGen::Engine::Scene::EquipmentNode::EquipmentNode(const std::string & name) 
	: TGen::SceneNode(name)
	, visibleEquipment(NULL)
{
	
}

TGen::Engine::Scene::EquipmentNode::~EquipmentNode() {
	for (EquipmentMap::iterator iter = equipmentData.begin(); iter != equipmentData.end(); ++iter) {
		delete iter->second;
	}
}

void TGen::Engine::Scene::EquipmentNode::linkGlobally(TGen::Engine::EntityList & list, TGen::Engine::Entity & entity) {
	for (EquipmentMap::iterator iter = equipmentData.begin(); iter != equipmentData.end(); ++iter)
		iter->second->linkGlobally(this, list, entity);
	
	EquipmentMap::iterator iter = equipmentData.find(initialChild);
	if (iter != equipmentData.end())
		changeEquipment(iter->second);
}

void TGen::Engine::Scene::EquipmentNode::setInitialChild(const std::string & initialChild) {
	this->initialChild = initialChild;
}

void TGen::Engine::Scene::EquipmentNode::traverse(const TGen::SceneNode::Walker & walker) {
	if (walker.getFlags() & TGen::SceneNode::WalkerFollowVisibility) {
		if (visibleEquipment) {
			if (visibleEquipment->getNode())
				visibleEquipment->getNode()->traverse(walker);
		
		}
	}
	else {
		TGen::SceneNode::traverse(walker);
	}
}

void TGen::Engine::Scene::EquipmentNode::setEquipmentData(const std::string & name, TGen::Engine::Scene::EquipmentData * data) {
	data->setIndex(equipmentSorted.size());
	equipmentSorted.push_back(data);
	equipmentData.insert(std::make_pair(name, data));
}

void TGen::Engine::Scene::EquipmentNode::changeEquipment(TGen::Engine::Scene::EquipmentData * equipment) {
	if (equipment->inInventory())
		visibleEquipment = equipment;
	else
		std::cout << "NOT IN INVENTORY" << std::endl;
}

void TGen::Engine::Scene::EquipmentNode::beginFire(int mode) {
	if (visibleEquipment)
		visibleEquipment->getWeapon()->beginFire(mode);
}

void TGen::Engine::Scene::EquipmentNode::endFire(int mode) {
	if (visibleEquipment)
		visibleEquipment->getWeapon()->endFire(mode);	
}

void TGen::Engine::Scene::EquipmentNode::changeEquipmentRelative(int relative) {
	int newIndex = 0;
	if (visibleEquipment)
		newIndex = visibleEquipment->getIndex() + relative;
	
	if (newIndex < 0)
		newIndex = equipmentSorted.size() + newIndex;
	
	//if (newIndex > equipmentSorted.size() - 1)
		newIndex %= equipmentSorted.size();
	
	std::cout << "NEW INDEX: " << newIndex << std::endl;
	
	changeEquipmentAbsolute(newIndex);
}

void TGen::Engine::Scene::EquipmentNode::changeEquipmentAbsolute(int absolute) {
	changeEquipment(equipmentSorted.at(absolute));
}

