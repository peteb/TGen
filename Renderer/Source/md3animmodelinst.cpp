/*
 *  md3animmodelinst.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmodelinst.h"
#include "md3animmodel.h"
#include "md3animmeshinst.h"
#include "renderlist.h"

TGen::MD3::AnimatingModelInstance::AnimatingModelInstance(const std::string & name, 
																			 const std::string & materialNamePostfix, 
																			 TGen::MD3::AnimatingModel & base) 
	: TGen::NewModelInstance(name, "", materialNamePostfix)
	, base(base)
	, materialSource(NULL)
	, num(0.0f)
{
	
}

TGen::MD3::AnimatingModelInstance::~AnimatingModelInstance() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
}

bool TGen::MD3::AnimatingModelInstance::isPureInstance() const {
	return true;
}

// TODO: riktig animering, indexbuffers ska INTE DUPLICERAS F÷R VARJE KEYFRAME OMG FFS
//       ska vara en samling tags per frame, sÂ interpolerar man h‰r typ, getJoint

void TGen::MD3::AnimatingModelInstance::update() {	
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->updateVertices(int(num), num - float(int(num)));
	
	num += 0.01f;
	if (num > 200.0)
		num = 0.0;
}

TGen::ModelJoint TGen::MD3::AnimatingModelInstance::getJoint(const std::string & name) const {
	return base.getJoint(name);
}

void TGen::MD3::AnimatingModelInstance::linkMaterial(TGen::MaterialSource & source) {
	TGen::NewModelInstance::linkMaterial(source);
	
	materialSource = &source;
	
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->linkMaterial(source);
}

void TGen::MD3::AnimatingModelInstance::unlinkMaterial() {
	TGen::NewModelInstance::unlinkMaterial();
	
	materialSource = NULL;
	
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->unlinkMaterial();	
}

void TGen::MD3::AnimatingModelInstance::fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, TGen::SceneNode const * node) {
	for (int i = 0; i < meshes.size(); ++i) {
		list.addFace(TGen::NewFace(meshes[i], (overridingMaterial ? overridingMaterial : meshes[i]->getMaterial()), node));
	}	
}

int TGen::MD3::AnimatingModelInstance::getNumMeshes() const {
	return meshes.size();
}

TGen::NewMeshInstance * TGen::MD3::AnimatingModelInstance::getMesh(int num) {
	return meshes[num];
}

void TGen::MD3::AnimatingModelInstance::addMesh(TGen::MD3::AnimatingMeshInstance * mesh) {
	meshes.push_back(mesh);
}

void TGen::MD3::AnimatingModelInstance::writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream) {	
	if (metaType == TGen::MetaAxis) {
		typedef TGen::JoinVertexElements2<TGen::Vertex3<float>, TGen::Color4<float> > MetaLineDecl;
		
		TGen::Vector3 start = transform.getOrigin();
		TGen::Vector3 end = start + transform.getZ();
		
		MetaLineDecl::Type points[2];
		points[0].x = start.x;
		points[0].y = start.y;
		points[0].z = start.z;
		points[0].r = 1.0f;
		points[0].g = 1.0f;
		points[0].b = 0.0f;
		points[0].a = 1.0f;
			
		points[1].x = end.x;
		points[1].y = end.y;
		points[1].z = end.z;
		points[1].r = 1.0f;
		points[1].g = 1.0f;
		points[1].b = 0.0f;
		points[1].a = 1.0f;
			
		stream.writeVertex(&points[0]);
		stream.writeVertex(&points[1]);
	}
	
}

TGen::NewModelInstance * TGen::MD3::AnimatingModelInstance::clone() const {
	TGen::NewModelInstance * newInstance = base.instantiate(base.getDataSource());
	
	if (materialSource)
		newInstance->linkMaterial(*materialSource);
	
	return newInstance;
}