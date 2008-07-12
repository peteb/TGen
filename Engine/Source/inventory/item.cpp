/*
 *  item.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "inventory/item.h"
#include <algorithm>

void TGen::Engine::Inventory::Item::increaseValue(int value) {
	this->value += value;
	this->value = std::min(this->value, maxValue);
}
