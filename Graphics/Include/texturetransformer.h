/*
 *  texturetransformer.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_TEXTURETRANSFORMER_H
#define _TGEN_GRAPHICS_TEXTURETRANSFORMER_H

#include <tgen_math.h>

namespace TGen {
	class ScalarGenerator;
	
	class TextureTransformer {
	public:
		TextureTransformer(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool centered);
		virtual ~TextureTransformer();
		
		virtual void applyTransform(TGen::Matrix4x4 & matrix, scalar time) abstract;
		bool shouldCenter() const;
		
	protected:
		scalar startedAt;
		bool centered;

		TGen::ScalarGenerator * genU, * genV;
	};
	
	
	
	class TextureTranslate : public TGen::TextureTransformer {
	public:
		TextureTranslate(float u, float v, bool scroll = false);
		TextureTranslate(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool scroll = false);
		
		void applyTransform(TGen::Matrix4x4 & matrix, scalar time);
		void setCoord(float u, float v);
		
	private:
		float u, v;
		bool scroll; 
	};
	
	class TextureScale : public TGen::TextureTransformer {
	public:
		TextureScale(float u, float v, bool centered = false);
		TextureScale(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool centered = false);
		
		void applyTransform(TGen::Matrix4x4 & matrix, scalar time);
		void setScale(float u, float v);
		
	private:
		float u, v;
	};	
	
	class TextureRotate : public TGen::TextureTransformer {
	public:
		TextureRotate(float speed, bool centered = false);
		TextureRotate(TGen::ScalarGenerator * genRot, bool centered = false);
		
		void applyTransform(TGen::Matrix4x4 & matrix, scalar time);
		void setSpeed(float speed);
		
	private:
		float speed;
	};
	
} // !TGen

#endif // !_TGEN_GRAPHICS_TEXTURETRANSFORMER_H
