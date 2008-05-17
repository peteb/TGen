/*
 *  id4cmtokenizer.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ID4CMTOKENIZER_H
#define _TGEN_ENGINE_ID4CMTOKENIZER_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		namespace Physics {
			enum Id4CMTokens {
				Id4CMTokenBlockStart = 20,
				Id4CMTokenBlockEnd = 21,
				Id4CMTokenEOL = 22,	// not used
				Id4CMTokenArrayStart = 23,
				Id4CMTokenArrayEnd = 24,
				Id4CMTokenIdent = 25,
				Id4CMTokenCollisionModel = 26,
				Id4CMTokenVertices = 27,
				Id4CMTokenEdges = 28,
				Id4CMTokenNodes = 29,
				Id4CMTokenPolygons = 30,
				Id4CMTokenBrushes = 31,
				
			};
						
			class Id4CMTokenizer : public TGen::Tokenizer {
			public:
				Id4CMTokenizer();
				
				int getSpecialToken(char * text, TGen::TokenStream & stream);				
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ID4CMTOKENIZER_H

