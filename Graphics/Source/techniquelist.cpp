/*
 *  techniquelist.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 8/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "techniquelist.h"
#include "technique.h"

TGen::TechniqueList::TechniqueList(const std::string & name)
	: name(name)
	, timesUsed(0)
{
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
			timesUsed++;

			return techniques[i];
		}
	}
	
	for (int i = minreqs; i < 10; ++i) {
		if (techniques[i]) {
			//std::cout << "no lower technique, chosing " << i << std::endl;
			timesUsed++;

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

void TGen::TechniqueList::link(TGen::MaterialLinkCallback & callback) {
	TechniqueVector::iterator iter = techniques.begin();
	for (; iter != techniques.end(); ++iter) {
		if (*iter)
			(*iter)->link(callback);
	}
}

void TGen::TechniqueList::update(scalar time) {
	TechniqueVector::iterator iter = techniques.begin();
	for (; iter != techniques.end(); ++iter) {
		if (*iter)
			(*iter)->update(time);
	}	
}

const std::string & TGen::TechniqueList::getName() const {
	return name;
}

int TGen::TechniqueList::getTimesUsed() const {
	return timesUsed;
}
