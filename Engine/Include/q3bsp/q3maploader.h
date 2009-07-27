/*
 *  q3maploader.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_Q3BSP_Q3MAPLOADER_H
#define _TGEN_ENGINE_Q3BSP_Q3MAPLOADER_H

#include <string>

namespace TGen {
	class VertexTransformer;
	class InputStream;
	
	namespace Engine {
		class StandardLogs;
		class Q3Map;
		
		class Q3MapLoader {
		public:	
			Q3MapLoader(TGen::Engine::StandardLogs & logs);
			
			TGen::Engine::Q3Map * createMap(const std::string & name, TGen::InputStream & source, const TGen::VertexTransformer & transformer);
			
			
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
			
			struct Header {
				char magic[4];
				int version;
				DirectoryEntry entries[17];
			};

			
		private:
			TGen::Engine::StandardLogs & logs;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_Q3BSP_Q3MAPLOADER_H

