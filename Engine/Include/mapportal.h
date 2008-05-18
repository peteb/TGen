/*
 *  mapportal.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/8/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MAPPORTAL_H
#define _TGEN_ENGINE_MAPPORTAL_H

#include <vector>
#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		class Map;
		class MapModel;
		
		class MapPortal {
		public:
			MapPortal(int posAreaId, int negAreaId);
			
			void addPoint(const TGen::Vector3 & point);
			
			int getNumPoints() const;
			const TGen::Vector3 & getPoint(int num) const;
			const TGen::Vector3 * getPoints() const;
			
			void link(TGen::Engine::Map * map);
			
			const TGen::Plane3 & getPlane() const;
			TGen::Engine::MapModel * getPosArea();
			TGen::Engine::MapModel * getNegArea();
			bool open;
			
		private:
			std::vector<TGen::Vector3> points;
			int posAreaId, negAreaId;
			TGen::Engine::MapModel * posArea, * negArea;
			TGen::Plane3 plane;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAPPORTAL_H

