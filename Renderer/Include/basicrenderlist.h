/*
 *  basicrenderlist.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// A basic unoptimized transparent-sorted render list.
// addFace adds a face to the faces list, which on sort() is sorted into two other lists; opaqueFaces
// and transparentFaces. transparentFaces will be sorted back-to-front, opaqueFaces won't be sorted.
// Whether a face needs to be sorted is determined by looking at the sort level of the current lod 
// of the face's material.
//
// algorithm: std::sort
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_BASICRENDERLIST_H
#define _TGEN_RENDERER_BASICRENDERLIST_H

#include "renderlist.h"
#include "face_new.h"

namespace TGen {
	class BasicRenderList : public TGen::RenderList  {
	public:
		BasicRenderList();
		~BasicRenderList();
		
		void addMeta(TGen::MetaWriter * metaWriter, const TGen::SceneNode * node);
		void writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream);
		void render(TGen::Renderer & renderer, const TGen::Matrix4x4 & baseMat, const TGen::LodInfo & lod, const std::string & specialization);
		void renderWithinRadius(TGen::Renderer & renderer, const TGen::Matrix4x4 & baseMat, const TGen::LodInfo & lod, const TGen::Vector3 & pos, scalar radius);
		void addFace(const TGen::NewFace & face);
		void addUser(void * user, int id);
		void sort(const TGen::Camera & camera, const std::string & specialization);
		bool needSorting();
		void clear();
		void print();
		int getNumUserInfo();
		int getNumFaces();
		UserInfo & getUserInfo(int id);
		
	private:			
		class SortedFace {
		public:
			SortedFace(const TGen::NewFace & face, scalar distance = 0.0f);
				
			TGen::NewFace face;
			scalar distanceToCamera;
		};
		
		class Sorter {
		public:	
			Sorter();
				
			bool operator() (const TGen::BasicRenderList::SortedFace & face1, const TGen::BasicRenderList::SortedFace & face2);
		};

		struct Meta {
			Meta(TGen::MetaWriter * writer, const TGen::SceneNode * node)
				: writer(writer)
				, node(node) {}
			
			TGen::MetaWriter * writer;
			const TGen::SceneNode * node;
		};
		
		typedef std::vector<SortedFace> SortedFaceList;
		typedef std::vector<NewFace> FaceList;
		typedef std::vector<UserInfo> UserInfoList;
		
		void renderList(SortedFaceList & list, TGen::Renderer & renderer, const TGen::Matrix4x4 & baseMat, const TGen::LodInfo & lod, const std::string & specialization);
		void calculateCameraDistance(SortedFaceList & list, const TGen::Camera & camera);
		
		UserInfoList userInfo;
		FaceList faces;
		SortedFaceList opaqueFaces, transparentFaces;	// opaque faces get distance to camera too, but it's not calculated
		
		std::vector<Meta> metaWriters;
	}; 
	
	
} // !TGen

#endif // !_TGEN_RENDERER_BASICRENDERLIST_H
