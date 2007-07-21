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
	
	void ReadHeader(std::ifstream & file);
	void ReadTextures(std::ifstream & file);
	void ReadFaces(std::ifstream & file);
	void ReadVertices(std::ifstream & file);
	void ReadMeshverts(std::ifstream & file);
	
	const StringList & getTextures() const;
	
private:
	typedef TGen::JoinVertex5<TGen::Vertex3<float>, TGen::TexCoord2<float, 0>, TGen::TexCoord2<float, 1>, TGen::Normal3<float>, TGen::Color4<uint8> > MyVertex;
	typedef TGen::Index<uint> MyIndex;
	
	StringList materialDeps;
	
	struct Face {
		int texture, effect, type, vertex, num_vertices, meshvert, num_meshvertices, lm_index;
		int lm_start[2], lm_size[2];
		float lm_origin[3], lm_vecs[2][3], normal[3];
		int size[2];		
	};
	
	struct Meshvert {
		int offset;		
	};
	
	struct Vertex {
		float position[3];
		float texcoord[2][2];
		float normal[3];
		uint8 color[4];
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

	Header header;
	Texture * textures;
	Face * faces;
	Vertex * vertices;
	Meshvert * meshverts;
	
	int numTextures, numFaces, numVertices, numMeshverts;
};

#endif // !_THEQUAKER_BSPLOADER_H
