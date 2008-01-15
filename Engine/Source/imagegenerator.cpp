/*
 *  imagegenerator .cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "imagegenerator.h"
#include "generateline.h"
#include <tgen_core.h>
#include <tgen_math.h>
#include <tgen_graphics.h>

TGen::Image * TGen::Engine::ImageGenerator::generateImage(const TGen::Engine::GenerateLine & line) {
	TGen::Rectangle size(2, 2);
	
	if (line.getParameters().find("width") != line.getParameters().end())
		size = TGen::Rectangle(TGen::lexical_cast<float>(line.getParameter("width")), TGen::lexical_cast<float>(line.getParameter("height")));
	
	if (line.getParameters().find("size") != line.getParameters().end()) {
		float newSize = TGen::lexical_cast<float>(line.getParameter("size"));
		size = TGen::Rectangle(newSize, newSize);
	}
	
	TGen::Canvas * canvas = new TGen::Canvas(size);
	
	if (line.getName() == "plain") {
		canvas->fill(TGen::Color::Parse(line.getParameter("color")));
	}
	else if (line.getName() == "fadeU") {
		// start - end, U+
		TGen::Color start = TGen::Color::Parse(line.getParameter("start"));
		TGen::Color end = TGen::Color::Parse(line.getParameter("end"));
		
		for (int x = 0; x < size.width; ++x) {
			float t = float(x) / size.width;
			canvas->drawLine(TGen::Vector2(x, 0.0f), TGen::Vector2(x, size.height - 1.0), TGen::Interpolate(start, end, t));
		}
	}
	else if (line.getName() == "fadeV") {
		// V+
		TGen::Color start = TGen::Color::Parse(line.getParameter("start"));
		TGen::Color end = TGen::Color::Parse(line.getParameter("end"));

		for (int y = 0; y < size.height; ++y) {
			float t = float(y) / size.height;
			canvas->drawLine(TGen::Vector2(0.0f, y), TGen::Vector2(size.width - 1.0, y), TGen::Interpolate(start, end, t));
		}
	}
	else if (line.getName() == "fadeCircle") {
		TGen::Vector2 center = TGen::Vector2::Parse(line.getParameter("center", "0.5 0.5"));
		TGen::Color start = TGen::Color::Parse(line.getParameter("start"));
		TGen::Color end = TGen::Color::Parse(line.getParameter("end"));
		float radius = TGen::lexical_cast<float>(line.getParameter("radius", "0.5"));
	
		for (int x = 0; x < size.width; ++x) {
			for (int y = 0; y < size.height; ++y) {
				TGen::Vector2 pos(float(x) / size.width, float(y) / size.height);
				scalar distance = (pos - center).getMagnitude();
				canvas->setPixel(TGen::Vector2(x, y), TGen::Interpolate(end, start, TGen::IdentityClamp(radius - distance)));
			}
		}
	}
	else if (line.getName() == "checker") {
		TGen::Color color1 = TGen::Color::Parse(line.getParameter("color1"));
		TGen::Color color2 = TGen::Color::Parse(line.getParameter("color2"));
		
		int tileHeight = size.height / TGen::lexical_cast<int>(line.getParameter("tiles"));
		int tileWidth = size.width / TGen::lexical_cast<int>(line.getParameter("tiles"));

		bool fill = false;
		
		for (int x = 0; x < size.width; ++x) {
			if (!(x % tileWidth))
				fill = !fill;

			for (int y = 0; y < size.height; ++y) {
				if (!(y % tileHeight))
					fill = !fill;
				
				if (fill)
					canvas->setPixel(TGen::Vector2(x, y), color2);
				else
					canvas->setPixel(TGen::Vector2(x, y), color1);

			}
		}
	}
	else {		
		delete canvas;
		throw TGen::RuntimeException("ImageGenerator::generateImage", "generator not supported: '" + line.getName() + "'");
	}
	
	return canvas;
}

// TODO: throwErrors ska bara köras varje sekund
