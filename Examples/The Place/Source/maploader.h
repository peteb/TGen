/*
 *  maploader.h
 *  The Place
 *
 *  Created by Peter Backman on 7/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_THEPLACE_MAPLOADER_H
#define _TGEN_THEPLACE_MAPLOADER_H

#include <list>
#include <tgen_core.h>
#include <tgen_graphics.h>

class Map;
class Mesh;
class MapLinkCallback;
class MeshData;


enum MapTokens {
	MapTokenBlockStart = 20,
	MapTokenBlockEnd = 21,
	MapTokenMesh = 22,
	MapTokenVertices = 23,
	MapTokenIndices = 24,
	MapTokenEndOfLine = 25,	
	
};


class MapLoader {
public:	
	MapLoader();
	~MapLoader();

	void Parse(const char * code, std::list<Map *> & maps);
	void Link(MapLinkCallback & callback);
	
	
	void ParseGlobalBlock();
	void ParseMeshBlock(Mesh * mesh);
	void ParseVertices(MeshData * mesh);
	
private:
	std::string getStringToken(const std::string & name, bool ignorelf, bool quote);
	std::string getNumericToken(const std::string & name, bool ignorelf, bool quote);	
	void StepToken();
	void StepOverLF();
			
	TGen::TokenStream::TokenList::iterator currentToken, endIter;
	TGen::TokenStream tokens;

};

class MapTokenizer : public TGen::Tokenizer {
public:
	MapTokenizer();
	int getSpecialToken(char * text, TGen::TokenStream & stream);
};


class MapLinkCallback {
public:
	virtual ~MapLinkCallback() {}
	
	virtual Mesh * getMesh(const std::string & name) abstract;
	virtual void setMesh(const std::string & name, const MeshData * vertexData, const MeshData * indexData, TGen::PrimitiveType primitive);
};

#endif // !_TGEN_THEPLACE_MAPLOADER_H
