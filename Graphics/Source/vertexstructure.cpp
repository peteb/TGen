/*
 *  vertexstructure.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vertexstructure.h"
#include "error.h"
#include <iostream>

TGen::VertexStructure::VertexStructure() 
	: align(4) 
	, hasCoordElements(false)
	, hasNormalElements(false)
	, hasColorElements(false)
	, hasEdgeElements(false)
	, changed(false)
{
	for (int i = 0; i < 8; ++i)
		hasTexCoordUnitElements[i] = false;
}

/*TGen::VertexStructure::VertexStructure(TGen::VertexElement * elements) {
	uchar elementCount = elements[0].count;
	
	for (int i = 0; i < elementCount; ++i) {
		this->elements.push_back(elements[i + 1]);
	}
}*/

TGen::VertexElement::VertexElement()
	: bound(false)
	, boundValue(0)
	, normalize(false)
{

}

TGen::VertexElement::VertexElement(TGen::VertexElementType type, FormatType dataType, uchar count, bool shared, uchar unit, bool normalize) 
	: type(type)
	, dataType(dataType)
	, count(count)
	, unit(unit)
	, shared(shared) 
	, bound(false)
	, boundValue(0)
	, normalize(normalize)
{
	
}

TGen::VertexStructure::~VertexStructure() {
	
}

int TGen::VertexStructure::getElementCount() const {
	return int(elements.size());
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

TGen::VertexElement & TGen::VertexStructure::getElementAt(int num) {
	return elements[num];
}

TGen::VertexElement TGen::VertexStructure::getElementAtComponent(int component) const {
	int compPos = 0;
	for (int i = 0; i < int(elements.size()); ++i) {
		compPos += elements[i].count;
		if (compPos > component)
			return elements[i];
	}
	
	throw TGen::RuntimeException("VertexStructure::getElementAtComponent", "component out of range");
}

int TGen::VertexStructure::getComponentCount() const {
	int ret = 0;
	for (int i = 0; i < int(elements.size()); ++i) {
		ret += elements[i].count;
	}
	
	return ret;
}

int TGen::VertexStructure::getSize() const {
	int size = 0;
	for (int i = 0; i < int(elements.size()); ++i) {
		if (!elements[i].shared) {
			switch (elements[i].dataType) {	// TODO: use template-converter-thingy
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

void TGen::VertexStructure::addElement(TGen::VertexElementType type, FormatType dataType, uchar count, bool shared, uchar unit, bool normalize) {
	switch (type) {
		case TGen::VertexElementCoord:
			hasCoordElements = true;
			break;
			
		case TGen::VertexElementTexCoord:
			hasTexCoordUnitElements[unit] = true;
			break;
			
		case TGen::VertexElementNormal:
			hasNormalElements = true;
			break;
			
		case TGen::VertexElementColor:
			hasColorElements = true;
			break;
			
		case TGen::VertexElementEdgeFlag:
			hasEdgeElements = true;
			break;			
	}
	
	elements.push_back(TGen::VertexElement(type, dataType, count, shared, unit, normalize));
	changed = true;
}

/*
	format type count [unit = 0] [shared = no]
	ft20s = float tex 2 0 shared
	usc4 = unsigned short coord 4
 */

void TGen::VertexStructure::addElement(const std::string & format) {
	TGen::VertexElementType elType;
	TGen::FormatType type;
	uchar count = 0, unit = 0;
	bool shared = false;
	bool twoFormat = false;
	
	char format1 = format[0];
	char format2 = format[1];
	
	switch (format1) {
	case 'f':
		type = TGen::TypeFloat;
		break;
		
	case 'i':
		type = TGen::TypeInt;
		break;
		
	case 's':
		type = TGen::TypeShort;
		break;
		
	case 'd':
		type = TGen::TypeDouble;
		break;
		
	case 'b':
		type = TGen::TypeByte;
		break;
		
	case 'u':
		twoFormat = true;
		
		switch (format2) {
		case 'i':
			type = TGen::TypeUnsignedInt;
			break;
			
		case 's':
			type = TGen::TypeUnsignedShort;
			break;
			
		case 'b':
			type = TGen::TypeUnsignedByte;
			break;
			
		default:
			throw TGen::RuntimeException("VertexStructure::AddElement", "second format letter is insane");
		}
		break;
		
	default:
		throw TGen::RuntimeException("VertexStructure::AddElement", "first format letter is insane");
	}
	
	char typeName = 0;
	if (twoFormat)
		typeName = format[2];
	else
		typeName = format[1];
	
	switch (typeName) {
		case 'v':
			elType = TGen::VertexElementCoord;
			break;
			
		case 'n':
			elType = TGen::VertexElementNormal;
			break;
			
		case 'c':
			elType = TGen::VertexElementColor;
			break;
			
		case 'i':
			elType = TGen::VertexElementColorIndex;
			break;
		
		case 'e':
			elType = TGen::VertexElementEdgeFlag;
			break;
			
		case 't':
			elType = TGen::VertexElementTexCoord;
			break;
			
		default:
			throw TGen::RuntimeException("VertexStructure::AddElement", "element type invalid");
	}
		
	if (twoFormat)
		count = format[3] - '0';
	else
		count = format[2] - '0';
	
	if (twoFormat && format.size() > 4 || !twoFormat && format.size() > 3) {
		char unitName = 0;
		if (twoFormat)
			unitName = format[4];
		else
			unitName = format[3];
		
		if (unitName >= '0' && unitName <= '9')
			unit = unitName - '0';
		
		if (twoFormat && format.size() > 5 || !twoFormat && format.size() > 3) {
			if (twoFormat)
				shared = format[5] == 's';
			else
				shared = format[4] == 's';
		}
	}
	
	std::cout << "TYPE: " << int(elType) << " DATATYPE: " << int(type) << " COUNT: " << int(count) << " UNIT: " << int(unit) << " SHARED: " << shared << std::endl;

	elements.push_back(TGen::VertexElement(elType, type, count, shared, unit));
	changed = true;
}

/*TGen::VertexStructure::~VertexStructure() {
	
}
*/

/*
 format type count [unit = 0] [shared = no]
 ft20s = float tex 2 0 shared
 usc4 = unsigned short coord 4
 
 VertexElementType type;
 FormatType dataType;
 uchar count, unit;
 bool shared, bound, normalize;
 uint boundValue;
 */

const std::string & TGen::VertexStructure::getStringRepresentation() const {
	if (changed) {
		std::stringstream ss;
		
		for (int i = 0; i < elements.size(); ++i) {
			switch (elements[i].dataType) {
				case TGen::TypeFloat:
					ss << "f";
					break;
					
				case TGen::TypeInt:
					ss << "i";
					break;
					
				case TGen::TypeShort:
					ss << "s";
					break;
					
				case TGen::TypeDouble:
					ss << "d";
					break;
					
				case TGen::TypeByte:
					ss << "b";
					break;
					
				case TGen::TypeUnsignedInt:
					ss << "ui";
					break;

				case TGen::TypeUnsignedShort:
					ss << "us";
					break;
					
				case TGen::TypeUnsignedByte:
					ss << "ub";
					break;
										
				default:
					throw TGen::RuntimeException("VertexStructure::getStringRepresentation", "first format letter is insane");
			}

			
			switch (elements[i].type) {
				case TGen::VertexElementCoord:
					ss << "v";
					break;
					
				case TGen::VertexElementNormal:
					ss << "n";
					break;
					
				case TGen::VertexElementColor:
					ss << "c";
					break;
					
				case TGen::VertexElementColorIndex:
					ss << "i";
					break;
					
				case TGen::VertexElementEdgeFlag:
					ss << "e";
					break;
					
				case TGen::VertexElementTexCoord:
					ss << "t";
					break;
					
				case TGen::VertexElementAttribute:
					ss << "a";
					break;
					
				default:
					throw TGen::RuntimeException("VertexStructure::getStringRepresentation", "element type invalid");
			}
			
			ss << int(elements[i].count) << int(elements[i].unit);
			if (elements[i].shared)
				ss << "s";
			if (elements[i].normalize)
				ss << "n";
		}
		
		
		cachedRepresentation = ss.str();
		
		changed = false;
	}
	
	return cachedRepresentation;
}

