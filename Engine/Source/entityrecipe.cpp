/*
 *  entityrecipe.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "entityrecipe.h"
#include "componentrecipe.h"
#include "entity.h"
#include "component.h"

TGen::Engine::EntityRecipe::EntityRecipe(const std::string & name) 
	: name(name)
{
}

TGen::Engine::EntityRecipe::~EntityRecipe() {
	for (int i = 0; i < componentRecipes.size(); ++i)
		delete componentRecipes[i];
}

const std::string & TGen::Engine::EntityRecipe::getName() const {
	return name;
}

void TGen::Engine::EntityRecipe::addComponentRecipe(TGen::Engine::ComponentRecipe * recipe, const std::string & name) {
	componentRecipes.push_back(recipe);
	recipeLookup.insert(std::make_pair(name, recipe));
}

void TGen::Engine::EntityRecipe::linkGlobally(TGen::Engine::EntityList & entities) {
	for (RecipeList::iterator iter = componentRecipes.begin(); iter != componentRecipes.end(); ++iter)
		(*iter)->linkGlobally(entities, *this);
	
}

TGen::Engine::Entity * TGen::Engine::EntityRecipe::createEntity() const {
	std::auto_ptr<TGen::Engine::Entity> newEntity(new TGen::Engine::Entity(name));
	
	for (RecipeList::const_iterator iter = componentRecipes.begin(); iter != componentRecipes.end(); ++iter) {
		TGen::Engine::Component * newComponent = (*iter)->createComponent(*this, *newEntity.get());
		newEntity->addComponent(newComponent, newComponent->getName());
	}
	
	for (int i = 0; i < componentRecipes.size(); ++i) {
	//	fastLinkConstructed(TGen::Engine::Component & constructed, TGen::Engine::Entity & entity);
		componentRecipes[i]->fastLinkConstructed(*newEntity->getComponent(i), *newEntity);
	}
	
	return newEntity.release();
}

TGen::Engine::ComponentRecipe * TGen::Engine::EntityRecipe::getComponentRecipe(const std::string & name) {
	RecipeMap::iterator iter = recipeLookup.find(name);
	if (iter == recipeLookup.end())
		return NULL;
		
		//throw TGen::RuntimeException("EntityRecipe::getComponentRecipe", "recipe " + name + " does not exist");
	
	return iter->second;
}

void TGen::Engine::EntityRecipe::setWorldInterface(const std::string & worldInterface) {
	this->worldInterface = worldInterface;
}

// TODO: dummy scene node, does everything except draw itself

const std::string & TGen::Engine::EntityRecipe::getWorldInterface() const {
	return worldInterface;
}

int TGen::Engine::EntityRecipe::getComponentIndex(const std::string & name) const {
	for (int i = 0; i < componentRecipes.size(); ++i) {
		if (componentRecipes[i]->getName() == name)
			return i;
	}
	
	return -1;
}

