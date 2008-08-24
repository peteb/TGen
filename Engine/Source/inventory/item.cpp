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

#include <iostream>


int TGen::Engine::Inventory::Item::increaseValue(int value) {
	int ret = value;
	
	if (this->value + value > maxValue)
		ret -= this->value + value - maxValue;
	
	this->value += value;
	this->value = std::min(this->value, maxValue);

	std::cout << "INCREASED WITH " << value << " TO " << this->value << std::endl;
	
	return ret;
}


void TGen::Engine::Inventory::Item::setValue(int value) {
	this->value = std::min(value, maxValue);
	
	std::cout << "changed to " << value << std::endl;
}
