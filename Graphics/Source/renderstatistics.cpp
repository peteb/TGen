/*
 *  renderstatistics.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 12/26/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderstatistics.h"
#include <iomanip>

TGen::RenderStatistics::RenderStatistics() {
	reset();
	
	for (int i = 0; i < 5; ++i)
		pstats[i] = 0;
}

void TGen::RenderStatistics::reset() {
	for (int i = 0; i < 20; ++i)
		stats[i] = 0;
	
	shaders.clear();
	
	for (int i = 0; i < 8; ++i)
		textures[i].clear();
}

void TGen::RenderStatistics::addStat(RenderStats stat, int val) {
	stats[stat] += val;
}

void TGen::RenderStatistics::addPStat(StaticRenderStats stat, int val) {
	pstats[stat] += val;
}

TGen::RenderStatistics::operator std::string() const {
	std::stringstream ss;
	
	std::string valueNames[20] = {
		"vbCache",
		"ibCache",
		"generalStateCache",
		"textureCache",
		"shaderCache"
	};
	
	/*
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
	 StatBusTexture = 15,
	 StatBusBuffers = 16,
	 */
	
	
	for (int i = 0; i < 5; ++i) {
		uint hits = stats[i * 2];
		uint misses = stats[i * 2 + 1];
		
		ss << valueNames[i] << ": " << hits << " hits, " << misses << " misses";
		ss << " (" << std::setprecision(2) << std::fixed << (float(hits) / float(hits + misses)) * 100.0f << "% hits)";
		
		if (i == 3) {
			int unique = 0;
			
			for (int i = 0; i < 8; ++i) {
				unique += getUniqueEntries(textures[i]);
			}

			uint calcMisses = unique;
			uint calcHits = (hits + misses) - calcMisses;
			ss << " calc best case: " << calcHits << " hits, " << calcMisses << " misses (" << (float(calcHits) / float(hits + misses)) * 100.0f << "% hits)";
		}
		else if (i == 4) {
			uint calcMisses = getUniqueEntries(shaders);
			uint calcHits = (hits + misses) - calcMisses;
			ss << " calc best case: " << calcHits << " hits, " << calcMisses << " misses (" << (float(calcHits) / float(hits + misses)) * 100.0f << "% hits)";
		}
		
		ss << "\n";
		
		ss.clear();
	}

	ss << "bus transfers (up/down):\n";
	ss << "   texture: " << TGen::getSize(stats[StatBusUpTexture]) << " / " << TGen::getSize(stats[StatBusDownTexture]) << "\n";
	ss << "   buffer: " << TGen::getSize(stats[StatBusUpBuffers]) << " / " << TGen::getSize(stats[StatBusDownBuffers]) << "\n";
	ss << "   total: " << TGen::getSize(stats[StatBusUpBuffers] + stats[StatBusUpTexture]) << " / " << TGen::getSize(stats[StatBusDownBuffers] + stats[StatBusDownTexture]) << "\n";
	
	ss << "allocated: \n";
	ss << "   textures: " << TGen::getSize(pstats[StatTextureAlloc]) << "\n";
	ss << "   buffers: " << TGen::getSize(pstats[StatBufferAlloc]) << "\n";
	ss << "   total: " << TGen::getSize(pstats[StatBufferAlloc] + pstats[StatTextureAlloc]) << "\n";
	
	return ss.str();
}

void TGen::RenderStatistics::addShader(TGen::ShaderProgram * shader) {
	shaders.push_back(shader);
}

void TGen::RenderStatistics::addTexture(int unit, TGen::Texture * texture) {
	textures[unit].push_back(texture);
}

int TGen::RenderStatistics::getUniqueEntries(const std::vector<void *> & values) const {
	int unique = 0;
	typedef std::map<void *, bool> FoundMap;
	FoundMap foundMap;
	
	for (int i = 0; i < values.size(); ++i) {
		if (foundMap.find(values[i]) == foundMap.end()) {
			unique++;
			foundMap.insert(FoundMap::value_type(values[i], true));
		}
	}
	
	return unique;
}

