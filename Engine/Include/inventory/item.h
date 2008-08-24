/*
 *  item.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_INVENTORY_ITEM_H
#define _TGEN_ENGINE_INVENTORY_ITEM_H

namespace TGen {
	namespace Engine {
		namespace Inventory {
			class Item {
			public:
				int increaseValue(int value);
				void setValue(int value);
				
				int value, maxValue;
			};
		} // !Inventory
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_INVENTORY_ITEM_H

