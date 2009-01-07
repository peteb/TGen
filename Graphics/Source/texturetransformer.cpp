/*
 *  texturetransformer.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "texturetransformer.h"
#include "generator.h"

TGen::TextureTransformer::TextureTransformer(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool centered) 
	: genU(genU)
	, genV(genV)
	, startedAt(0.0)
	, centered(centered)
{
}

TGen::TextureTransformer::~TextureTransformer() {
	delete genU;
	
	if (genV != genU)
		delete genV;
}

bool TGen::TextureTransformer::shouldCenter() const {
	return centered;
}


TGen::TextureTranslate::TextureTranslate(float u, float v, bool scroll)
	: TGen::TextureTransformer(NULL, NULL, false)
	, u(u)
	, v(v)
	, scroll(scroll) 
{
}

TGen::TextureTranslate::TextureTranslate(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool scroll)
	: TGen::TextureTransformer(genU, genV, false)
	, scroll(scroll) 
{
}

void TGen::TextureTranslate::setCoord(float u, float v) {
	this->u = u;
	this->v = v;
}

void TGen::TextureTranslate::applyTransform(TGen::Matrix4x4 & matrix, scalar time) {
	if (!scroll) {
		float fixedU = 0.0f, fixedV = 0.0f;
		
		if (!genU)
			fixedU = u;
		else
			fixedU = genU->getValue(time);
		
		if (!genV)
			fixedV = v;
		else
			fixedV = genV->getValue(time);
		
		//fixedU = fixedU - floor(fixedU);		// fix range. crappy code
		//fixedV = fixedV - floor(fixedV);		
		
		matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(-fixedU, fixedV));		
	}
	else {
		
		float fixedU = 0.0f, fixedV = 0.0f;
		
		if (!genU)
			fixedU = (time - startedAt) * u;
		else
			fixedU = genU->getValue(time);
		
		if (!genV)
			fixedV = (time - startedAt) * v;
		else
			fixedV = genV->getValue(time);				
		
		
		//	fixedU = fixedU - floor(fixedU);		// fix range
		//		fixedV = fixedV - floor(fixedV);
		
		matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(-fixedU, fixedV));
	}
}


TGen::TextureScale::TextureScale(float u, float v, bool centered)
	: TGen::TextureTransformer(NULL, NULL, centered)
	, u(u)
	, v(v)
{
}

TGen::TextureScale::TextureScale(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool centered)
	: TGen::TextureTransformer(genU, genV, centered)
	, u(0.0f)
	, v(0.0f)
{
}

void TGen::TextureScale::setScale(float u, float v) {
	this->u = u;
	this->v = v;
}

void TGen::TextureScale::applyTransform(TGen::Matrix4x4 & matrix, scalar time) {
	float fixedU = 0.0f, fixedV = 0.0f;
	
	if (!genU)
		fixedU = u;
	else
		fixedU = genU->getValue(time);
	
	if (!genV)
		fixedV = v;
	else
		fixedV = genV->getValue(time);
	
	if (fixedU < 0.0f)
		fixedU = 0.0f;
	if (fixedV < 0.0f)
		fixedV = 0.0f;
	
	matrix *= TGen::Matrix4x4::Scaling(TGen::Vector3(fixedU, fixedV, 1.0));
}

TGen::TextureRotate::TextureRotate(float speed, bool centered)
	: TGen::TextureTransformer(NULL, NULL, centered)
	, speed(speed)
{
}

TGen::TextureRotate::TextureRotate(TGen::ScalarGenerator * genRot, bool centered) 
	: TGen::TextureTransformer(genRot, NULL, centered)
	, speed(0.0f)
{
}

void TGen::TextureRotate::setSpeed(float speed) {
	this->speed = speed;
}

void TGen::TextureRotate::applyTransform(TGen::Matrix4x4 & matrix, scalar time) {
	if (!genU) {
		float degrees = (time - startedAt) * speed;
		
		degrees = degrees - floor(degrees / 360.0f) * 360.0f;
		//if (speed < 0.0)
		//		degrees = -degrees;
		
		matrix *= TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 0.0f, 1.0f), TGen::Degree(-degrees));
	}
	else {
		matrix *= TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 0.0f, 1.0f), TGen::Degree(genU->getValue(time)));
	}
}

