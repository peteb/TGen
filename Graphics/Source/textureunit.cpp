/*
 *  textureunit.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 8/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "textureunit.h"

TGen::TextureUnit::TextureUnit(int unit, TGen::Texture * texture) 
	: unit(unit)
	, texture(texture)
	, transformed(false)
	, transform(TGen::Matrix4x4::Identity)
	, genU(TGen::TextureCoordGenBase)
	, genV(TGen::TextureCoordGenBase)
	, textureType(0) 
	, minFilter(TGen::TextureFilterLinear)
	, magFilter(TGen::TextureFilterLinear)
{
}

TGen::TextureUnit::TextureUnit(int unit, int textureType)
	: unit(unit)
	, texture(NULL)
	, transformed(false)
	, transform(TGen::Matrix4x4::Identity)
	, genU(TGen::TextureCoordGenBase)
	, genV(TGen::TextureCoordGenBase)
	, textureType(textureType) 
	, minFilter(TGen::TextureFilterLinear)
	, magFilter(TGen::TextureFilterLinear)
{

}

TGen::Texture * TGen::TextureUnit::getTexture() const {
	return texture;
}

void TGen::TextureUnit::setCoordGen(TGen::TextureCoordGen genU, TGen::TextureCoordGen genV) {
	this->genU = genU;
	this->genV = genV;
}

void TGen::TextureUnit::getCoordGen(TGen::TextureCoordGen & genU, TGen::TextureCoordGen & genV) const {
	genU = this->genU;
	genV = this->genV;
}

int TGen::TextureUnit::getTextureType() const {
	return textureType;
}

int TGen::TextureUnit::getUnit() const {
	return unit;
}

void TGen::TextureUnit::setFilter(TGen::TextureFilter minFilter, TGen::TextureFilter magFilter) {
	this->minFilter = minFilter;
	this->magFilter = magFilter;
}

TGen::TextureFilter TGen::TextureUnit::getMinFilter() const {
	return minFilter;
}

TGen::TextureFilter TGen::TextureUnit::getMagFilter() const {
	return magFilter;
}
