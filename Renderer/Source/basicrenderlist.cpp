/*
 *  basicrenderlist.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "basicrenderlist.h"
#include "face_new.h"
#include "geometry.h"
#include "scenenode.h"
#include "camera.h"
#include "meshinstance_new.h"
#include <tgen_graphics.h>
#include <algorithm>
#include "modelrenderproperties.h"

TGen::BasicRenderList::BasicRenderList() {
	faces.reserve(11000);
	opaqueFaces.reserve(10000);
	transparentFaces.reserve(1000);
	userInfo.reserve(400);
	metaWriters.reserve(100);
}

TGen::BasicRenderList::~BasicRenderList() {}

void TGen::BasicRenderList::addFace(const TGen::NewFace & face) {
	if (!face.getMaterial())
		throw TGen::RuntimeException("BasicRenderList::addFace", "no material linked!");
	
//	if  (!face->getGeometry())
	//	throw TGen::RuntimeException("BasicRenderList::addFace", "no geometry!");
		
	/*if (face->getMaterial()->getSortLevel() == TGen::MaterialSortOpaque)
		opaqueFaces.push_back(face);
	else
		transparentFaces.push_back(face);*/
	
	faces.push_back(face);
}

void TGen::BasicRenderList::addUser(void * user, int id) {
	userInfo.push_back(UserInfo(user, id));
}

void TGen::BasicRenderList::clear() {
	faces.clear();
	opaqueFaces.clear();
	transparentFaces.clear();
	userInfo.clear();
	metaWriters.clear();
}

void TGen::BasicRenderList::sort(const TGen::Camera & camera, const std::string & specialization) {
	float lodNear = camera.getLod().lodNear;
	float lodFar = camera.getLod().lodFar;
	
	if (needSorting()) {
		for (int i = 0; i < faces.size(); ++i) {
			scalar distance = (faces[i].getWorldOrigin() - camera.getWorldPosition()).getMagnitude();
			int lod = 9 - int(((distance - lodNear) / lodFar) * 10.0 - 1.0);	
			lod = TGen::Clamp(lod, 0, 9);
			int sortLevel = faces[i].getMaterial()->getSpecialization(specialization)->getPassList(lod)->getSortLevel();	// ouch vilken rad
		
			if (sortLevel == TGen::MaterialSortOpaque)
				opaqueFaces.push_back(SortedFace(faces[i], distance));
			else
				transparentFaces.push_back(SortedFace(faces[i], distance));
		}
		
		// TODO: sort per material, possibly using a map
		
		//calculateCameraDistance(transparentFaces, camera);
		//calculateCameraDistance(opaqueFaces, camera);
		std::sort(transparentFaces.begin(), transparentFaces.end(), TGen::BasicRenderList::Sorter());
	}
}

void TGen::BasicRenderList::render(TGen::Renderer & renderer, const TGen::Matrix4x4 & baseMat, const TGen::LodInfo & lod, const std::string & specialization) {
	//renderer.setTransform(TGen::TransformProjection, camera.getProjection());

	renderList(opaqueFaces, renderer, baseMat, lod, specialization);
	renderList(transparentFaces, renderer, baseMat, lod, specialization);
}

void TGen::BasicRenderList::renderList(TGen::BasicRenderList::SortedFaceList & list, TGen::Renderer & renderer, const TGen::Matrix4x4 & baseMat, const TGen::LodInfo & lod, const std::string & specialization) {
	//std::cout << std::string(baseMat) << std::endl;
	
	TGen::SceneNode const * lastNode = NULL;
	//std::cout << ">>RENDER<<" << std::endl;
	//std::cout << "basemat: " << std::endl << std::string(baseMat) << std::endl;
	
	scalar lodNear = lod.lodNear; //camera.getLodNear();
	scalar lodFar = lod.lodFar; //camera.getLodFar();
	scalar clipFar = lod.clipFar; //camera.getClipFar();
	
	std::map<TGen::Renderable *, bool> faceRendered;
	
	for (int i = 0; i < list.size(); ++i) {
		const TGen::NewFace * face = &list[i].face;
		TGen::SceneNode const * node = face->getSceneNode();
		
		if (node && lastNode != node) {
			renderer.setTransform(TGen::TransformWorldView, baseMat * node->getTransform());
			//std::cout << "SET TRANSFORM: " << std::endl << std::string(renderer.getTransform(TGen::TransformWorldView)) << std::endl;
			
			lastNode = node;
		}
		
		int lod = 9 - int(((list[i].distanceToCamera - lodNear) / lodFar) * 10.0 - 1.0);
		lod = TGen::Clamp(lod, 0, 9);
		
		//std::cout << lod << std::endl;
		if (lod > 0) {
			if (face->getRenderProperties())
				renderer.setFaceWinding(face->getRenderProperties()->frontFaceDef);
			
			TGen::Material * selectedMaterial = face->getMaterial();
			
			if (materialOverride)
				selectedMaterial = materialOverride;
			
			if (!faceRendered[face->getMesh()]) {
				faceRendered[face->getMesh()] = true;
				
				TGen::MaterialRenderMetadata metadata(specialization, lod, NULL, 0, face->getMesh(), override);
				
				selectedMaterial->render(renderer, *face->getMesh(), metadata);
			}
		}
	}
}

void TGen::BasicRenderList::renderWithinRadius(TGen::Renderer & renderer, const TGen::Matrix4x4 & baseMat, const TGen::LodInfo & lod, const TGen::Vector3 & pos, scalar radius) {
	TGen::BasicRenderList::SortedFaceList & list = opaqueFaces;
	
	std::map<TGen::Renderable *, bool> faceRendered;
	
	//renderer.setTransform(TGen::TransformProjection, camera.getProjection());
	
	int numRendered = 0;
	
	for (int i = 0; i < list.size(); ++i) {
		const TGen::NewFace * face = &list[i].face;
		const TGen::SceneNode * node = face->getSceneNode();
		
		
		bool passedRadius = false;
		
		TGen::NewMeshInstance * mesh = face->getMesh();
		
		scalar objectRadius = (mesh->min - mesh->max).getMagnitude();
		TGen::Matrix4x4 transform = baseMat * node->getTransform();
		TGen::Vector3 objectOrigin = transform * mesh->origin;
		
		if (abs((objectOrigin - pos).getMagnitude()) <= objectRadius + radius)
			passedRadius = true;
		
		if (passedRadius) {
			renderer.setTransform(TGen::TransformWorldView, transform);

			if (face->getRenderProperties())
				renderer.setFaceWinding(face->getRenderProperties()->frontFaceDef);
				
			TGen::Material * selectedMaterial = face->getMaterial();
				
			if (materialOverride)
				selectedMaterial = materialOverride;
				
			if (!faceRendered[face->getMesh()]) {
				faceRendered[face->getMesh()] = true;
				
				TGen::MaterialRenderMetadata metadata("default", 9, NULL, 0, face->getMesh(), override);
				selectedMaterial->render(renderer, *face->getMesh(), metadata);
				
				numRendered++;
			}
		}
		
	}
}

void TGen::BasicRenderList::calculateCameraDistance(SortedFaceList & list, const TGen::Camera & camera) {
	for (int i = 0; i < list.size(); ++i) {
		list[i].distanceToCamera = (list[i].face.getWorldOrigin() - camera.getWorldPosition()).getMagnitude();
	}
}

void TGen::BasicRenderList::print() {
	std::cout << "opaque: " << std::endl;
	for (int i = 0; i < opaqueFaces.size(); ++i)
		std::cout << &opaqueFaces[i].face << std::endl;
	
	std::cout << "transparent: " << std::endl;
	for (int i = 0; i < transparentFaces.size(); ++i)
		std::cout << &transparentFaces[i].face << "  " << std::string(transparentFaces[i].face.getWorldOrigin()) << " - " << transparentFaces[i].distanceToCamera << std::endl;
}

bool TGen::BasicRenderList::needSorting() {
	return true;
}

TGen::BasicRenderList::Sorter::Sorter() {
}

TGen::BasicRenderList::SortedFace::SortedFace(const TGen::NewFace & face, scalar distance)
	: face(face)
	, distanceToCamera(distance)
{
}

bool TGen::BasicRenderList::Sorter::operator() (const TGen::BasicRenderList::SortedFace & face1, const TGen::BasicRenderList::SortedFace & face2) {
	return (face1.distanceToCamera > face2.distanceToCamera);
}

int TGen::BasicRenderList::getNumUserInfo() {
	return userInfo.size();
}

TGen::RenderList::UserInfo & TGen::BasicRenderList::getUserInfo(int id) {
	return userInfo[id];
}

void TGen::BasicRenderList::addMeta(TGen::MetaWriter * metaWriter, const TGen::SceneNode * node) {
	metaWriters.push_back(Meta(metaWriter, node));
}

void TGen::BasicRenderList::writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream) {
	for (int i = 0; i < metaWriters.size(); ++i) {
		metaWriters[i].writer->writeMeta(metaType, metaWriters[i].node->getTransform(), stream);		
	}
}

int TGen::BasicRenderList::getNumFaces() {
	return faces.size();
}
