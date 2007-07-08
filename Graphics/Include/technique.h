/*
 *  technique.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_TECHNIQUE_H
#define _TGEN_TECHNIQUE_H

#include <vector>
#include "tgen_math.h"

namespace TGen {
	class PassList;
	class MaterialLinkCallback;
	
	class Technique {
	public:
		Technique();
		~Technique();
	
		void setPassList(PassList * pass, int lod);
		PassList * getPassList(int lod);
		void Link(TGen::MaterialLinkCallback & callback);
		void Update(scalar time);
		
	private:
		typedef std::vector<PassList *> PassVector;
		PassVector lods;
	};
	
	class TechniqueList {
	public:
		TechniqueList();
		~TechniqueList();	
		
		void setTechnique(Technique * tech, int reqs);
		Technique * getTechnique(int minreqs);
		void Link(TGen::MaterialLinkCallback & callback);
		void Update(scalar time);
		
	private:
		typedef std::vector<Technique *> TechniqueVector;
		TechniqueVector techniques;
	};
	
} // !TGen

#endif // !_TGEN_TECHNIQUE_H
