/*
 *  setting.cpp
 *  settingregistry
 *
 *  Created by Peter Backman on 8/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "setting.h"

TGen::Engine::Setting::Setting(const std::string & name, const std::string & value, const std::string & defaultValue, uint flags)
	: name(name)
	, value(value)
	, defaultValue(defaultValue)
	, flags(flags)
{
	observers.reserve(1);	
}

TGen::Engine::Setting::~Setting() {
	triggerRemoved();
}

TGen::Engine::SettingChangeRejected::SettingChangeRejected(const Setting & setting, const std::string & description)
	: TGen::RuntimeException(setting.getName(), description)
{		
}

TGen::Engine::SettingObserver::~SettingObserver() {
	for (int i = 0; i < settingsObserving.size(); ++i) {
		settingsObserving[i]->removeObserver(this);
	}
}

void TGen::Engine::SettingObserver::addSettingObserved(Setting * setting) {
	settingsObserving.push_back(setting);
}

void TGen::Engine::Setting::addObserver(SettingObserver * observer) {
	if (std::find(observers.begin(), observers.end(), observer) != observers.end())
		return;
	
	observers.push_back(observer);
	observer->addSettingObserved(this);
}

void TGen::Engine::Setting::removeObserver(SettingObserver * observer) {
	ObserverList::iterator iter = std::find(observers.begin(), observers.end(), observer);
	if (iter != observers.end())
		observers.erase(iter);
}

void TGen::Engine::Setting::triggerPreChange(const std::string & newValue) {
	for (int i = 0; i < observers.size(); ++i) {
		observers[i]->preSettingChange(*this, newValue);
	}
}

void TGen::Engine::Setting::triggerRemoved() {
	for (int i = 0; i < observers.size(); ++i) {
		observers[i]->onSettingRemoved(*this);
	}	
}

std::string TGen::Engine::Setting::getName() const {
	return name;
}

uint TGen::Engine::Setting::getFlags() const {
	return flags;
}
