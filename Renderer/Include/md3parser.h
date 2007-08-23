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

namespace TGen {
	class Mesh;
	
	namespace MD3 {
		class File;
		struct Header;
		
		class Parser {
		public:	
			Parser();
			~Parser();
		
			File * parse(std::istream & source);
		};
		
		
		class File {
		private:
			File(TGen::MD3::Header * header);
			
		public:
			~File();
			
			void printInfo(std::ostream & stream) const;
			TGen::Mesh * createMesh(scalar scale) const;
			
			friend class TGen::MD3::Parser;
			
		private:
			TGen::MD3::Header * header;
		};
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3PARSER_H

