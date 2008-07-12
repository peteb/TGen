/*
 *  itemvalueop.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_ITEMVALUEOP_H
#define _TGEN_ENGINE_SCRIPT_ITEMVALUEOP_H

#include "eventoperation.h"

namespace TGen {
	namespace Engine {
		namespace Inventory {
			class Item;
			class Inventory;
		}
		
		namespace Script {
			class ItemValueOperation : public TGen::Engine::Script::EventOperation {
			public:
				ItemValueOperation();
				
				void trigger(void ** argv, int argc);
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);
				
				void setItem(const std::string & item);
				void setParameter(int para);
				
			private:
				std::string itemName;
				TGen::Engine::Inventory::Item * item;
				TGen::Engine::Inventory::Inventory * inventory;
				int parameter;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_ITEMVALUEOP_H

