/*
 *  materialrendermetadata.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_MATERIALRENDERMETADATA_H
#define _TGEN_GRAPHICS_MATERIALRENDERMETADATA_H

#include <string>

namespace TGen {
	class Texture;
	class ShaderVariableUpdater;
	class MaterialOverride;
	
	class MaterialRenderMetadata {
	public:
		MaterialRenderMetadata(const std::string & mode, int lod, TGen::Texture ** textureTypes, int shaderMode, TGen::ShaderVariableUpdater * varupdater = NULL, TGen::MaterialOverride * override = NULL);
		
		friend class Material;
		friend class PassList;
		
	private:
		std::string mode;
		int lod, shaderMode;
		TGen::Texture ** textureTypes;
		TGen::ShaderVariableUpdater * varupdater; 
		TGen::MaterialOverride * override;
	};
	
} // !TGen

#endif // !_TGEN_GRAPHICS_MATERIALRENDERMETADATA_H
