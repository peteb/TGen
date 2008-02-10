/*
 *  md5modelparser.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Renamed by Peter Backman on 2/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5MODELPARSER_H
#define _TGEN_RENDERER_MD5MODELPARSER_H

#include <tgen_core.h>

namespace TGen {
	namespace MD5 {
		class ModelFile;
		class FileMesh;
		
		class ModelParser {
		public:
			ModelParser();
			~ModelParser();
			
			TGen::MD5::ModelFile * parse(TGen::InputStream & source);
			
		private:
			TGen::MD5::ModelFile * parseGlobalBlock();
			TGen::MD5::FileMesh * parseMeshBlock();
			void parseJointsBlock(TGen::MD5::ModelFile & file);
			
			void step();
			void checkEOS();			
			void stepAndCheck();
			
			TGen::TokenStream tokens;
			TGen::TokenStream::TokenList::iterator currentToken, endIter;
		};
		
		
	} // !MD5	
} // !TGen

#endif // !_TGEN_ENGINE_MD5MODELPARSER_H
