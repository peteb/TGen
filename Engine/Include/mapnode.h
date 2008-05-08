/*
 *  mapnode.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MAPNODE_H
#define _TGEN_ENGINE_MAPNODE_H

#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		class Map;
		class MapModel;
		class MapLeafNode;
		
		class MapLinkNode {
		public:
			MapLinkNode(const TGen::Plane3 & plane);
			~MapLinkNode();
			
			void setPosChild(int posChild);
			void setNegChild(int negChild);
			void setPosLeaf(TGen::Engine::MapLeafNode * pos);
			void setNegLeaf(TGen::Engine::MapLeafNode * neg);
			
			void link(TGen::Engine::Map * map);

			TGen::Plane3 plane;
			TGen::Engine::MapLinkNode * pos, * neg;
			TGen::Engine::MapLeafNode * posLeaf, * negLeaf;
			
		private:
			int posChild, negChild;
		};
		
		class MapLeafNode {
		public:
			MapLeafNode(int areaNumber);
			~MapLeafNode();
			
			void link(TGen::Engine::Map * map);
			const TGen::Engine::MapModel * getModel() const;
			TGen::Engine::MapModel * getModel();

			TGen::Engine::MapModel * area;

		private:
			int areaNumber;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAPNODE_H

