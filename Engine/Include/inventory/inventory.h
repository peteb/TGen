/*
 *  inventory.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_INVENTORY_H
#define _TGEN_ENGINE_INVENTORY_H

#include "component.h"
#include "item.h"

namespace TGen {
	namespace Engine {
		namespace Inventory {
			class Item;
			
			class Inventory : public TGen::Engine::Component {
			public:
				Inventory(const std::string & name);
				
				void addItem(const std::string & name, const TGen::Engine::Inventory::Item & item);
				int getItemValue(const std::string & name);
				void addItemValue(const std::string & name, int value);
				
			private:
				typedef std::map<std::string, TGen::Engine::Inventory::Item> ItemMap;
				
				ItemMap items;
			};
		} // !Inventory
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_INVENTORY_H

