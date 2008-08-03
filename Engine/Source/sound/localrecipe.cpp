/*
 *  localrecipe.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/23/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "localrecipe.h"
#include "entityrecipe.h"
#include "localsource.h"
#include "entity.h"
#include "componentinterfaces.h"
#include "sound/subsystem.h"
#include "componentlinker.h"

TGen::Engine::Sound::LocalRecipe::LocalRecipe(const std::string & name, TGen::Engine::Sound::Subsystem & soundSubsystem)
	: TGen::Engine::ComponentRecipe(name)
	, componentLinkNum(-1)
	, prototypeSource(NULL)
	, soundSubsystem(soundSubsystem)
{
}

TGen::Engine::Component * TGen::Engine::Sound::LocalRecipe::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	std::cout << "CREATE SOUND COMPONENT" << std::endl;

	LocalSource * newSource = new LocalSource(*dynamic_cast<TGen::Engine::Sound::LocalSource *>(prototypeSource));
	//newSource->setLink(dynamic_cast<TGen::Engine::WorldObject *>(constructing.getComponent(componentLinkNum)));
	newSource->setPrototype(false);
	soundSubsystem.addLocalSource(newSource);
	
	return newSource;
}


// called when the entity has been constructed from the prototype
void TGen::Engine::Sound::LocalRecipe::fastLinkConstructed(const TGen::Engine::ComponentLinker & linker, TGen::Engine::Component & constructed) {
	TGen::Engine::WorldObject * object = dynamic_cast<TGen::Engine::WorldObject *>(linker.getComponent(componentLinkNum));
	
	dynamic_cast<TGen::Engine::Sound::LocalSource &>(constructed).setLink(object);
}


// called when the prototype has been constructed
void TGen::Engine::Sound::LocalRecipe::link(const TGen::Engine::ComponentLinker & linker, TGen::Engine::EntityRecipe & prototype) {
	componentLinkNum = prototype.getComponentIndex(linkWithName);
	
	std::cout << "COMPONENT LINK NUM: " << componentLinkNum << std::endl;
}

void TGen::Engine::Sound::LocalRecipe::setLinkWith(const std::string & link) {
	linkWithName = link;
}

void TGen::Engine::Sound::LocalRecipe::setSoundPrototype(TGen::Engine::Sound::Source * source) {
	prototypeSource = source;
}


