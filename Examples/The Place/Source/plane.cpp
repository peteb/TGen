/*
 *  plane.cpp
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "plane.h"
#include <iostream>

Plane::Plane(const TGen::Vector3 & position, const TGen::Vector3 & direction, TGen::Material * material, dGeomID geomId) 
	: position(position), direction(direction), material(material), vb(NULL), geomId(geomId)
{
	
}

Plane::~Plane() {
	dGeomDestroy(geomId);
	delete vb;
}

void Plane::LoadData(TGen::Renderer & renderer, ResourceManager & resources) {
	float halfSize = 100.0f;
	
	Vertex::Type vertices[4] = {
		Vertex::Type(TGen::Vector3(halfSize, -halfSize, 0.0f), TGen::Vector2(0.0f, 0.0f), TGen::Vector3(0.0f, 0.0f, 1.0f)),
		Vertex::Type(TGen::Vector3(-halfSize, -halfSize, 0.0f), TGen::Vector2(halfSize, 0.0f), TGen::Vector3(0.0f, 0.0f, 1.0f)),
		Vertex::Type(TGen::Vector3(-halfSize, halfSize, 0.0f), TGen::Vector2(halfSize, halfSize), TGen::Vector3(0.0f, 0.0f, 1.0f)),
		Vertex::Type(TGen::Vector3(halfSize, halfSize, 0.0f), TGen::Vector2(0.0f, halfSize), TGen::Vector3(0.0f, 0.0f, 1.0f)),
	};
	
	vb = renderer.CreateVertexBuffer(Vertex(), sizeof(Vertex::Type) * 4, TGen::UsageStatic);
	vb->BufferData(vertices, sizeof(Vertex::Type) * 4, 0);
	
}

void Plane::PrepareRender(TGen::Renderer & renderer) {
	renderer.setVertexBuffer(vb);	
}

void Plane::Render(TGen::Renderer & renderer) {
	renderer.DrawPrimitive(TGen::PrimitiveQuads, 0, 4);	
}

TGen::Material * Plane::getMaterial() {
	return material;
}

void Plane::ApplyTransform(TGen::Renderer & renderer) {
	/*
	 Matrix4 ret = Matrix4::Identity();
	 Vector forward = dir.normalized();
	 Vector UP = up.normalized();
	 Vector right = forward.crossProduct(UP).normalize();
	 Vector newUp = forward.crossProduct(right).normalize();
	 
	 ret.set(right, -newUp, -forward);
	 ret = ret.Transpose();
	 ret *= Matrix4::Translation(-position);
	
	 */
	
	TGen::Vector3 x, y, z;
	TGen::Vector3 forward, up, right;
	
	forward = direction.getNormalized();
	up = TGen::Vector3(0.0f, 0.0f, -1.0f);
	right = TGen::Vector3::CrossProduct(forward, up).Normalize();
	//up = TGen::Vector3::CrossProduct(forward, right).Normalize();
		
	TGen::Matrix4x4 mat(right, -up, forward);
	
//	std::cout << std::string(mat) << std::endl;
	
	renderer.setTransform(TGen::TransformWorldView, TGen::Matrix4x4::Translation(TGen::Vector3(0.0f, -2.0f, -6.0f)) * mat);

}
