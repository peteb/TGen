/*
 *  basicrenderlist.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "basicrenderlist.h"
#include "face.h"
#include "geometry.h"
#include "scenenode.h"
#include "camera.h"
#include <tgen_graphics.h>
#include <algorithm>

TGen::BasicRenderList::BasicRenderList() {
	faces.reserve(11000);
	opaqueFaces.reserve(10000);
	transparentFaces.reserve(1000);
	userInfo.reserve(400);
}

TGen::BasicRenderList::~BasicRenderList() {}

void TGen::BasicRenderList::addFace(const TGen::Face * face) {
	if (!face->getMaterial())
		throw TGen::RuntimeException("BasicRenderList::addFace", "no material linked!");
	
	if  (!face->getGeometry())
		throw TGen::RuntimeException("BasicRenderList::addFace", "no geometry!");
		
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
}

void TGen::BasicRenderList::sort(const TGen::Camera & camera, const std::string & specialization) {
	float lodNear = camera.getLodNear();
	float lodFar = camera.getLodFar();
	
	if (needSorting()) {
		for (int i = 0; i < faces.size(); ++i) {
			scalar distance = (faces[i]->getWorldOrigin() - camera.getWorldPosition()).getMagnitude();
			int lod = 9 - int(((distance - lodNear) / lodFar) * 10.0 - 1.0);			
			int sortLevel = faces[i]->getMaterial()->getSpecialization(specialization)->getPassList(lod)->getSortLevel();	// ouch vilken rad
		
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

void TGen::BasicRenderList::render(TGen::Renderer & renderer, const TGen::Camera & camera, const std::string & specialization) {
	renderer.setTransform(TGen::TransformProjection, camera.getProjection());

	renderList(opaqueFaces, renderer, camera, specialization);
	renderList(transparentFaces, renderer, camera, specialization);
}

void TGen::BasicRenderList::renderList(TGen::BasicRenderList::SortedFaceList & list, TGen::Renderer & renderer, const TGen::Camera & camera, const std::string & specialization) {
	TGen::Matrix4x4 baseMat = camera.getTransform();
	
	//std::cout << std::string(baseMat) << std::endl;
	
	TGen::SceneNode * lastNode = NULL;
	//std::cout << ">>RENDER<<" << std::endl;
	//std::cout << "basemat: " << std::endl << std::string(baseMat) << std::endl;
	
	scalar lodNear = camera.getLodNear();
	scalar lodFar = camera.getLodFar();
	scalar clipFar = camera.getClipFar();
	
	// OPT: allt det här är förmodligen väldigt segt....
	for (int i = 0; i < list.size(); ++i) {
		scalar geomRadius = TGen::Sphere(list[i].face->getGeometry()->getMin(), list[i].face->getGeometry()->getMax()).radius;
		TGen::Plane3 cameraPlane(TGen::Vector3(camera.getWorldOrientation().x, camera.getWorldOrientation().y, camera.getWorldOrientation().z), 0.0f);
		
		TGen::Vector3 pos1, pos2;
		pos1 = list[i].face->getWorldOrigin() - camera.getWorldPosition() - TGen::Vector3(geomRadius, geomRadius, geomRadius);
		pos2 = list[i].face->getWorldOrigin() - camera.getWorldPosition() + TGen::Vector3(geomRadius, geomRadius, geomRadius);
		
		scalar dist1 = cameraPlane.getPointSide(pos1);
		scalar dist2 = cameraPlane.getPointSide(pos2);
		
	//	std::cout << "******* " << dist1 << "  " << dist2 << std::endl;
		
		//if (list[i].distanceToCamera < clipFar + geomRadius && (dist1 <= 0.0f || dist2 <= 0.0f)) {
			const TGen::Face * face = list[i].face;
			TGen::SceneNode * node = face->getSceneNode();
			
			if (node && lastNode != node) {	// only set a new transformation if it's another node
				renderer.setTransform(TGen::TransformWorldView, baseMat * node->getTransform());
				//std::cout << "SET TRANSFORM: " << std::endl << std::string(renderer.getTransform(TGen::TransformWorldView)) << std::endl;
				
				lastNode = node;
			}
			
			//std::cout << "POS IN CAM::::::: " << std::string(baseMat * node->getTransform() * face->getGeometry()->getOrigin()) << std::endl;
			
			int lod = 9 - int(((list[i].distanceToCamera - lodNear) / lodFar) * 10.0 - 1.0);
			TGen::Clamp(lod, 0, 9);
			
			//std::cout << "DIST: " << list[i].distanceToCamera << " LOD: " << lod << std::endl;
			const TGen::Geometry::SubfaceList * subfaces = face->getGeometry()->getLeaves();			

			TGen::Material * globalMaterial = face->getMaterial();

			if (!subfaces) {	// render the face
				globalMaterial->render(renderer, *face->getGeometry(), specialization, lod, NULL, face->getGeometry());	// TODO: opt specialization, symbol look-up before loop
			}
			else {	// render the leaves
				face->getGeometry()->preRender(renderer);
				
				for (int i = 0; i < subfaces->size(); ++i) {
					// material = subfaces[i]->getMaterial
					TGen::Material * material = globalMaterial;
					//if ((*subfaces)[i]->getMaterial())
					//	material = (*subfaces)[i]->getMaterial();
					
					material->render(renderer, *(*subfaces)[i], specialization, lod, NULL, face->getGeometry());
					
				}				
			}
	/*	}
		else {
			std::cout << "face discarded, too far away or behind camera" << std::endl;
		}*/
	}
}

void TGen::BasicRenderList::calculateCameraDistance(SortedFaceList & list, const TGen::Camera & camera) {
	for (int i = 0; i < list.size(); ++i) {
		list[i].distanceToCamera = (list[i].face->getWorldOrigin() - camera.getWorldPosition()).getMagnitude();
	}
}

void TGen::BasicRenderList::print() {
	std::cout << "opaque: " << std::endl;
	for (int i = 0; i < opaqueFaces.size(); ++i)
		std::cout << opaqueFaces[i].face << std::endl;
	
	std::cout << "transparent: " << std::endl;
	for (int i = 0; i < transparentFaces.size(); ++i)
		std::cout << transparentFaces[i].face << "  " << std::string(transparentFaces[i].face->getWorldOrigin()) << " - " << transparentFaces[i].distanceToCamera << std::endl;
}

bool TGen::BasicRenderList::needSorting() {
	return true;
}

TGen::BasicRenderList::Sorter::Sorter() {
}

TGen::BasicRenderList::SortedFace::SortedFace(const TGen::Face * face, scalar distance)
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

