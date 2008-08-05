/*
 *  eventoperation.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_EVENTOPERATION_H
#define _TGEN_ENGINE_SCRIPT_EVENTOPERATION_H

#include "triggerable.h"

namespace TGen {
	namespace Engine {
		class EntityList;
		class Entity;
		class ComponentLinker;
		class EntityRecipe;
		
		namespace Script {
			class EventOperation {
			public:
				EventOperation(const std::string & name, EventOperation * parent);
				virtual ~EventOperation();
				
				void addOperation(EventOperation * operation);
				void addAlias(const std::string & alias, const std::string & value);
				std::string getAlias(const std::string & alias) const;
				void setNumParameters(int num);
				int getNumParameters() const;
				void setNumLocalVars(int localVars);
				int getNumLocalVars() const;
				EventOperation * getParent() const;
				std::string getName() const;
				
				int getNumOperations() const;
				EventOperation * getOperation(int id) const;
				
				virtual void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				virtual void link(const TGen::Engine::ComponentLinker & linker);
				virtual void prelink(const TGen::Engine::ComponentLinker & linker);
				
			private:
				std::vector<EventOperation *> operations;
				EventOperation * parent;
				
				typedef std::map<std::string, std::string> AliasMap;
				AliasMap aliases;
				int numParameters, numLocalVars;
				std::string name;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_EVENTOPERATION_H

