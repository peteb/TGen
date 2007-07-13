/*
 *  plane.h
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_THEPLACE_PLANE_H
#define _TGEN_THEPLACE_PLANE_H

#include <tgen_math.h>
#include <tgen_graphics.h>

#define __TGEN__
#include <ode/ode.h>

class ResourceManager;

class Plane : public TGen::Renderable {
private:
	Plane(const TGen::Vector3 & position, const TGen::Vector3 & direction, TGen::Material * material, dGeomID geomId);

public:	
	~Plane();
	
	void LoadData(TGen::Renderer & renderer, ResourceManager & resources);
	void PrepareRender(TGen::Renderer & renderer);
	void Render(TGen::Renderer & renderer);
	TGen::Matrix4x4 & getTransform(TGen::Renderer & renderer);
	TGen::Material * getMaterial();
	
	friend class World;
	
private:
	typedef TGen::JoinVertex3<TGen::Vertex3<float>, TGen::TexCoord2<float, 0>, TGen::Normal3<float> > Vertex;
	
	TGen::Vector3 position, direction;
	TGen::Material * material;
	TGen::VertexBuffer * vb;
	TGen::Matrix4x4 cachedMatrix;
	
	dGeomID geomId;
};

#endif // !_TGEN_THEPLACE_PLANE_H
