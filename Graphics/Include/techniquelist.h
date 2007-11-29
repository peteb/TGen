/*
 *  techniquelist.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 8/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_TECHNIQUELIST_H
#define _TGEN_TECHNIQUELIST_H

#include <tgen_math.h>
#include <vector>

namespace TGen {
	class Technique;
	class MaterialLinkCallback;
	
	class TechniqueList {
	public:
		TechniqueList(const std::string & name);
		~TechniqueList();	
		
		void setTechnique(Technique * tech, int reqs);
		Technique * getTechnique(int minreqs);
		void link(TGen::MaterialLinkCallback & callback);
		void update(scalar time);
		const std::string & getName() const;
		int getTimesUsed() const;
		
	private:
		typedef std::vector<Technique *> TechniqueVector;
		TechniqueVector techniques;
		std::string name;
		int timesUsed;
	};	
	
} // !TGen

#endif // !_TGEN_TECHNIQUELIST_H
