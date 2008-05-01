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

TGen::Image * TGen::Engine::ImageLoader::load(TGen::Engine::File * file) {
	ILuint newImage = 0;
	ilGenImages(1, &newImage);
	ilBindImage(newImage);

	// TODO: if windows, use SDL_image instead. pfffffffffffffffff!

	if (ilLoadF(0, static_cast<ILHANDLE>(file)) == IL_FALSE) {
		std::stringstream ss;

		ILenum errorCode = ilGetError();
		while (errorCode != IL_NO_ERROR) {
			ss << errorCode << ", ";
			errorCode = ilGetError();
		}

		throw TGen::RuntimeException("ImageLoader::load", "failed to load image, error codes: " + ss.str());
	}
	
	// TODO: error checking
	
	return new TGen::Engine::DevilImage(newImage);
}

ILHANDLE TGen::Engine::ImageLoader::Open(const ILstring file) {
	throw TGen::RuntimeException("DevIL::ImageLoader::Open", "can not open file");
}

void TGen::Engine::ImageLoader::Close(ILHANDLE file) {
	TGen::Engine::File * realFile = static_cast<TGen::Engine::File *>(file);
	delete realFile;
}

ILboolean TGen::Engine::ImageLoader::Eof(ILHANDLE file) {
	TGen::Engine::File * realFile = static_cast<TGen::Engine::File *>(file);
	if (realFile)
		return realFile->eof() ? IL_TRUE : IL_FALSE;
	
	return true;
}

ILint TGen::Engine::ImageLoader::Getc(ILHANDLE file) {
	TGen::Engine::File * realFile = static_cast<TGen::Engine::File *>(file);
	if (realFile) {
		char data = 0;
		realFile->read(&data, sizeof(char));
		return data;
	}
	
	return 0;
}

ILint TGen::Engine::ImageLoader::Read(void * data, ILuint objectSize, ILuint objectCount, ILHANDLE file) {	
	TGen::Engine::File * realFile = static_cast<TGen::Engine::File *>(file);
	if (realFile)
		return realFile->read(data, objectSize, objectCount);
	
	return 0;
}
// varför laddar den inte in texturen rätt??? 
ILint TGen::Engine::ImageLoader::Seek(ILHANDLE file, ILint bytes, ILint mode) {
	TGen::Engine::File * realFile = static_cast<TGen::Engine::File *>(file);
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
	TGen::Engine::File * realFile = static_cast<TGen::Engine::File *>(file);
	if (!realFile)
		return 0;
	
	return realFile->getReadPos();
}
