/*
 *  bsploader.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_BSPLOADER_H
#define _THEQUAKER_BSPLOADER_H

#include "bsp.h"
#include <tgen_core.h>
#include <list>

enum BSPLumps {
	BSPLumpTextures = 1,
	BSPLumpModels = 7,
	BSPLumpVertices = 10,
	BSPLumpMeshverts = 11,
	BSPLumpEffects = 12,	
	BSPLumpFaces = 13,
	BSPLumpNodes = 3,
	BSPLumpPlanes = 2,
	BSPLumpLeaves = 4,
	BSPLumpLeafFaces = 5,
	BSPLumpEntities = 0,
};

enum BSPFaceType {
	BSPFacePolygon = 1,
	BSPFacePatch = 2,
	BSPFaceMesh = 3,
	BSPFaceBillboard = 4,	
};



class BSPLoader {
public:
	typedef std::list<std::string> StringList;

	BSPLoader();
	~BSPLoader();
	
	void Parse(std::ifstream & file);
	BSPTree * CreateTree(TGen::Renderer & renderer, SurfaceLinker & linker);
	
	void CreateNodes(TGen::Renderer & renderer, BSPNode * node, int nodeIndex, SurfaceLinker & linker);
	void CreateLeaf(TGen::Renderer & renderer, BSPLeaf * leaf, BSPNode * node, int leafIndex, SurfaceLinker & linker);
	void CreateFace(TGen::Renderer & renderer, BSPLeaf * leaf, BSPNode * node, int faceIndex, SurfaceLinker & linker);
	void CreateFaces(TGen::Renderer & renderer, BSPNode * node);
	
	void ReadHeader(std::ifstream & file);
	void ReadTextures(std::ifstream & file);
	void ReadFaces(std::ifstream & file);
	void ReadVertices(std::ifstream & file);
	void ReadMeshverts(std::ifstream & file);
	void ReadNodes(std::ifstream & file);
	void ReadPlanes(std::ifstream & file);
	void ReadLeaves(std::ifstream & file);
	void ReadLeafFaces(std::ifstream & file);
	void ReadEntities(std::ifstream & file);
	
	const StringList & getTextures() const;
	
	TGen::VertexBuffer * globalVB;
	
private:
	typedef TGen::JoinVertex5<TGen::Vertex3<float>, TGen::TexCoord2<float, 0>, TGen::TexCoord2<float, 1>, TGen::Normal3<float>, TGen::Color4<uint8> > MyVertex;
	typedef TGen::Index<uint> MyIndex;
	typedef std::map<std::string, std::vector<MyIndex::Type> > IndexMap;
	typedef std::map<int, bool> FaceAddedMap;

	FaceAddedMap facesAdded;
	StringList materialDeps;
	
	BSPGeometry ** loadedFaces;
	
	TGen::Vector3 getQuadratic(const TGen::Vector3 & p0, const TGen::Vector3 & p1, const TGen::Vector3 & p2, scalar t);
	const scalar SCALE;
	
	struct Face {
		int texture, effect, type, vertex, num_vertices, meshvert, num_meshvertices, lm_index;
		int lm_start[2], lm_size[2];
		float lm_origin[3], lm_vecs[2][3], normal[3];
		int size[2];		
	};
	
	struct Meshvert {
		int offset;		
	};
	
	struct Node {
		int plane;
		int children[2];
		int mins[3];
		int maxs[3];
	};
	
	struct Leaf {
		int cluster, area;
		int mins[3], maxs[3];
		int leafface;
		int n_leaffaces;
		int leafbrush;
		int n_leafbrushes;
	};
	
	struct Plane {
		float normal[3];
		float dist;
	};
	
	struct LeafFace {
		int face;
	};
	
	struct Vertex {
		float position[3];
		float texcoord[2][2];
		float normal[3];
		uint8 color[4];
		
		Vertex operator * (scalar value) {
			Vertex ret;
			ret = *this;
			ret.position[0] = position[0] * value;
			ret.position[1] = position[1] * value;
			ret.position[2] = position[2] * value;
			ret.texcoord[0][0] = texcoord[0][0] * value;
			ret.texcoord[0][1] = texcoord[0][1] * value;
			
	//		for (int i = 0; i < 4; ++i)
	//			ret.color[i] = color[i] * value;
			
			return ret;
		}
		
		Vertex operator + (const Vertex & vert) {
			Vertex ret;
			ret = vert;
			ret.position[0] = position[0] + vert.position[0];
			ret.position[1] = position[1] + vert.position[1];
			ret.position[2] = position[2] + vert.position[2];
			ret.texcoord[0][0] = texcoord[0][0] + vert.texcoord[0][0];
			ret.texcoord[0][1] = texcoord[0][1] + vert.texcoord[0][1];
			
			for (int i = 0; i < 4; ++i) {
				ret.color[i] = vert.color[i] + (color[i] - vert.color[i]) * 0.5;
			}
			
			return ret;
		}
		
	};
	
	struct Texture {
		char name[64];
		int flags, contents;		
	};	
	
	struct DirectoryEntry {
		int32 offset, length;
	};
	
	struct Header {
		uint8 magic[4];
		int32 version;
		DirectoryEntry entries[17];
	};

	class Bezier {
	public:
		Vertex controls[9];
		
		void Tessellate(int level);
	
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;
		std::vector<int32> trianglesPerRow;
		std::vector<uint32 *> rowIndices;
		
	};
	
	Header header;
	Texture * textures;
	Face * faces;
	Vertex * vertices;
	Meshvert * meshverts;
	Node * nodes;
	Plane * planes;
	Leaf * leaves;
	LeafFace * leafFaces;
	char * entities;
	
	int numTextures, numFaces, numVertices, numMeshverts, numNodes, numPlanes, numLeaves, numLeafFaces;
};

#endif // !_THEQUAKER_BSPLOADER_H
