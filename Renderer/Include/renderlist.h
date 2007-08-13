/*
 *  renderlist.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_RENDERLIST_H
#define _TGEN_RENDERER_RENDERLIST_H

#include <vector>

namespace TGen {
	class Face;
	class Camera;
	class Renderer;
	
	class RenderList {
	public:	
		RenderList();
		~RenderList();
		
		void render(TGen::Renderer & renderer, const TGen::Camera & camera);
		void addFace(const TGen::Face * face);
		void clear();
		void sort(const TGen::Vector3 & position);
		void print();
		
	private:
		typedef std::vector<const TGen::Face *> FaceList;
		
		FaceList opaqueFaces, transparentFaces;
	};
	
	
	class Sorter {
	public:	
		Sorter(const TGen::Vector3 & position);
		
		bool operator() (const TGen::Face * face1, const TGen::Face * face2);
		
	private:
		TGen::Vector3 position;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_RENDERLIST_H
