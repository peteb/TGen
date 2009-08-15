/*
 *  q3bsp.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/15/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_Q3BSP_Q3BSP_H
#define _TGEN_ENGINE_Q3BSP_Q3BSP_H

namespace TGen {
	namespace Engine {
		namespace Q3Bsp {
			enum Lumps {
				LumpTextures = 1,
				LumpModels = 7,
				LumpVertices = 10,
				LumpMeshverts = 11,
				LumpEffects = 12,	
				LumpFaces = 13,
				LumpNodes = 3,
				LumpPlanes = 2,
				LumpLeaves = 4,
				LumpLeafFaces = 5,
				LumpEntities = 0,
			};
			
			enum FaceType {
				FacePolygon = 1,
				FacePatch = 2,
				FaceMesh = 3,
				FaceBillboard = 4,	
			};
			
			struct Texture {
				char name[64];
				int flags, contents;		
			};
			
			struct Effect {
				char name[64];
				int brush, unknown;		
			};
			
			struct Model {
				float bbmin[3];
				float bbmax[3];
				int face, num_faces, brush, num_brushes;		
			};
			
			struct DirectoryEntry {
				int offset, length;
			};
			
			struct Face {
				int texture;
				int effect;
				int type;
				int vertex;
				int num_vertices;
				int meshvert;
				int num_meshverts;
				int lm_index;
				int lm_start[2];
				int lm_size[2];
				float lm_origin[3];
				float lm_vecs[2][3];
				float normal[3];
				int size[2];				
			};
			
			struct Vertex {
				float position[3];
				float texcoord[2][2];
				float normal[3];
				unsigned char color[4];
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
			
			struct Header {
				char magic[4];
				int version;
				DirectoryEntry entries[17];
			};			
		}
		
		struct Q3BspFile {
			Q3BspFile() {
				faces = NULL;
				vertices = NULL;
				meshverts = NULL;
				node = NULL;
				planes = NULL;
				leaves = NULL;
				leafFaces = NULL;
				models = NULL;
				textures = NULL;
			}
			
			~Q3BspFile() {
				delete [] faces;
				delete [] vertices;
				delete [] meshverts;
				delete [] node;
				delete [] planes;
				delete [] leaves;
				delete [] leafFaces;
				delete [] models;
				delete [] textures;
			}
			
			Q3Bsp::Header header;
			Q3Bsp::Face * faces;
			Q3Bsp::Vertex * vertices;
			Q3Bsp::Meshvert * meshverts;
			Q3Bsp::Node * node;
			Q3Bsp::Plane * planes;
			Q3Bsp::Leaf * leaves;
			Q3Bsp::LeafFace * leafFaces;
			Q3Bsp::Model * models;
			Q3Bsp::Texture * textures;
			
			int numTextures;
			int numModels;
			int numFaces;
			int numVertices;
			int numMeshverts;
			int numNodes;
			int numPlanes;
			int numLeaves;
			int numLeafFaces;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_Q3BSP_Q3BSP_H

