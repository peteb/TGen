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
}

void TGen::Engine::EntityRecipe::linkGlobally(TGen::Engine::EntityList & entities) {
	for (RecipeList::iterator iter = componentRecipes.begin(); iter != componentRecipes.end(); ++iter)
		(*iter)->linkGlobally(entities, *this);
	
}
