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
	
	std::auto_ptr<TGen::Canvas> canvas(new TGen::Canvas(size));
	
	if (line.getName() == "plain" || line.getName() == "fill" || line.getName() == "solid")
		genPlain(line, canvas.get());
	else if (line.getName() == "fadeU")
		genFade(line, canvas.get(), FadeU);
	else if (line.getName() == "fadeV")
		genFade(line, canvas.get(), FadeV);
	else if (line.getName() == "fadeCircle")
		genFadeCircle(line, canvas.get());
	else if (line.getName() == "checker")
		genChecker(line, canvas.get());
	else
		throw TGen::RuntimeException("ImageGenerator::generateImage", "generator not supported: '" + line.getName() + "'");
	
	return canvas.release();
}

void TGen::Engine::ImageGenerator::genPlain(const TGen::Engine::GenerateLine & line, TGen::Canvas * canvas) {
	canvas->fill(TGen::Color::Parse(line.getParameter("color")));
}

void TGen::Engine::ImageGenerator::genFade(const TGen::Engine::GenerateLine & line, TGen::Canvas * canvas, FadeDirection direction) {
	// start - end, U+
	TGen::Color start = TGen::Color::Parse(line.getParameter("start"));
	TGen::Color end = TGen::Color::Parse(line.getParameter("end"));
	TGen::Rectangle size = canvas->getSize();
	
	if (direction == FadeU) {
		for (int x = 0; x < size.width; ++x) {
			float t = float(x) / size.width;
			canvas->drawLine(TGen::Vector2(x, 0.0f), TGen::Vector2(x, size.height - 1.0), TGen::Interpolate(start, end, t));
		}	
	}
	else if (direction == FadeV) {
		// V+
		TGen::Color start = TGen::Color::Parse(line.getParameter("start"));
		TGen::Color end = TGen::Color::Parse(line.getParameter("end"));
	
		for (int y = 0; y < size.height; ++y) {
			float t = float(y) / size.height;
			canvas->drawLine(TGen::Vector2(0.0f, y), TGen::Vector2(size.width - 1.0, y), TGen::Interpolate(start, end, t));
		}
	}
	else {
		throw TGen::RuntimeException("ImageGenerator::genFade", "invalid fade direction");
	}
}

void TGen::Engine::ImageGenerator::genFadeCircle(const TGen::Engine::GenerateLine & line, TGen::Canvas * canvas) {
	TGen::Vector2 center = TGen::Vector2::Parse(line.getParameter("center", "0.5 0.5"));
	TGen::Color start = TGen::Color::Parse(line.getParameter("start"));
	TGen::Color end = TGen::Color::Parse(line.getParameter("end"));
	float radius = TGen::lexical_cast<float>(line.getParameter("radius", "0.5"));
	TGen::Rectangle size = canvas->getSize();
	
	for (int x = 0; x < size.width; ++x) {
		for (int y = 0; y < size.height; ++y) {
			TGen::Vector2 pos(float(x) / size.width, float(y) / size.height);
			scalar distance = (pos - center).getMagnitude();
			canvas->setPixel(TGen::Vector2(x, y), TGen::Interpolate(end, start, TGen::IdentityClamp(radius - distance)));
		}
	}	
}

void TGen::Engine::ImageGenerator::genChecker(const TGen::Engine::GenerateLine & line, TGen::Canvas * canvas) {
	TGen::Color color1 = TGen::Color::Parse(line.getParameter("color1"));
	TGen::Color color2 = TGen::Color::Parse(line.getParameter("color2"));	
	TGen::Rectangle size = canvas->getSize();
	
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

// TODO: om man inte kör med vissa egenskaper på writeAttribute så kommer saker bakifrån skina igenom....
// TODO: se över hierarkin, att den verkligen fungerar. 
//      börja med att sätta ut en billboardnode i scenen så man får se att de fungerar sen hitta på ett sätt att få sånna att ritas upp för olika entiteter
// TODO: rendrera lampor som en billboard, dvs, fixa billboards. det är möjligtvis bara en quad som använder en viss shader
// TODO: riktig spelargrej, först spectator. Kunna titta i alla riktningar och gå framåt osv, är samma som vanligt fast inte fysiskt begränsad
//				undrar vad som händer om man hookar på physBody på en kamera... haha

