/*
 *  equipmentdata.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/28/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "scene/equipmentdata.h"
#include "scene/equipmentnode.h"
#include "inventory/inventory.h"
#include "inventory/weapon.h"
#include "entity.h"
#include "componentlinker.h"

TGen::Engine::Scene::EquipmentData::EquipmentData(const std::string & name)
	: name(name)
	, node(NULL)
	, inventory(NULL)
	, weapon(NULL)
	, index(0)
{
}

void TGen::Engine::Scene::EquipmentData::setRequires(const std::string & requiresName) {
	this->requiresName = requiresName;
}

void TGen::Engine::Scene::EquipmentData::setWeapon(const std::string & weaponName) {
	this->weaponName = weaponName;
}

void TGen::Engine::Scene::EquipmentData::setIndex(int index) {
	this->index = index;
}

int TGen::Engine::Scene::EquipmentData::getIndex() const {
	return index;
}

TGen::SceneNode * TGen::Engine::Scene::EquipmentData::getNode() {
	return node;
}

TGen::Engine::Inventory::Weapon * TGen::Engine::Scene::EquipmentData::getWeapon() {
	return weapon;
}

void TGen::Engine::Scene::EquipmentData::link(const TGen::Engine::ComponentLinker & linker, TGen::Engine::Scene::EquipmentNode * node) {
	this->node = node->getChild(name);
	inventory = dynamic_cast<TGen::Engine::Inventory::Inventory *>(linker.getComponent("inventory"));
	weapon = dynamic_cast<TGen::Engine::Inventory::Weapon *>(linker.getComponent(weaponName));
}

bool TGen::Engine::Scene::EquipmentData::inInventory() {
	if (!inventory)
		return true;
	
	int itemCount = inventory->getItemValue(requiresName);

	return (itemCount == -1 || itemCount > 0);
}
