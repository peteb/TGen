/*
 *  variable.h
 *  variableregistry
 *
 *  Created by Peter Backman on 8/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_VARIABLE_H
#define _TGEN_ENGINE_VARIABLE_H

#include <string>
#include <vector>
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		enum VariableFlags {
			VariableReadOnly =			0x00000001,		// read only, forever. Can't be changed by dev
			VariableCheat =				0x00000002,		// read only, can be changed by dev
			VariableDump =				0x00000004,		// dumped to file when app is quitting
			VariableConfigWriteOnly =	0x00000008,		// only writable until the app starts
			VariableNonInit =			0x00000010,		// not initialized; created by set but never initialized.
			VariableUser =				0x00000100,		// base line for extensions
		};


		class Variable;

		class VariableObserver {
			typedef std::vector<Variable *> VariableList;
			VariableList variablesObserving;
			
			void addVariableObserved(Variable * variable);
			
			friend class Variable;
			
		public:
			virtual ~VariableObserver();
			
			virtual void preVariableChange(const Variable & variable, const std::string & newValue) {}
			virtual void postVariableChange(const Variable & variable) {}
			virtual void onVariableRemoved(const Variable & variable) {}
		};


		class Variable {
			typedef std::vector<VariableObserver *> ObserverList;
			
			ObserverList observers;
			std::string name, value, defaultValue;
			uint flags;
			
			void triggerPreChange(const std::string & newValue);
			void triggerPostChange();
			void triggerRemoved();
			
		public:
			Variable(const std::string & name, const std::string & value, const std::string & defaultValue, uint flags);
			~Variable();
			
			void addObserver(VariableObserver * observer);
			void removeObserver(VariableObserver * observer);
			void takeDefaults(const TGen::Engine::Variable & var);
			

			std::string getName() const;
			std::string getDefaultValue() const;
			uint getFlags() const;
			
			template<typename T> Variable & operator = (const T & value) {
				std::string newValue = TGen::lexical_cast<std::string>(value);
				triggerPreChange(newValue);
				this->value = newValue;
				triggerPostChange();
				return *this;
			}

			
			template<typename T> operator T () const {
				return getValue<T>();
			}
			
			operator std::string() const {
				return value;
			}
			
			Variable & operator = (const std::string & value) {
				triggerPreChange(value);
				this->value = value;
				triggerPostChange();
				return *this;
			}
				
			template<typename T> T getValue() const {
				return TGen::lexical_cast<T>(value);
			}

			std::string getValue() const {
				return value;
			}
			
		}; 


		class VariableChangeRejected : public TGen::RuntimeException {
		public:
			VariableChangeRejected(const Variable & variable, const std::string & description = "");
			
		};

	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VARIABLE_H
