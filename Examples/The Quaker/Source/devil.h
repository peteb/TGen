/*
 *  devil.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_THEQUAKER_DEVIL_H
#define _TGEN_THEQUAKER_DEVIL_H

#include <tgen_graphics.h>
#include <IL/IL.h>

class DevilImage : public TGen::Image {
public:
	DevilImage(ILuint imageName);
	~DevilImage();
	
	TGen::Rectangle getSize() const;
	TGen::ImageFormat getFormat() const;
	TGen::FormatType getComponentFormat() const;
	const void * getData() const;
	
private:
	ILuint imageName;
};


#endif // !_TGEN_THEQUAKER_DEVIL_H
