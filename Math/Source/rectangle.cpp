/*
 *  rectangle.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "rectangle.h"
#include <sstream>

TGen::Rectangle::Rectangle() 
	: width(0.0f)
	, height(0.0f) 
{	
}

TGen::Rectangle::Rectangle(const TGen::Rectangle & rectangle) 
	: center(rectangle.center)
	, width(rectangle.width)
	, height(rectangle.height)
{	
}

TGen::Rectangle::Rectangle(const TGen::Vector2 & center, scalar width, scalar height) 
	: center(center)
	, width(width)
	, height(height)
{	
}

TGen::Rectangle::Rectangle(scalar width, scalar height) 
	: center(width / 2.0f, height / 2.0f)
	, width(width)
	, height(height)
{	
}

TGen::Rectangle & TGen::Rectangle::operator = (const TGen::Rectangle & rectangle) {
	center = rectangle.center;
	width = rectangle.width;
	height = rectangle.height;
	
	return *this;
}

TGen::Rectangle TGen::Rectangle::FromUpperLeft(const TGen::Vector2 & topleft, scalar width, scalar height) {
	TGen::Rectangle ret;
	ret.center = topleft + TGen::Vector2(width, height) / 2.0f;
	ret.width = width;
	ret.height = height;
	
	return ret;	
}

TGen::Rectangle::operator std::string() const {
	std::stringstream ss;
	ss << "center: " << std::string(center) << " width: " << width << " height: " << height;
	
	return ss.str();
}

TGen::Vector2 TGen::Rectangle::getUpperLeft() const {
	return TGen::Vector2(center.x - width / 2.0f, center.y - height / 2.0f);
}

TGen::Vector2 TGen::Rectangle::getLowerRight() const {
	return TGen::Vector2(center.x + width / 2.0f, center.y + height / 2.0f);	
}

bool TGen::Rectangle::intersects(const TGen::Rectangle & rect) const {
	TGen::Vector2 utop1, utop2, ubot1, ubot2;
	utop1 = rect.getUpperLeft();
	ubot1 = rect.getLowerRight();
	utop2 = getUpperLeft();
	ubot2 = getLowerRight();
	
	if (ubot1.y < utop2.y) return false;
	if (utop1.y > ubot2.y) return false;
	if (ubot1.x < utop2.x) return false;
	if (utop1.x > ubot2.x) return false;
	
	return true;
}
#include <iostream>
void TGen::Rectangle::calculate(const TGen::Vector2 * vectors, int vectorcount) {
	TGen::Vector2 min(1336.0f, 1336.0f), max(1336.0f, 1336.0f);
	bool first = true;
	for (int i = 0; i < vectorcount; ++i) {
		if (first) {
			min = vectors[i];
			max = vectors[i];
			first = false;
		}
		else {
			min.x = std::min(vectors[i].x, min.x);
			min.y = std::min(vectors[i].y, min.y);
			max.x = std::max(vectors[i].x, max.x);
			max.y = std::max(vectors[i].y, max.y);
		}
	}
	
	std::cout << "min: " << std::string(min) << " max: " << std::string(max) << std::endl;
	
	width = (max.x - min.x);
	height = (max.y - min.y);
	center = min + (max - min) / 2.0f;
}
