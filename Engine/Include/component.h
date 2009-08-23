/*
 *  component.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMPONENT_H
#define _TGEN_ENGINE_COMPONENT_H

#include <string>
#include <vector>

namespace TGen {
	namespace Engine {
		class Entity;
		class EntityList;
		class ComponentLinker;
		
		namespace Script {
			class ComponentScript;
		}
		
		class Component {
		public:
			Component(const std::string & name, bool staticComponent = false);
			virtual ~Component();
			
			//virtual void linkLocally(TGen::Engine::Entity & entity);
			//virtual void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);

			virtual void link(const TGen::Engine::ComponentLinker & linker);

			virtual void setEnabled(bool enabled) {}
			
			bool isStatic() const;
			const std::string & getName() const;
			TGen::Engine::Component * getParent() const;
			void setOwner(TGen::Engine::Entity * owner);
			//TGen::Engine::Entity * getOwner() const;
			void addSubcomponent(TGen::Engine::Component * component);
			void setScriptInterface(TGen::Engine::Script::ComponentScript * scriptInterface);
			TGen::Engine::Script::ComponentScript * getScriptInterface();
			
			int getNumChildren() const;
			TGen::Engine::Component * getChild(int id);
			
		protected:
			TGen::Engine::Component * parent;
			TGen::Engine::Script::ComponentScript * scriptInterface;
			std::vector<TGen::Engine::Component *> children;
			
			const std::string name;
			const bool staticComponent;
			TGen::Engine::Entity * owner;
		};
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_COMPONENT_H
