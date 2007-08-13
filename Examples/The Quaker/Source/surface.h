/*
 *  surface.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_SURFACE_H
#define _THEQUAKER_SURFACE_H

#include <tgen_graphics.h>
#include "geometry.h"

class SceneNode;

class SurfaceLinker {
public:
	virtual TGen::Material * getMaterial(const std::string & name) abstract;
};

class Surface : public Geometry {
public:	
	Surface(const std::string & materialName, Geometry * geometry);
	Surface(TGen::Material * material, Geometry * geometry);
	~Surface();
	
	void Attached(SceneNode * parent);
	TGen::Material * getMaterial() const;
	void Link(SurfaceLinker & linker);
	void preRender(TGen::Renderer & renderer) const;
	void render(TGen::Renderer & renderer) const;
	SceneNode * getSceneNode() const;
	TGen::Vector3 getMin() const;
	TGen::Vector3 getMax() const;
	Geometry * getGeometry() const;

private:
	std::string materialName;
	TGen::Material * material;
	SceneNode * sceneNode;
	Geometry * geometry;
};

#endif // !_THEQUAKER_SURFACE_H
