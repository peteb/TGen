/*
 *  entity.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ENTITY_H
#define _TGEN_ENGINE_ENTITY_H

#include <string>
#include <map>
#include <vector>
#include "triggerable.h"
#include "triggercontext.h"
#include "symbols.h"

namespace TGen {
	namespace Engine {
		class Component;
		class EntityList;
		class ComponentLinker;
		
		class Entity : public TGen::Engine::Triggerable {
		public:
			Entity(const std::string & name);
			~Entity();
			
			void link(const TGen::Engine::ComponentLinker & linker);
			void addComponent(TGen::Engine::Component * component, const std::string & name);
			void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
			void initialize();
			
			TGen::Engine::Component * getComponent(const std::string & name, std::nothrow_t noth);
			TGen::Engine::Component & getComponent(const std::string & name);
			
			TGen::Engine::Component * getComponent(int index, std::nothrow_t noth);
			TGen::Engine::Component & getComponent(int index);

			TGen::Engine::TriggerContext & getContext();
			void registerEvent(TGen::Engine::Symbol symbolId, TGen::Engine::Triggerable * event);
			
			const std::string & getName() const;
			
		private:
			void triggerGetComponent(TGen::Engine::TriggerContext & context);
			void triggerRespondsTo(TGen::Engine::TriggerContext & context);
			
			typedef std::map<std::string, TGen::Engine::Component *> ComponentMap;
			typedef std::map<int, TGen::Engine::Triggerable *> EventMap;
			typedef std::map<int, TGen::Engine::Component *> ComponentSymbolMap;

			std::vector<TGen::Engine::Component *> components;

			ComponentSymbolMap componentSymbols;
			ComponentMap componentLookup;			
			EventMap events;

			std::string name;
			static TGen::Engine::Symbol symbolGetComponent, symbolRespondsTo;
			TGen::Engine::TriggerContext context;
			TGen::Engine::Triggerable * initializer, * dispatcher;
		}; 
		
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITY_H
