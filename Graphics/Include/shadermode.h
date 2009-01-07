/*
 *  shadermode.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_SHADERMODE_H
#define _TGEN_GRAPHICS_SHADERMODE_H

#include <string>
#include <vector>

namespace TGen {
	class MaterialLinkCallback;
	class ShaderUpdater;
	class ShaderProgram;
	
	class ShaderMode {
	public:
		ShaderMode(const std::string & name);
		
		void link(TGen::MaterialLinkCallback & callback);
		void update();
		void addShaderUpdater(TGen::ShaderUpdater * updater);
		
		typedef std::vector<TGen::ShaderUpdater *> ShaderUpdaterList;
		
		
		ShaderUpdaterList shaderUpdaters;
		std::string name;
		TGen::ShaderProgram * shader;
	};
	
} // !TGen

#endif // !_TGEN_GRAPHICS_SHADERMODE_H
