/*
 *  interpolator.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/3/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "utilities/interpolator.h"
#include "utilities/interpolatorscript.h"

#include "componentlinker.h"
#include "worldobject.h"

// TODO: interpolator -> controllers

TGen::Engine::Utilities::Interpolator::Interpolator(const std::string & name)
	: TGen::Engine::Component(name)
	, positionDelegate(NULL)
	, time(0.0f)
	, speed(1.0f)
	, scriptInterface(NULL)
	, enabled(true)
{
}

TGen::Engine::Utilities::Interpolator::~Interpolator() {
	delete scriptInterface;
}

void TGen::Engine::Utilities::Interpolator::link(const TGen::Engine::ComponentLinker & linker) {	
	positionDelegate = dynamic_cast<TGen::Engine::WorldObject *>(linker.getComponent(delegateName));
}

void TGen::Engine::Utilities::Interpolator::update(scalar dt) {
	if (!enabled)
		return;
	
	float lastPos = time;
	time += dt * speed;

	
	if ((time < 0.0f && lastPos >= 0.0f) || (time > 1.0f && lastPos <= 1.0f)) {
		enabled = false;
	
		if (scriptInterface)
			scriptInterface->onReachedEnd();
	}

	time = TGen::Clamp(time, 0.0f, 1.0f);
	TGen::Vector3 pos = TGen::Interpolate(keypoints[0], keypoints[1], time);
	
	if (positionDelegate)
		positionDelegate->setPosition(pos);
}

void TGen::Engine::Utilities::Interpolator::setPositionDelegate(const std::string & delegateName) {
	this->delegateName = delegateName;
}

void TGen::Engine::Utilities::Interpolator::setSpeed(float speed) {
	this->speed = speed;
}

void TGen::Engine::Utilities::Interpolator::addKeypoint(const TGen::Vector3 & point) {
	keypoints.push_back(point);
}

void TGen::Engine::Utilities::Interpolator::setScriptInterface(TGen::Engine::Utilities::InterpolatorScript * scriptInterface) {
	this->scriptInterface = scriptInterface;
}

void TGen::Engine::Utilities::Interpolator::setEnabled(bool enabled) {
	this->enabled = enabled;
}

bool TGen::Engine::Utilities::Interpolator::getEnabled() const {
	return enabled;
}


