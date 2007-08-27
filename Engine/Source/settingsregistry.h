/*
 *  settingsregistry.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SETTINGSREGISTRY_H
#define _TGEN_ENGINE_SETTINGSREGISTRY_H

#include "setting.h"

namespace TGen {
	namespace Engine {
		class SettingsRegistry {
		public:
			SettingsRegistry();
			~SettingsRegistry();
			
			const TGen::Engine::Setting & getSetting(const std::string & name) const;
			TGen::Engine::Setting & getSetting(const std::string & name);
			void addSetting(const TGen::Engine::Setting & setting);

		private:
			typedef std::map<std::string, TGen::Engine::Setting> SettingMap;
			SettingMap settings;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SETTINGSREGISTRY_H

