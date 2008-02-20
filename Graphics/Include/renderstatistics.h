/*
 *  rendererstatistics.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 12/26/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERERSTATISTICS_H
#define _TGEN_RENDERERSTATISTICS_H

#include <tgen_core.h>

namespace TGen {
	enum RenderStats {
		StatVBCacheHit = 0,
		StatVBCacheMiss = 1,
		StatIBCacheHit = 2,
		StatIBCacheMiss = 3,
		StatGeneralStateCacheHit = 4,
		StatGeneralStateCacheMiss = 5,
		StatTextureCacheHit = 6,
		StatTextureCacheMiss = 7,
		StatShaderCacheHit = 8,
		StatShaderCacheMiss = 9,
		StatBusUpTexture = 15,
		StatBusUpBuffers = 16,
		StatBusDownTexture = 17,
		StatBusDownBuffers = 18,
	};
	
	enum StaticRenderStats {
		StatTextureAlloc = 0,
		StatBufferAlloc = 1,
	};
	
	class ShaderProgram;
	class Texture;
	
	class RenderStatistics {
	public:
		RenderStatistics();

		void reset();
		void addStat(RenderStats stat, int val = 1);
		void addPStat(StaticRenderStats stat, int val = 1);
		
		void addShader(TGen::ShaderProgram * shader);
		void addTexture(int unit, TGen::Texture * texture);
		
		operator std::string() const;
		
	private:
		int getUniqueEntries(const std::vector<void *> & values) const;
		
		uint stats[20], pstats[5];
		
		std::vector<void *> shaders;
		std::vector<void *> textures[8];
	};
	
	
	
} // !TGen

#endif // !_TGEN_RENDERERSTATISTICS_H
