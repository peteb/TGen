/*
 *  md5parser.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5PARSER_H
#define _TGEN_RENDERER_MD5PARSER_H

#include <tgen_core.h>

namespace TGen {
	namespace MD5 {
		class File;
		class FileMesh;
		
		class Parser {
		public:
			Parser();
			~Parser();
			
			TGen::MD5::File * parse(TGen::InputStream & source);
			
		private:
			TGen::MD5::File * parseGlobalBlock();
			TGen::MD5::FileMesh * parseMeshBlock();
			void parseJointsBlock(TGen::MD5::File & file);
			
			void step();
			void checkEOS();			
			void stepAndCheck();
			
			TGen::TokenStream tokens;
			TGen::TokenStream::TokenList::iterator currentToken, endIter;
		};
		
		
	} // !MD5	
} // !TGen

#endif // !_TGEN_ENGINE_MD5PARSER_H
