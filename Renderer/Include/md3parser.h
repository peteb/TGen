/*
 *  md3parser.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3PARSER_H
#define _TGEN_RENDERER_MD3PARSER_H

#include <istream>
#include <tgen_core.h>
#include <vector>
#include "md3struct.h"

namespace TGen {
	class Mesh;
	class VertexDataSource;
	class NewModel;
	
	namespace MD3 {
		class File;
		class StaticModel;
		class AnimatingModel;
		
		struct Header;
		
		class Parser {
		public:	
			Parser();
			~Parser();
		
			File * parse(TGen::InputStream & source);
		};
		
		
		class File {
		private:
			File(TGen::MD3::Header * header, const TGen::MD3::SurfaceList & surfaces);
			
		public:
			~File();
			
			void printInfo(std::ostream & stream) const;
			TGen::NewModel * createModel(TGen::VertexDataSource & dataSource, scalar scale) const;
			
			friend class TGen::MD3::Parser;
			
		private:
			TGen::MD3::StaticModel * createStaticModel(TGen::VertexDataSource & dataSource, scalar scale) const;
			TGen::MD3::AnimatingModel * createAnimatingModel(TGen::VertexDataSource & dataSource, scalar scale) const;
			static TGen::Vector3 normalToVector(TGen::MD3::S16 normal);
			
			TGen::MD3::Header * header;
			TGen::MD3::SurfaceList surfaces;
		};
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3PARSER_H

