/*
 *  shader.h
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_SHADER_H
#define _TGEN_SHADER_H

namespace TGen {
	class Renderer;
	
	class Shader {
	public:	
		Shader(TGen::Renderer & creator);
		virtual ~Shader() {}
		
	private:
		TGen::Renderer & creator;
	};
	
} // !TGen

#endif // !_TGEN_SHADER_H
