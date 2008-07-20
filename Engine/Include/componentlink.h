/*
 *  componentlink.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMPONENTLINK_H
#define _TGEN_ENGINE_COMPONENTLINK_H

#include "entitylist.h"
#include "entity.h"

namespace TGen {
	namespace Engine {
		template<typename T>
		class ComponentLink {
		public:
			ComponentLink(int index)
				: linkedComponent(NULL)
				, componentIndex(index)
			{
				
			}
		
			ComponentLink(const std::string & name)
				: linkedComponent(NULL)
				, componentIndex(-1)
				, componentName(name)
			{
			}
			
			ComponentLink() 
				: linkedComponent(NULL)
				, componentIndex(-1)
			{
				
			}
			
			T & operator * () {
				if (!linkedComponent)
					throw TGen::RuntimeException("ComponentLink::*", "no component linked!");
				
				return *linkedComponent;
			}
			
			T * operator -> () {
				if (!linkedComponent)
					throw TGen::RuntimeException("ComponentLink::*", "no component linked!");
				
				return linkedComponent;
			}
			
			operator bool() const {
				return linkedComponent != NULL;
			}
			
			void link(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
				TGen::Engine::Component * component = NULL;
				
				if (componentIndex != -1)
					component = entity.getComponent(componentIndex, std::nothrow);
				else if (!componentName.empty())
					component = entities.getComponent(componentName, entity, std::nothrow);
				//else
				//	throw TGen::RuntimeException("ComponentLink::link", "no resource to link");
				
				linkedComponent = dynamic_cast<T *>(component);
			}
						
			ComponentLink<T> & operator = (const std::string & name) {
				componentName = name;
				
				return *this;
			}
			
		private:
			std::string componentName;
			int componentIndex;
			
			T * linkedComponent;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTLINK_H

