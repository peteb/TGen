/*
 *  color.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "color.h"
#include <sstream>

TGen::Color TGen::Color::Identity(1.0f, 1.0f, 1.0f, 1.0f);
TGen::Color TGen::Color::White(1.0f, 1.0f, 1.0f, 1.0f);
TGen::Color TGen::Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
TGen::Color TGen::Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
TGen::Color TGen::Color::Green(0.0f, 1.0f, 0.0f, 1.0f);
TGen::Color TGen::Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
TGen::Color TGen::Color::Purple(1.0f, 0.0f, 1.0f, 1.0f);

TGen::Color::Color(scalar r, scalar g, scalar b, scalar a) : r(r), g(g), b(b), a(a) {
	
}

TGen::Color::Color(const TGen::Color & color) : r(color.r), g(color.g), b(color.b), a(color.a) {
	
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

TGen::Color & TGen::Color::operator *= (scalar scalar) {
	r *= scalar;
	g *= scalar;
	b *= scalar;
	a *= scalar;
	
	return *this;
}


scalar TGen::Color::getAverage() const {
	return (r + g + b + a) / 4.0f;
}

