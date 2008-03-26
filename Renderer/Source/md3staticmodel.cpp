/*
 *  md3staticmodel.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Renamed by Peter Backman on 2/3/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "md3staticmodel.h"
#include "md3staticmesh.h"
#include "renderlist.h"
#include "model_new.h"

TGen::MD3::StaticModel::StaticModel(const std::string & name, const std::string & materialName, const std::string & materialNamePostfix)
	: TGen::NewModel(name, materialName, materialNamePostfix)
{
		
}

TGen::MD3::StaticModel::~StaticModel() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];

}

bool TGen::MD3::StaticModel::isPureInstance() const {
	return false;
}

TGen::MD3::StaticModel * TGen::MD3::StaticModel::instantiate() {
	return this;
}

TGen::ModelJoint TGen::MD3::StaticModel::getJoint(const std::string & name) const {
	JointMap::const_iterator iter = joints.find(name);
	if (iter == joints.end())
		throw TGen::RuntimeException("Model::getJoint", "joint not found: '" + name + "'");
	
	return iter->second;
}

void TGen::MD3::StaticModel::addJoint(const std::string & name, const TGen::ModelJoint & joint) {
	if (joints.find(name) != joints.end())
		throw TGen::RuntimeException("MD3::Model::addJoint", "joint '" + name + "' already added");
	
	joints.insert(JointMap::value_type(name, joint));
}

int TGen::MD3::StaticModel::getNumMeshes() const {
	return meshes.size();
}

TGen::NewMeshInstance * TGen::MD3::StaticModel::getMesh(int num) {
	return meshes[num];
}

void TGen::MD3::StaticModel::addMesh(TGen::MD3::StaticMesh * instance) {
	meshes.push_back(instance);
}

void TGen::MD3::StaticModel::fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, const TGen::SceneNode * node) {
	for (int i = 0; i < meshes.size(); ++i) {
		list.addFace(TGen::NewFace(meshes[i], (overridingMaterial ? overridingMaterial : meshes[i]->getMaterial()), node));
	}
}

void TGen::MD3::StaticModel::linkMaterial(TGen::MaterialSource & source) {
	TGen::NewModelInstance::linkMaterial(source);

	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->linkMaterial(source);
}

void TGen::MD3::StaticModel::unlinkMaterial() {
	TGen::NewModelInstance::unlinkMaterial();
	
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->unlinkMaterial();	
}

void TGen::MD3::StaticModel::update() {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->update();
}

TGen::Vector3 TGen::MD3::StaticModel::getMax() const {
	return TGen::Vector3(1.0f, 1.0f, 1.0f);
}

TGen::Vector3 TGen::MD3::StaticModel::getMin() const {
	return TGen::Vector3(-1.0f, -1.0f, -1.0f);
}

TGen::Vector3 TGen::MD3::StaticModel::getOrigin() const {
	return TGen::Vector3::Zero;
}

void TGen::MD3::StaticModel::writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream) {
	typedef TGen::JoinVertexElements2<TGen::Vertex3<float>, TGen::Color4<float> > MetaLineDecl;
	
	// TODO: skicka med modelview transform...... måste
	for (JointMap::iterator iter = joints.begin(); iter != joints.end(); ++iter) {
		MetaLineDecl::Type points[2];
		TGen::Vector3 origin = iter->second.origin;
		TGen::Quaternion4 orient = TGen::Quaternion4(0.0f, 1.0f, 0.0f); //iter->second.orientation;
		
		//std::cout << "==> " << std::string(iter->second.origin) << std::endl;
		
		TGen::Vector3 start = transform * origin;
		TGen::Vector3 end = transform * (origin + TGen::Vector3(orient) * 1.0f);
		
	//	std::cout << std::string(start) << " - " << std::string(end) << std::endl;
		
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

// max/min/origin/radius i model
