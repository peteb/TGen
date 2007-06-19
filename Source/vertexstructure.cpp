/*
 *  vertexstructure.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vertexstructure.h"
#include <iostream>

TGen::VertexStructure::VertexStructure() : align(4) {
	
}

TGen::VertexStructure::VertexStructure(TGen::VertexElement * elements) {
	uchar elementCount = elements[0].count;
	
	for (int i = 0; i < elementCount; ++i) {
		this->elements.push_back(elements[i + 1]);
	}
}

TGen::VertexElement::VertexElement(TGen::VertexElementType type, FormatType dataType, uchar count, bool shared, uchar unit) :
	type(type), dataType(dataType), count(count), unit(unit), shared(shared) {
	
}


int TGen::VertexStructure::getElementCount() const {
	return elements.size();
}

void TGen::VertexStructure::getElement(int num, TGen::VertexElement & ret) {
	ret = elements[num];
}

void TGen::VertexStructure::getElement(int num, TGen::VertexElement & ret) const {
	ret = elements[num];
}

TGen::FormatType TGen::VertexStructure::getElementDataType(int num) const {
	return elements[num].dataType;
}

int TGen::VertexStructure::getSize() const {
	int size = 0;
	for (int i = 0; i < elements.size(); ++i) {
		if (!elements[i].shared) {
			switch (elements[i].dataType) {
				case TGen::TypeFloat:
					size += sizeof(float) * elements[i].count;
					break;
				
				case TGen::TypeDouble:
					size += sizeof(double) * elements[i].count;
					break;
				
				case TGen::TypeUnsignedInt:
				case TGen::TypeInt:
					size += sizeof(int) * elements[i].count;
					break;

				case TGen::TypeUnsignedShort:
				case TGen::TypeShort:
					size += sizeof(short) * elements[i].count;
					break;

				case TGen::TypeUnsignedByte:
				case TGen::TypeByte:
					size += sizeof(char) * elements[i].count;
					break;
			}
		}
	}
	
	return size;
}

int TGen::VertexStructure::getStride() const {
//	std::cout << "size: " << getSize() << " stride: " << getSize() % align << std::endl;
	
	return getSize() % align;
}

void TGen::VertexStructure::AddElement(TGen::VertexElementType type, FormatType dataType, uchar count, bool shared, uchar unit) {
	elements.push_back(TGen::VertexElement(type, dataType, count, shared, unit));
}

