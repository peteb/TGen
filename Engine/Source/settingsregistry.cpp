/*
 *  settingsregistry.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "settingsregistry.h"
#include <tgen_core.h>

TGen::Engine::SettingsRegistry::SettingsRegistry() {
	
}

TGen::Engine::SettingsRegistry::~SettingsRegistry() {
	
}

const TGen::Engine::Setting & TGen::Engine::SettingsRegistry::getSetting(const std::string & name) const {
	SettingMap::const_iterator iter = settings.find(name);
	
	if (iter == settings.end())
		throw TGen::RuntimeException("SettingsRegistry::getSetting", "setting '" + name + "' does not exist");
	
	return iter->second;
}

TGen::Engine::Setting & TGen::Engine::SettingsRegistry::getSetting(const std::string & name) {
	SettingMap::iterator iter = settings.find(name);
	
	if (iter == settings.end())
		throw TGen::RuntimeException("SettingsRegistry::getSetting", "setting '" + name + "' does not exist");
	
	return iter->second;	
}

void TGen::Engine::SettingsRegistry::addSetting(const TGen::Engine::Setting & setting) {
	SettingMap::const_iterator iter = settings.find(setting.getName());
	
	if (iter != settings.end())	// TODO: check whether the setting has a "remove on new"-flag
		throw TGen::RuntimeException("SettingsRegistry::addSetting", "setting '" + setting.getName() + "' already exists");
	
	settings.insert(SettingMap::value_type(setting.getName(), setting));
}
