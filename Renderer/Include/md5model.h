/*
 *  md5model.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Renamed by Peter Backman on 2/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5MODEL_H
#define _TGEN_RENDERER_MD5MODEL_H

#include "model_new.h"
#include <tgen_graphics.h>
#include <vector>

namespace TGen {
	class VertexDataSource;

	namespace MD5 {
		class Mesh;
		enum {
			SurfaceTangentAttribute = 10,
		};
		
		class Model : public TGen::NewModel {
		public:
			Model(const std::string & name, TGen::VertexDataSource & dataSource);
			~Model();
			
			void addMeshData(TGen::MD5::Mesh * mesh);
			
			/*void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			void update(const TGen::Camera & camera, scalar distance, scalar time);
			
			TGen::Vector3 getMax() const;
			TGen::Vector3 getMin() const;
			TGen::Vector3 getOrigin() const;
			
			std::string getDefaultMaterial() const;*/
			
			TGen::NewModelInstance * instantiate(TGen::VertexDataSource & source);
			bool isPureInstance() const;
		
		private:
			typedef std::vector<TGen::MD5::Mesh *> MeshList;
			
			TGen::VertexDataSource & dataSource;
			MeshList meshes;
		};
		
		typedef TGen::Vertex3<float> Vertex;
		typedef TGen::TexCoord2<float, 0> TexCoord;
		typedef TGen::Normal3<float> Normal;
		typedef TGen::VertexAttribute4<TGen::MD5::SurfaceTangentAttribute, float> Tangent;				// TODO: SurfaceTangentAttribute might be unsycned with engine
		
		typedef TGen::JoinVertexElements4<Vertex, TexCoord, Normal, Tangent> VertexDecl;
		typedef TGen::Index<unsigned int> IndexDecl;
	} // !MD5	
} // !TGen

#endif // !_TGEN_RENDERER_MD5MODEL_H
