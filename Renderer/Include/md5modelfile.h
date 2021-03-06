/*
 *  md5modelfile.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Renamed by Peter Backman on 2/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5MODELFILE_H
#define _TGEN_RENDERER_MD5MODELFILE_H

#include <tgen_core.h>
#include <tgen_math.h>
#include <istream>
#include <vector>

namespace TGen {
	class VertexDataSource;
	
	namespace MD5 {
		class ModelParser;
		class FileMesh;
		class Mesh;
		class Model;
		
		struct FileVertex {
			float s, t;
			int startWeight, weightCount;
		};
		
		struct FileTriangle {
			int indices[3];
		};
		
		struct FileWeight {
			int joint;
			float bias;
			TGen::Vector3 position;
		};
		
		struct FileJoint {
			std::string name;
			int parent;
			TGen::Vector3 position;
			TGen::Quaternion4 orientation;
		};
		
		class ModelFile {
			ModelFile();

		public:
			~ModelFile();
			
			void printInfo(std::ostream & stream) const;
			TGen::MD5::Model * createModel(TGen::VertexDataSource & dataSource, const std::string & name, scalar scale) const;
			
			friend class TGen::MD5::ModelParser;
			
		private:
			void setVersion(int version);
			void setNumMeshes(int numMeshes);
			void setNumJoints(int numJoints);
			void addMesh(FileMesh * mesh);
			
			int version, numMeshes, numJoints;
			
			std::vector<TGen::MD5::FileJoint *> joints;
			std::vector<TGen::MD5::FileMesh *> meshes;
		};
		
		class FileMesh {
			FileMesh();
			
		public:	
			~FileMesh();
			
			void printInfo(std::ostream & stream) const;
			const std::vector<TGen::MD5::FileVertex> & getVertices() const;
			const std::vector<TGen::MD5::FileTriangle> & getTriangles() const;
			const std::vector<TGen::MD5::FileWeight> & getWeights() const;
			
			std::string getMaterialName() const;
			
			friend class TGen::MD5::ModelParser;
			
		private:
			void setMaterial(const std::string & material);
			void setNumVerts(int numVerts);
			void setNumTris(int numTris);
			void setNumWeights(int numWeights);
			void setVertices(const std::vector<TGen::MD5::FileVertex> & vertices);
			void setTriangles(const std::vector<TGen::MD5::FileTriangle> & triangles);
			void setWeights(const std::vector<TGen::MD5::FileWeight> & weights);
			
			int numVerts, numTris, numWeights;
			std::string material;
			
			std::vector<TGen::MD5::FileVertex> vertices;
			std::vector<TGen::MD5::FileTriangle> triangles;
			std::vector<TGen::MD5::FileWeight> weights;
		};
		
	} // !MD5
} // !TGen

#endif // !_TGEN_RENDERER_MD5MODELFILE_H
