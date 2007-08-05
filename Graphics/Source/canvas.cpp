/*
 *  canvas.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "canvas.h"
#include "error.h"
#include "color.h"
#include <iostream>

TGen::Canvas::Canvas(const TGen::Rectangle & size, TGen::ImageFormat format, TGen::FormatType componentFormat) 
	: size(size)
	, format(format)
	, componentFormat(componentFormat)
	, data(NULL) 
{
	data = malloc(TGen::FormatTypeSize(componentFormat) * ImageFormatComponents(format) * size.width * size.height);
		
	if (!data)
		throw TGen::RuntimeException("Canvas::Canvas", "failed to allocated memory for canvas (size=") << TGen::FormatTypeSize(componentFormat) * ImageFormatComponents(format) * size.width * size.height << ")";
}

TGen::Canvas::~Canvas() {
	if (data)
		free(data);
	
	data = NULL;
}

TGen::Rectangle TGen::Canvas::getSize() const {
	return size;
}

TGen::ImageFormat TGen::Canvas::getFormat() const {
	return format;
}

TGen::FormatType TGen::Canvas::getComponentFormat() const {
	return componentFormat;
}

const void * TGen::Canvas::getData() const {
	return data;
}

void TGen::Canvas::fill(const TGen::Color & color) {
	int bytesPerPixel = TGen::FormatTypeSize(componentFormat) * ImageFormatComponents(format);
	int bytesTotal = bytesPerPixel * size.width * size.height;
	//int elementsTotal = size.width * size.height;

	void * fillWith = malloc(bytesPerPixel);
	if (!fillWith)
		throw TGen::RuntimeException("Canvas::Fill", "failed to allocate template for filling (bytes per pixel=") << bytesPerPixel << ")";

	memset(fillWith, 0, bytesPerPixel);
	//std::cout << "bytes per pixel: " << bytesPerPixel << std::endl;
	color.getFormatted(format, componentFormat, fillWith);

	/*for (int i = 0; i < 4; ++i) {
		std::cout << "array FILL WITH: " << std::hex << reinterpret_cast<uint32 *>(fillWith)[i] << std::endl;
		
	}*/
	
	//std::cout << "FILL WITH: " << std::hex << *reinterpret_cast<uint32 *>(fillWith) << std::endl;
	
	uint8 * dataPos = reinterpret_cast<uint8 *>(data);
	for (; dataPos != reinterpret_cast<uint8 *>(data) + bytesTotal; dataPos += bytesPerPixel) {
		memcpy(dataPos, fillWith, bytesPerPixel);
	}
	
	
	free(fillWith);
}

void TGen::Canvas::setPixel(const TGen::Vector2 & pos, const TGen::Color & color) {
	int bytesPerPixel = TGen::FormatTypeSize(componentFormat) * ImageFormatComponents(format);
	void * fillWith = malloc(bytesPerPixel);
	if (!fillWith)
		throw TGen::RuntimeException("Canvas::setPixel", "failed to allocate template for setting pixel (bytes per pixel=") << bytesPerPixel << ")";
	
	memset(fillWith, 0, bytesPerPixel);
	color.getFormatted(format, componentFormat, fillWith);
	
	setPixelRaw(pos, fillWith, bytesPerPixel);
	
	free(fillWith);	
}

void TGen::Canvas::setPixelRaw(const TGen::Vector2 & pos, void * data, uint bpp) {
	uint8 * dataPos = &reinterpret_cast<uint8 *>(this->data)[int(bpp * (int(size.width) * int(pos.y) + int(pos.x)))];
	memcpy(dataPos, data, bpp);
}

void TGen::Canvas::drawLine(const TGen::Vector2 & start, const TGen::Vector2 & end, const TGen::Color & color) {
	int bytesPerPixel = TGen::FormatTypeSize(componentFormat) * ImageFormatComponents(format);
	void * fillWith = malloc(bytesPerPixel);
	if (!fillWith)
		throw TGen::RuntimeException("Canvas::DrawLine", "failed to allocate template for line drawing (bytes per pixel=") << bytesPerPixel << ")";
	
	memset(fillWith, 0, bytesPerPixel);
	color.getFormatted(format, componentFormat, fillWith);
	
	TGen::Vector2 pos = start;
	float mag = (end - start).getMagnitude();
	
	float dx = (end - start).x / mag;
	float dy = (end - start).y / mag;
	
	for (int i = 0; i <= int(mag); ++i) {
		setPixelRaw(pos, fillWith, bytesPerPixel);
		pos.x += dx;
		pos.y += dy;
	}

	free(fillWith);	
}

