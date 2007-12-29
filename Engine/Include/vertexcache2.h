/*
 *  vertexcache2.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_VERTEXCACHE2_H
#define _TGEN_ENGINE_VERTEXCACHE2_H

#include <tgen_core.h>
#include <tgen_graphics.h>

namespace TGen {
	class VertexStructure;
	class VertexData;
	
	namespace Engine {
		class PoolCollection2;
		class StandardLogs;
		
		class VertexCache2 : public TGen::VertexDataSource {
		public:
			VertexCache2(TGen::VertexDataSource & dataSource, TGen::Engine::StandardLogs & logs);
			~VertexCache2();
			
			TGen::VertexData * createVertexData(const TGen::VertexStructure & vertstruct, uint vertices, ushort usage);

		private:
			std::string getPoolId(const TGen::VertexStructure & vertstruct, ushort usage) const;
			void removeVertexData(TGen::VertexData * data);
			TGen::Engine::PoolCollection2 * getPoolCollection(const TGen::VertexStructure & vertstruct, ushort usage);
			TGen::Engine::PoolCollection2 * createPoolCollection(const TGen::VertexStructure & vertstruct, ushort usage);
			
			typedef std::map<std::string, TGen::Engine::PoolCollection2 *> PoolCollectionMap;
			
			PoolCollectionMap poolCollections;
			TGen::VertexDataSource & dataSource;
			TGen::Engine::StandardLogs & logs;
		};
		
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VERTEXCACHE2_H
