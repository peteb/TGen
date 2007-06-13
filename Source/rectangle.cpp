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

TGen::Rectangle::Rectangle() : width(0.0f), height(0.0f) {
	
}

TGen::Rectangle::Rectangle(const TGen::Rectangle & rectangle) :
	center(rectangle.center), width(rectangle.width), height(rectangle.height)
{
	
}

TGen::Rectangle::Rectangle(const TGen::Vector2 & center, scalar width, scalar height) :
	center(center), width(width), height(height)
{
	
}

TGen::Rectangle::Rectangle(scalar width, scalar height) :
	center(width / 2.0f, height / 2.0f), width(width), height(height)
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
