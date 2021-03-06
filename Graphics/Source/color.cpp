/*
 *  color.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "color.h"
#include "error.h"
#include <sstream>
#include <limits>
#include <tgen_math.h>

using TGen::scalar;

TGen::Color TGen::Color::Identity(1.0f, 1.0f, 1.0f, 1.0f);
TGen::Color TGen::Color::White(1.0f, 1.0f, 1.0f, 1.0f);
TGen::Color TGen::Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
TGen::Color TGen::Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
TGen::Color TGen::Color::Green(0.0f, 1.0f, 0.0f, 1.0f);
TGen::Color TGen::Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
TGen::Color TGen::Color::Purple(1.0f, 0.0f, 1.0f, 1.0f);
TGen::Color TGen::Color::Yellow(1.0f, 1.0f, 1.0f, 0.0f);

TGen::Color::Color(scalar r, scalar g, scalar b, scalar a) 
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{
	
}

TGen::Color::Color(const TGen::Color & color) 
	: r(color.r)
	, g(color.g)
	, b(color.b)
	, a(color.a) 
{
	
}


TGen::Color::operator std::string() const {
	std::stringstream ss;
	ss << "r: " << r << " g: " << g << " b: " << b << " a: " << a;
	return ss.str();
}

TGen::Color & TGen::Color::operator = (const TGen::Color & color) {
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
	
	return *this;
}

TGen::Color & TGen::Color::operator *= (const TGen::Color & color) {
	r *= color.r;
	g *= color.g;
	b *= color.b;
	a *= color.a;
	
	return *this;
}

TGen::Color & TGen::Color::operator *= (scalar value) {
	r *= value;
	g *= value;
	b *= value;
	a *= value;
	
	return *this;
}


scalar TGen::Color::getAverage() const {
	return (r + g + b + a) / 4.0f;
}

void TGen::Color::getFormatted(TGen::ImageFormat format, TGen::FormatType componentFormat, void * store) const {
	uint8 * store8 = reinterpret_cast<uint8 *>(store);
	int typeSize = TGen::FormatTypeSize(componentFormat);
	
	switch (format) {
		case TGen::RGBA:
			getValueFormatted(componentFormat, r, store8);
			getValueFormatted(componentFormat, g, store8 += typeSize);
			getValueFormatted(componentFormat, b, store8 += typeSize);
			getValueFormatted(componentFormat, a, store8 += typeSize);			
			break;
		
		case TGen::RGB:
			getValueFormatted(componentFormat, r, store8);
			getValueFormatted(componentFormat, g, store8 += typeSize);
			getValueFormatted(componentFormat, b, store8 += typeSize);
			break;
			
		case TGen::BGRA:
			getValueFormatted(componentFormat, b, store8);
			getValueFormatted(componentFormat, g, store8 += typeSize);
			getValueFormatted(componentFormat, r, store8 += typeSize);
			getValueFormatted(componentFormat, a, store8 += typeSize);			
			break;
			
		case TGen::BGR:
			getValueFormatted(componentFormat, b, store8);
			getValueFormatted(componentFormat, g, store8 += typeSize);
			getValueFormatted(componentFormat, r, store8 += typeSize);
			break;
			
		case TGen::ARGB:
			getValueFormatted(componentFormat, a, store8);			
			getValueFormatted(componentFormat, r, store8 += typeSize);
			getValueFormatted(componentFormat, g, store8 += typeSize);
			getValueFormatted(componentFormat, b, store8 += typeSize);
			break;
			
		default:
			throw TGen::RuntimeException("Color::getFormatted", "failed to convert components into format");		
	}
}

void TGen::Color::getValueFormatted(TGen::FormatType format, float value, void * store) const {
	switch (format) {
		case TGen::TypeByte:
			*reinterpret_cast<int8 *>(store) = (value) * CHAR_MAX;
			break;

		case TGen::TypeUnsignedByte:
			*reinterpret_cast<uint8 *>(store) = value * UCHAR_MAX;
			break;
			
		case TGen::TypeShort:
			*reinterpret_cast<int16 *>(store) = (value) * SHRT_MAX;
			break;
			
		case TGen::TypeUnsignedShort:
			*reinterpret_cast<uint16 *>(store) = value * USHRT_MAX;
			break;
			
		case TGen::TypeInt:
			*reinterpret_cast<int32 *>(store) = (value) * INT_MAX - 65;		// <= 64 results in a black texture. WEIRD
			break;
			
		case TGen::TypeUnsignedInt:
			*reinterpret_cast<uint32 *>(store) = value * UINT_MAX;
			break;
			
		case TGen::TypeFloat:
			*reinterpret_cast<float *>(store) = value;			
			break;
			
		case TGen::TypeDouble:
			*reinterpret_cast<double *>(store) = value;
			break;
			
		default:
			throw TGen::RuntimeException("Color::getValueFormatted", "failed to convert value into format");		
	}
	
}

TGen::Color TGen::Color::Parse(const std::string & text) {
	std::string fixedString = TGen::toLower(text);
	
	if (fixedString.find("red") != std::string::npos)
		return TGen::Color::Red;
	else if (fixedString.find("green") != std::string::npos)
		return TGen::Color::Green;
	else if (fixedString.find("blue") != std::string::npos)
		return TGen::Color::Blue;
	else if (fixedString.find("black") != std::string::npos)
		return TGen::Color::Black;
	else if (fixedString.find("white") != std::string::npos)
		return TGen::Color::White;
	else if (fixedString.find("identity") != std::string::npos)
		return TGen::Color::Identity;
	
	std::stringstream ss;
	ss << text;
	
	float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;
	if (ss >> r >> g)
		ss >> b >> a;
	else
		b = g = r;
	
	return TGen::Color(r, g, b, a);
}

TGen::Color TGen::Color::operator - (const TGen::Color & color) const {	
	return TGen::Color(r - color.r, g - color.g, b - color.b, a - color.a);
}

TGen::Color TGen::Color::operator + (const TGen::Color & color) const {	
	return TGen::Color(r + color.r, g + color.g, b + color.b, a + color.a);
}

TGen::Color TGen::Color::operator * (scalar value) {
	return TGen::Color(r * value, g * value, b * value, a * value);
}
