/*
 *  imageloader.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "imageloader.h"
#include "devilimage.h"
#include "file.h"

TGen::Engine::ImageLoader::ImageLoader() {
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

	ilSetRead(TGen::Engine::ImageLoader::Open, 
				 TGen::Engine::ImageLoader::Close, 
				 TGen::Engine::ImageLoader::Eof, 
				 TGen::Engine::ImageLoader::Getc, 
				 TGen::Engine::ImageLoader::Read, 
				 TGen::Engine::ImageLoader::Seek, 
				 TGen::Engine::ImageLoader::Tell);
}

TGen::Engine::ImageLoader::~ImageLoader() {
	
}

TGen::Image * TGen::Engine::ImageLoader::load(TGen::Engine::File & file, const std::string & ext) {
	// Use SDL_Image on windows, DevIL on all other platforms. this should be refactored into two classes

#ifndef _PLATFORM_WINDOWS
	return loadDevIL(file, ext);
#else
	return loadSDLImage(file, ext);
#endif
}

TGen::Image * TGen::Engine::ImageLoader::loadDevIL(TGen::Engine::File & file, const std::string & ext) {
	ILuint newImage = 0;
	ilGenImages(1, &newImage);
	ilBindImage(newImage);

	if (ilLoadF(0, reinterpret_cast<ILHANDLE>(&file)) == IL_FALSE) {
		std::stringstream ss;

		ILenum errorCode;
		while ((errorCode = ilGetError()) != IL_NO_ERROR) {
			ss << errorCode << ", ";
		}

		throw TGen::RuntimeException("ImageLoader::loadDevIL", "failed to load image, error codes: " + ss.str());
	}
	
	return new TGen::Engine::DevilImage(newImage);
}

#ifdef _PLATFORM_WINDOWS
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


TGen::Image * TGen::Engine::ImageLoader::loadSDLImage(TGen::Engine::File & file, const std::string & ext) {
	uint fileSize = file.getSize();
	std::auto_ptr<char> data(new char[fileSize]);
	//char * data = new char[fileSize];

	if (!file.read(data, fileSize))
		throw TGen::RuntimeException("ImageLoader::loadSDLImage", "failed to read!");

	std::cout << "FILE SIZE: " << fileSize << std::endl;

	SDL_RWops * rw = SDL_RWFromMem(data, fileSize);
	if (!rw)
		throw TGen::RuntimeException("ImageLoader::loadSDLImage", "failed to create rwops");

	SDL_Surface * imgSurf = IMG_LoadTyped_RW(rw, 1, reinterpret_cast<char *>(ext.c_str()));

	if (!imgSurf)
		throw TGen::RuntimeException("ImageLoader::loadSDLImage", "failed to load image to surface: " + std::string(IMG_GetError()));

	TGen::ImageFormat format = TGen::RGBA;
	if (imgSurf->format->BytesPerPixel == 3)
		format = TGen::RGB;

	TGen::auto_ptr<TGen::Canvas> canvas = new TGen::Canvas(TGen::Rectangle(imgSurf->w, imgSurf->h), format);

	SDL_LockSurface(imgSurf);

	canvas->fillData(imgSurf->pixels, imgSurf->w * imgSurf->h * imgSurf->format->BytesPerPixel);

	SDL_UnlockSurface(imgSurf);
	SDL_FreeSurface(imgSurf);

	return canvas.release();
}
#endif


ILHANDLE TGen::Engine::ImageLoader::Open(const ILstring file) {
	throw TGen::RuntimeException("DevIL::ImageLoader::Open", "can not open file");
}

void TGen::Engine::ImageLoader::Close(ILHANDLE file) {
	TGen::Engine::File * realFile = reinterpret_cast<TGen::Engine::File *>(file);
	delete realFile;
}

ILboolean TGen::Engine::ImageLoader::Eof(ILHANDLE file) {
	TGen::Engine::File * realFile = reinterpret_cast<TGen::Engine::File *>(file);
	if (realFile)
		return realFile->eof() ? IL_TRUE : IL_FALSE;
	
	return true;
}

ILint TGen::Engine::ImageLoader::Getc(ILHANDLE file) {
	TGen::Engine::File * realFile = reinterpret_cast<TGen::Engine::File *>(file);
	if (realFile) {
		char data = 0;
		realFile->read(&data, sizeof(char));
		return data;
	}
	
	return 0;
}

ILint TGen::Engine::ImageLoader::Read(void * data, ILuint objectSize, ILuint objectCount, ILHANDLE file) {	
	TGen::Engine::File * realFile = reinterpret_cast<TGen::Engine::File *>(file);
	if (realFile)
		return realFile->read(data, objectSize, objectCount);
	
	return 0;
}

ILint TGen::Engine::ImageLoader::Seek(ILHANDLE file, ILint bytes, ILint mode) {
	TGen::Engine::File * realFile = reinterpret_cast<TGen::Engine::File *>(file);
	if (!realFile)
		return 0;
	
	switch (mode) {
		case IL_SEEK_SET:
			realFile->seekReadPos(bytes, TGen::beg);
			break;
			
		case IL_SEEK_CUR:
			realFile->seekReadPos(bytes, TGen::cur);
			break;
			
		case IL_SEEK_END:
			realFile->seekReadPos(bytes, TGen::end);
			break;
	}
	
	return 0;
}

ILint TGen::Engine::ImageLoader::Tell(ILHANDLE file) {
	TGen::Engine::File * realFile = reinterpret_cast<TGen::Engine::File *>(file);
	if (!realFile)
		return 0;
	
	return realFile->getReadPos();
}

