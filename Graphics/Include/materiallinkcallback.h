/*
 *  materiallinkcallback.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_MATERIALLINKCALLBACK_H
#define _TGEN_GRAPHICS_MATERIALLINKCALLBACK_H

namespace TGen {
	class ShaderProgram;
	class Texture;
	
	class MaterialLinkCallback {
	public:
		virtual ~MaterialLinkCallback() {}
		
		virtual TGen::ShaderProgram * getShaderProgram(const std::string & name) abstract;
		virtual TGen::Texture * getTexture(const std::string & name) abstract;
		virtual int getTextureType(const std::string & name) abstract;
	};	
} // !TGen

#endif // !_TGEN_GRAPHICS_MATERIALLINKCALLBACK_H
