/*
 *  md5tokenizer.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5TOKENIZER_H
#define _TGEN_RENDERER_MD5TOKENIZER_H

#include <tgen_core.h>

namespace TGen {
	namespace MD5 {
		enum Tokens {
			TokenBlockStart = 20,
			TokenBlockEnd = 21,
			TokenEOL = 22,	// not used
			TokenVersion = 23,
			TokenNumJoints = 24,
			TokenNumMeshes = 25,
			TokenMesh = 26,
			TokenJoints = 27,
			TokenArrayStart = 28,
			TokenArrayEnd = 29,
		};
		
		class Tokenizer : public TGen::Tokenizer {
		public:
			Tokenizer();
		};
		
	} // !MD5
} // !TGen

#endif // !_TGEN_RENDERER_MD5TOKENIZER_H
