/*
 *  setting.h
 *  settingregistry
 *
 *  Created by Peter Backman on 8/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SETTING_H
#define _TGEN_ENGINE_SETTING_H

#include <string>
#include <vector>
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		enum SettingFlags {
			SettingReadOnly =			0x00000001,
			SettingDump =				0x00000002,
			SettingConfigWriteOnly =	0x00000004,
			SettingUser =				0x00000010,		// base line for extensions
		};


		class Setting;

		class SettingObserver {
			typedef std::vector<Setting *> SettingList;
			SettingList settingsObserving;
			
			void addSettingObserved(Setting * setting);
			
			friend class Setting;
			
		public:
			virtual ~SettingObserver();
			
			virtual void preSettingChange(const Setting & setting, const std::string & newValue) {}
			virtual void onSettingRemoved(const Setting & setting) {}
		};


		class Setting {
			typedef std::vector<SettingObserver *> ObserverList;
			
			ObserverList observers;
			std::string name, value, defaultValue;
			uint flags;
			
			void triggerPreChange(const std::string & newValue);
			void triggerRemoved();

		public:
			Setting(const std::string & name, const std::string & value, const std::string & defaultValue, uint flags);
			~Setting();
			
			void addObserver(SettingObserver * observer);
			void removeObserver(SettingObserver * observer);
			

			std::string getName() const;
			uint getFlags() const;
			
			template<typename T> Setting & operator = (const T & value) {
				std::string newValue = TGen::lexical_cast<std::string>(value);
				triggerPreChange(newValue);
				this->value = newValue;
				return *this;
			}

			
			template<typename T> operator T () const {
				return getValue<T>();
			}
			
			operator std::string() const {
				return value;
			}
			
			Setting & operator = (const std::string & value) {
				triggerPreChange(value);
				this->value = value;
				return *this;
			}
				
			template<typename T> T getValue() const {
				return TGen::lexical_cast<T>(value);
			}

			std::string getValue() const {
				return value;
			}
			
		}; 


		class SettingChangeRejected : public TGen::RuntimeException {
		public:
			SettingChangeRejected(const Setting & setting, const std::string & description = "");
			
		};

	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SETTING_H
