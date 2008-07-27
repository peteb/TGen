/*
 *  componentlinker.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "componentlinker.h"
#include "entity.h"
#include "entitylist.h"

TGen::Engine::ComponentLinker::ComponentLinker(TGen::Engine::EntityList * entityList, TGen::Engine::Entity * entity)
	: entityList(entityList)
	, entity(entity)
{
	
}

TGen::Engine::Component * TGen::Engine::ComponentLinker::getGlobalComponent(const std::string & name) const {
	TGen::Engine::Component * ret = NULL;
	
	if (entityList) {	
		if (entity)
			ret = entityList->getComponent(name, *entity, std::nothrow);
		else
			ret = entityList->getComponent(name, std::nothrow);
	}
	
	return ret;
}

TGen::Engine::Component * TGen::Engine::ComponentLinker::getEntityComponent(int index) const {
	TGen::Engine::Component * ret = NULL;
	
	if (entity)
		ret = entity->getComponent(index, std::nothrow);
	
	return ret;
}

TGen::Engine::EntityList * TGen::Engine::ComponentLinker::getEntityList() const {
	return entityList;
}

TGen::Engine::Entity * TGen::Engine::ComponentLinker::getEntity() const {
	return entity;
}

