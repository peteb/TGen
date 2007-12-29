/*
 *  maptokenizer.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/29/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MAPTOKENIZER_H
#define _TGEN_ENGINE_MAPTOKENIZER_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		enum ProcTokens {
			ProcTokenBlockStart = 20,
			ProcTokenBlockEnd = 21,
			ProcTokenEOL = 22,	// not used
			ProcTokenArrayStart = 23,
			ProcTokenArrayEnd = 24,
			ProcTokenIdent = 25,
			ProcTokenModel = 26,
			ProcTokenIAP = 27,
			ProcTokenNodes = 28,
		};
		
		class MapTokenizer : public TGen::Tokenizer {
		public:
			MapTokenizer();
			
			int getSpecialToken(char * text, TGen::TokenStream & stream);
		};
	} // !Engine
} // !Engine

#endif // !_TGEN_ENGINE_MAPTOKENIZER_H
