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

TGen::TechniqueList::TechniqueList() {
	for (int i = 0; i < 10; ++i)
		techniques.push_back(NULL);
}

TGen::TechniqueList::~TechniqueList() {
	for (int i = 0; i < techniques.size(); ++i)
		delete techniques[i];
}

TGen::Technique * TGen::TechniqueList::getTechnique(int minreqs) {
	for (int i = minreqs; i >= 0; --i) {
		if (techniques[i]) {
			//std::cout << "technique " << i << " chosen" << std::endl;
			return techniques[i];
		}
	}
	
	for (int i = minreqs; i < 10; ++i) {
		if (techniques[i]) {
			//std::cout << "no lower technique, chosing " << i << std::endl;
			return techniques[i];
		}
	}
	
	return NULL;
}

void TGen::TechniqueList::setTechnique(TGen::Technique * tech, int reqs) {
	if (techniques[reqs])
		delete techniques[reqs];
	
	techniques[reqs] = tech;
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

void TGen::Technique::Link(TGen::MaterialLinkCallback & callback) {
	PassVector::iterator iter = lods.begin();
	for (; iter != lods.end(); ++iter) {
		if (*iter)
			(*iter)->Link(callback);
	}
}

void TGen::TechniqueList::Link(TGen::MaterialLinkCallback & callback) {
	TechniqueVector::iterator iter = techniques.begin();
	for (; iter != techniques.end(); ++iter) {
		if (*iter)
			(*iter)->Link(callback);
	}
}

void TGen::TechniqueList::Update(scalar time) {
	TechniqueVector::iterator iter = techniques.begin();
	for (; iter != techniques.end(); ++iter) {
		if (*iter)
			(*iter)->Update(time);
	}	
}

void TGen::Technique::Update(scalar time) {
	PassVector::iterator iter = lods.begin();
	for (; iter != lods.end(); ++iter) {
		if (*iter)
			(*iter)->Update(time);
	}
}

