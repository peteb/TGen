/*
 *  md3model.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Renamed by Peter Backman on 2/3/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3MODEL_H
#define _TGEN_RENDERER_MD3MODEL_H

#include "model_new.h"
#include <tgen_graphics.h>

namespace TGen {
	namespace MD3 {
		class Model : public TGen::NewModel {
		public:	
			Model(const std::string & name);		
			~Model();

			//void preRender(TGen::Renderer & renderer) const;
			//void render(TGen::Renderer & renderer) const;
			//void update(const TGen::Camera & camera, scalar distance, scalar time);
			
		//	TGen::Vector3 getMax() const;
			//TGen::Vector3 getMin() const;
			//TGen::Vector3 getOrigin() const;
			
	//		std::string getDefaultMaterial() const;
			
			void addJoint(const std::string & name, const TGen::ModelJoint & joint);
			TGen::ModelJoint getJoint(const std::string & name) const;
			bool isPureInstance() const;
			TGen::MD3::Model * instantiate();
			
		private:
			typedef std::map<std::string, TGen::ModelJoint> JointMap;
			JointMap joints;
		};
		
		
		
		typedef TGen::JoinVertexElements3<TGen::Vertex3<float>, TGen::Normal3<float>, TGen::TexCoord2<float, 0> > VertexDecl;
		typedef TGen::Index<unsigned int> IndexDecl;
		
	} // !MD3	
} // !TGen

#endif // !_TGEN_RENDERER_MD3MODEL_H

