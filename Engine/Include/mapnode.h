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
		
		class MapNode {
		public:	
			virtual ~MapNode() {}
			
			virtual void link(TGen::Engine::Map * map) abstract;
		};
		
		class MapLinkNode : public TGen::Engine::MapNode {
		public:
			MapLinkNode(const TGen::Plane3 & plane);
			~MapLinkNode();
			
			void setPosChild(int posChild);
			void setPosChild(TGen::Engine::MapNode * posChild);
			void setNegChild(int negChild);
			void setNegChild(TGen::Engine::MapNode * negChild);
			void link(TGen::Engine::Map * map);

			TGen::Plane3 plane;
			TGen::Engine::MapNode * pos, * neg;

		private:
			int posChild, negChild;
		};
		
		class MapLeafNode : public TGen::Engine::MapNode {
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

