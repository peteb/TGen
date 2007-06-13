/*
 *  renderer.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderer.h"
#include "error.h"

TGen::Renderer::Renderer() : clearColor(TGen::Color::Black) {}
TGen::Renderer::~Renderer() {}

void TGen::Renderer::setClearColor(const TGen::Color & color) {
	clearColor = color;
}

TGen::Color TGen::Renderer::getClearColor() const {
	return clearColor;
}

void TGen::Renderer::setViewport(const TGen::Rectangle & viewport) {
	this->viewport = viewport;
}

TGen::Rectangle TGen::Renderer::getViewport() const {
	return viewport;
}

void TGen::Renderer::setTransform(TGen::TransformMode mode, const TGen::Matrix4x4 & transformation) {
	switch (mode) {
		case TGen::ProjectionTransform:
			projectionMatrix = transformation;
			break;
			
		case TGen::WorldTransform:
			worldMatrix = transformation;
			break;
			
		case TGen::TextureTransform:
			textureMatrix = transformation;
			break;
			
		default:
			throw TGen::RuntimeException("Renderer::setTransform", "invalid transformation mode");
	}
}

TGen::Matrix4x4 TGen::Renderer::getTransform(TGen::TransformMode mode) const {
	switch (mode) {
		case TGen::ProjectionTransform:
			return projectionMatrix;
			
		case TGen::WorldTransform:
			return worldMatrix;
			
		case TGen::TextureTransform:
			return textureMatrix;
	}

	throw TGen::RuntimeException("Renderer::getTransform", "invalid transformation mode");
}

