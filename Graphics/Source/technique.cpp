/*
 *  technique.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "technique.h"
#include "pass.h"
#include "passlist.h"
#include <iostream>

TGen::Technique::Technique() 
	: expressLane(NULL)
{
	for (int i = 0; i < 10; ++i)
		lods.push_back(NULL);
}

TGen::Technique::~Technique() {
	for (int i = 0; i < lods.size(); ++i)
		delete lods[i];
}


void TGen::Technique::setPassList(PassList * pass, int lod) {
	if (lods[lod])
		delete lods[lod];
	
	lods[lod] = pass;
	
	int lodsFound = 0;
	for (int i = 0; i < lods.size(); ++i) {
		if (lods[i])
			lodsFound++;
	}
	
	if (lodsFound == 0)
		expressLane = pass;
	else
		expressLane = NULL;
}

TGen::PassList * TGen::Technique::getPassList(int lod) {
	if (expressLane)
		return expressLane;
	
	for (int i = lod; i >= 0; --i) {
		if (lods[i]) {
			//std::cout << "lod " << i << " chosen" << std::endl;
			return lods[i];
		}
	}
	
	for (int i = lod; i < 10; ++i) {
		if (lods[i]) {
			std::cout << "no lower lod, chosing " << i << std::endl;
			return lods[i];
		}
	}	
	
	return NULL;
}

void TGen::Technique::link(TGen::MaterialLinkCallback & callback) {
	PassVector::iterator iter = lods.begin();
	for (; iter != lods.end(); ++iter) {
		if (*iter)
			(*iter)->link(callback);
	}
}

void TGen::Technique::update(scalar time) {
	PassVector::iterator iter = lods.begin();
	for (; iter != lods.end(); ++iter) {
		if (*iter)
			(*iter)->update(time);
	}
}

