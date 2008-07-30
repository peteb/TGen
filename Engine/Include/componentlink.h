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
#include "componentlinker.h"
#include "entityrecipe.h"

namespace TGen {
	namespace Engine {
		class ComponentLink {
		public:
			ComponentLink(int index)
				: componentIndex(index)
			{
				
			}
			
			ComponentLink(const std::string & name)
				: componentIndex(-1)
				, componentName(name)
			{
			}
			
			ComponentLink() 
				: componentIndex(-1)
			{
				
			}
			
			ComponentLink & operator = (const std::string & name) {
				componentName = name;
				
				return *this;
			}
			
			virtual void set(const std::string & name) {
				componentName = name;
			}
			
			void linkComponentIndex(const TGen::Engine::EntityRecipe & recipe) {
				componentIndex = recipe.getComponentIndex(componentName);
				std::cout << "CIND: found index for " << componentName << ": " << componentIndex << std::endl;
			}
			
		protected:
			TGen::Engine::Component * getComponent(const TGen::Engine::ComponentLinker & linker) {
				TGen::Engine::Component * component = NULL;
				
				if (componentIndex != -1)
					component = linker.getComponent(componentIndex);
				else if (!componentName.empty())
					component = linker.getComponent(componentName);
				
				if (!component)
					std::cerr << "warning: failed to find component " << componentName << " or index " << componentIndex << std::endl;
				else
					std::cerr << "component found: " << componentName << std::endl;
				
				return component;
			}
			
		private:
			std::string componentName;
			int componentIndex;
		};
		
		
		template<typename T>
		class UnaryDelegate : public TGen::Engine::ComponentLink {
		public:	
			UnaryDelegate() : linkedComponent(NULL) {}
			
			T & operator * () {
				if (!linkedComponent)
					throw TGen::RuntimeException("UnaryDelegate::*", "no component linked!");
				
				return *linkedComponent;
			}
			
			T * operator -> () {
				if (!linkedComponent)
					throw TGen::RuntimeException("UnaryDelegate::->", "no component linked!");
				
				return linkedComponent;
			}
			
			operator bool() const {
				return (linkedComponent != NULL);
			}
			
			T * get() const {
				return linkedComponent;
			}
			
			void set(T * link) {
				this->linkedComponent = link;
			}
			
			void set(const std::string & name) {
				TGen::Engine::ComponentLink::set(name);
			}
			
			void link(const TGen::Engine::ComponentLinker & linker) {
				linkedComponent = dynamic_cast<T *>(getComponent(linker));
			}
			
		private:
			T * linkedComponent;
		};
		
		

		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTLINK_H

