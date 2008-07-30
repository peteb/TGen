/*
 *  objectreference.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_OBJECTREFERENCE_H
#define _TGEN_ENGINE_OBJECTREFERENCE_H

#include "resourcecomponent.h"
#include "componentlink.h"
#include "triggerable.h"

namespace TGen {
	namespace Engine {
		class ObjectReference : public TGen::Engine::ResourceComponent {
		public:
			ObjectReference(const std::string & name);
			
			void * getData() const;
			void link(const TGen::Engine::ComponentLinker & linker);
			void setObjectName(const std::string & name);
			
		private:
			TGen::Engine::UnaryDelegate<TGen::Engine::Triggerable> reference;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_OBJECTREFERENCE_H

