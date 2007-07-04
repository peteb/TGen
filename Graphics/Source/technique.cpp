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

TGen::Technique::Technique() {
	for (int i = 0; i < 10; ++i)
		lods.push_back(NULL);
}

TGen::Technique::~Technique() {
	
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
		if (techniques.at(i)) {
			std::cout << "technique " << i << " chosen" << std::endl;
			return techniques[i];
		}
	}
	
	for (int i = minreqs; i < 10; ++i) {
		if (techniques.at(i)) {
			std::cout << "no lower technique, chosing " << i << std::endl;
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
}

TGen::PassList * TGen::Technique::getPassList(int lod) {
	for (int i = lod; i >= 0; --i) {
		if (lods.at(i)) {
			std::cout << "lod " << i << " chosen" << std::endl;
			return lods[i];
		}
	}
	
	for (int i = lod; i < 10; ++i) {
		if (lods.at(i)) {
			std::cout << "no lower lod, chosing " << i << std::endl;
			return lods[i];
		}
	}	
	
	return NULL;
}
