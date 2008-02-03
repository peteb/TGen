/*
 *  md3animmesh.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3ANIMMESH_H
#define _TGEN_RENDERER_MD3ANIMMESH_H

#include "md3struct.h"

namespace TGen {
	class VertexData;
	
	namespace MD3 {
		class AnimationFrame {
		public:
			AnimationFrame(const std::string & name);
			
			typedef std::vector<TGen::MD3::FrameVertexDecl::Type> VertexList;

			std::string name;
			float radius;
			TGen::Vector3 min, max, origin;
			
			VertexList vertices;
		};
		
		class AnimatingMesh {
		public:
			AnimatingMesh(const std::string & materialName);
			~AnimatingMesh();
			
			void addAnimationFrame(TGen::MD3::AnimationFrame * frame);
			std::string getMaterialName() const;
			int getNumAnimationFrames() const;
			TGen::MD3::AnimationFrame const & getAnimationFrame(int num) const;
			
		//private:
			typedef std::vector<TGen::MD3::IndexDecl::Type> IndexList;
			typedef std::vector<TGen::MD3::AnimationFrame *> FrameList;
			typedef std::vector<TGen::MD3::TexCoordDecl::Type> TexCoordList;
			
			uint vertexCount;
			IndexList indices;
			TexCoordList texcoords;
			FrameList frames;
			std::string materialName;
		};
		
	} // !MD3	
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMESH_H
