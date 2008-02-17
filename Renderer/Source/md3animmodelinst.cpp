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
{
	
}

TGen::MD3::AnimatingModelInstance::~AnimatingModelInstance() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
}

bool TGen::MD3::AnimatingModelInstance::isPureInstance() const {
	return true;
}

// TODO: riktig animering, indexbuffers ska INTE DUPLICERAS FÖR VARJE KEYFRAME OMG FFS
//       ska vara en samling tags per frame, så interpolerar man här typ, getJoint

void TGen::MD3::AnimatingModelInstance::update() {
	static float num = 0.0f;
	
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
	
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->linkMaterial(source);
}

void TGen::MD3::AnimatingModelInstance::unlinkMaterial() {
	TGen::NewModelInstance::unlinkMaterial();
	
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

void TGen::MD3::AnimatingModelInstance::writeMeta(uint metaType, TGen::VertexStream & stream) {
	typedef TGen::JoinVertexElements2<TGen::Vertex3<float>, TGen::Color4<float> > MetaLineDecl;
	
	MetaLineDecl::Type axes[6];
	axes[0].x = 0.0f;
	axes[0].y = 0.0f;
	axes[0].z = 0.0f;
	axes[0].r = 1.0f;
	axes[0].g = 0.0f;
	axes[0].b = 0.0f;
	axes[0].a = 1.0f;

	axes[1].x = 1.0f;
	axes[1].y = 0.0f;
	axes[1].z = 0.0f;
	axes[1].r = 1.0f;
	axes[1].g = 0.0f;
	axes[1].b = 0.0f;
	axes[1].a = 1.0f;


	axes[2].x = 0.0f;
	axes[2].y = 0.0f;
	axes[2].z = 0.0f;
	axes[2].r = 0.0f;
	axes[2].g = 1.0f;
	axes[2].b = 0.0f;
	axes[2].a = 1.0f;
	
	axes[3].x = 0.0f;
	axes[3].y = 1.0f;
	axes[3].z = 0.0f;
	axes[3].r = 0.0f;
	axes[3].g = 1.0f;
	axes[3].b = 0.0f;
	axes[3].a = 1.0f;
	
	
	axes[4].x = 0.0f;
	axes[4].y = 0.0f;
	axes[4].z = 0.0f;
	axes[4].r = 0.0f;
	axes[4].g = 0.0f;
	axes[4].b = 1.0f;
	axes[4].a = 1.0f;
	
	axes[5].x = 0.0f;
	axes[5].y = 0.0f;
	axes[5].z = 1.0f;
	axes[5].r = 0.0f;
	axes[5].g = 0.0f;
	axes[5].b = 1.0f;
	axes[5].a = 1.0f;
	
	for (int i = 0; i < 6; ++i)
		stream.writeVertex(&axes[i]);
}

