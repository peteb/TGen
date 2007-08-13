/*
 *  face.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "face.h"

TGen::Vector3 TGen::Face::getMin() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}

TGen::Vector3 TGen::Face::getMax() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}
