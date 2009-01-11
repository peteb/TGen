/*
 *  resourcemanager.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "resourcemanager.h"
#include "log.h"
#include "filesystem.h"
#include "file.h"
#include "meshgenerator.h"
#include "imagegenerator.h"
#include "devilimage.h"
#include "preprocessor.h"
#include "generateline.h"
#include "modelfactory.h"
#include "variableregister.h"

TGen::Engine::ResourceManager::ResourceManager(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem, TGen::Renderer & renderer, TGen::Engine::VariableRegister & variables) 
	: logs(logs)
	, filesystem(filesystem)
	, renderer(renderer)
	, vertexCache(renderer, logs)
	, variables(variables)
{
	logs.info["res+"] << "initializing resource manager..." << TGen::endl;
	
	std::vector<std::string> materialsToLoad;
	materialsToLoad.reserve(50);
	filesystem.enumerateFiles("/materials/", materialsToLoad, true);
	
	for (int i = 0; i < materialsToLoad.size(); ++i) {
		if (materialsToLoad[i].find(".svn") == std::string::npos)
			loadMaterials(materialsToLoad[i]);
	}
}

TGen::Engine::ResourceManager::~ResourceManager() {
	logs.info["res-"] << "shutting down..." << TGen::endl;

	logs.info["res-"] << "shaders: " << TGen::endl;
	for (ShaderMap::iterator iter = shaders.begin(); iter != shaders.end(); ++iter)
		logs.info["res-"] << iter->first << TGen::endl;
	
	logs.info["res-"] << "materials: " << TGen::endl;
	for (MaterialMap::iterator iter = materials.begin(); iter != materials.end(); ++iter) {
		logs.info["res-"] << iter->first << " (frames: " << iter->second->getTimesRendered() << ")" << TGen::endl;

		const TGen::Material::SpecializationList & specializations = iter->second->getSpecializations();
		for (TGen::Material::SpecializationList::const_iterator iter = specializations.begin(); iter != specializations.end(); ++iter) {
			logs.info["res-"] << "   " << iter->second->getName() << " (" << iter->second->getTimesUsed() << ")" << TGen::endl;
		}
	}
	
	logs.info["res-"] << "removing meshes..." << TGen::endl;
	for (MeshMap::iterator iter = meshes.begin(); iter != meshes.end(); ++iter) {
		logs.info["res-"] << "removing mesh " << iter->first << TGen::endl;
		delete iter->second;
	}
	
	logs.info["res-"] << "removing textures..." << TGen::endl;
	for (TextureMap::iterator iter = textures.begin(); iter != textures.end(); ++iter)
		delete iter->second;
	
	logs.info["res-"] << "removing shaders..." << TGen::endl;
	for (ShaderMap::iterator iter = shaders.begin(); iter != shaders.end(); ++iter)
		delete iter->second;
	
	logs.info["res-"] << "removing materials..." << TGen::endl;
	for (MaterialMap::iterator iter = materials.begin(); iter != materials.end(); ++iter)
		delete iter->second;
	
}

// varje pass har en vector med shaders
// när man skapar ett material så skickar man med en vector med strängar som berättar vilka defs som ska läggas på
// typ:         LIGHT_POS,SHADOW_MAP
//					LIGHT_SPOT,GRAH,BAH,SHADOW_MAP
// sen kör man material->setCurrentShader(0), vilket är LIGHT_POS,SHADOW_MAP
//
// ska finnas något som klurar ut utifrån description field, typ LIGHT_POS|SHADOW_MAP|GLOW, och ger ett visst id för det
// och kunna skapa textsträng för det

TGen::ShaderProgram * TGen::Engine::ResourceManager::getShaderProgram(const std::string & name) {
	ShaderMap::iterator iter = shaders.find(name);
	if (iter != shaders.end())
		return iter->second;

	std::string fixedName;
	bool generate = false;
	
	if (name.substr(0, 4) == "gen:") {
		int colPos = name.rfind(":");
		fixedName = "/shaders/" + name.substr(4, colPos - 4) + ".shader";
		generate = true;
	}
	else {
		int colPos = name.rfind(":");
		fixedName = "/shaders/" + name.substr(0, colPos) + ".shader";
		generate = true;
	}
	
	logs.info["res"] << "loading shader '" << fixedName << "'..." << TGen::endl;
	
	std::string contents;
	
	{
		TGen::auto_ptr<TGen::Engine::File> file = filesystem.openRead(fixedName);
		contents = file->readAll();
	}
	
	// gen:directionalLighting:MAX_LIGHTS=1,STUFF=blabla
	
	
	generate = true;
	
	if (generate) {
		int colPos = name.rfind(":");
		if (colPos != std::string::npos) {
			std::string genString = name.substr(colPos + 1, name.size() - colPos - 1);
		
			logs.info["res"] << "preprocessing " << fixedName << " with " << genString << TGen::endl;
			
			TGen::Engine::TextPreprocessor processor;
			contents = processor.process(contents, genString);
			contents = processor.processIncludes(contents, *this);
			
			//std::cout << contents << std::endl;
		}
	}
	
	
	
	
	TGen::ShaderProgram * ret = renderer.createShaderProgram(contents.c_str());
	ret->link();
	
	shaders[name] = ret;
	
	return ret;
}

std::string TGen::Engine::ResourceManager::getIncludeContent(const std::string & identifier) {
	std::string fixedPath = TGen::prependPath(identifier, "/shaders/") + ".shader";
	TGen::auto_ptr<TGen::Engine::File> file = filesystem.openRead(fixedPath);
	
	return file->readAll();
}


TGen::Texture * TGen::Engine::ResourceManager::getTexture(const std::string & name) {
	TextureMap::iterator iter = textures.find(name);
	if (iter != textures.end())
		return iter->second;
	
	TGen::Texture * newTexture = NULL;
	
	if (name.substr(0, 4) == "gen:") {
		TGen::Engine::GenerateLine line(name);
		
		TGen::Engine::ImageGenerator generator;
		TGen::auto_ptr<TGen::Image> newImage = generator.generateImage(line);
		
		newTexture = renderer.createTexture(DerefHandle(newImage), TGen::RGBA);		// TODO: specify format in genline?
	}
	else {
		/*ILuint imageName;
		ilGenImages(1, &imageName);
		ilBindImage(imageName);*/		// what is this? I commented it out now. looks stoopid.
	
		std::string fixedName = TGen::prependPath(name, "textures/");
		std::string ext = TGen::toUpper(TGen::getExtension(fixedName));
				
		logs.info["res"] << "request for '" + fixedName + "'..." << TGen::endl;
	
		TGen::auto_ptr<TGen::Engine::File> file = filesystem.openRead(fixedName);
		TGen::auto_ptr<TGen::Image> image = imageLoader.load(DerefHandle(file), ext);

		// TESTA derefres här
		
		newTexture = renderer.createTexture(DerefHandle(image), TGen::RGBA);	// TODO: variable switch between RGBA and format of image image->getFormat()
	}
	
	textures[name] = newTexture;
	
	return newTexture;
}

int TGen::Engine::ResourceManager::getTextureType(const std::string & name) {
	// {NULL, colorMap, depthMap, normalMap}
	
	if (name == "$colormap")
		return 1;
	else if (name == "$normalmap" || name == "$bloom")
		return 2;
	else if (name == "$shadowmap")
		return 5;
	else if (name == "$miscmap")
		return 3;
	else if (name == "$depthmap")
		return 4;
	
	return 0;
}


TGen::Mesh * TGen::Engine::ResourceManager::getMesh(const std::string & name) {
	// hur ska man ladda in fillmeshen? kan ju ha en meshgenerator som kan skapa lite olika meshes
	// getMesh("gen:quad:-1 -1 1 1");
	// getMesh("gen:fillquad");
	
	MeshMap::iterator iter = meshes.find(name);
	if (iter != meshes.end())
		return iter->second;
		
	TGen::Mesh * newMesh = NULL;
	
	if (name.substr(0, 4) == "gen:") {
		std::string generateName = name.substr(strlen("gen:"), name.size() - strlen("gen:"));
		logs.info["res"] << "generating mesh for '" << generateName << "'..." << TGen::endl;
		TGen::Engine::MeshGenerator generator;
		newMesh = generator.generateMesh(generateName, vertexCache);
	}
	else {
		// TODO: checka filformat för parser
		TGen::auto_ptr<TGen::Engine::File> file = filesystem.openRead(name);
		
		/*if (name.substr(name.size() - strlen(".md5mesh")) == ".md5mesh") {
			TGen::MD5::Parser modelParser;
			TGen::MD5::File * md5File = modelParser.parse(*file);
			
			newMesh = md5File->createMesh(vertexCache, "testmd5", 0.1);
		}
		else {*/
			TGen::MD3::Parser modelParser;
			TGen::auto_ptr<TGen::MD3::File> md3File = modelParser.parse(DerefHandle(file));
		
		//md3File->printInfo(std::cout);
		
			//newMesh = md3File->createMesh(vertexCache, 0.001);	// TODO: 0.001 är scale factor, en global scale factor och sen kunna sätta per objekt?
		//}
	}
	
	//meshes[name] = newMesh;
	
	return newMesh;
}

TGen::NewModelInstance * TGen::Engine::ResourceManager::instantiateModel(const std::string & name) {
	ModelMap::iterator iter = models.find(name);
	TGen::NewModel * newModel = NULL;
	
	if (iter != models.end()) {	// the model template is already loaded
		newModel = iter->second;
	}
	else {
		TGen::Engine::ModelFactory factory(filesystem);
		TGen::Engine::GenerateLine line("gen:" + name);
		
		newModel = factory.createModel(line, vertexCache);
	}
	
	TGenAssert(newModel);
	
	return newModel->instantiate(vertexCache);
	
	/*std::string filename = name;
	scalar scale = 0.001f;
	
	if (name.substr(0, 4) == "gen:") {
		TGen::Engine::GenerateLine line(name);	
		
		filename = line.getName();
		scale = TGen::lexical_cast<scalar>(line.getParameter("scale"));
	}
	else {
		filename = name;
	}
	

	
	TGen::Engine::File * file = filesystem.openRead(filename);
	TGen::NewModel * newModel = NULL;
	
	
	if (filename.find(".md3") != std::string::npos) {
		TGen::MD3::Parser modelParser;
		TGen::MD3::File * md3file = modelParser.parse(*file);
		//md3file->printInfo(std::cout);
		newModel = md3file->createModel(renderer, TGen::VertexScaler(scale));
		
		delete md3file;
	}
	else if (filename.find(".md5mesh") != std::string::npos) {
		std::vector<std::string> files;
		filesystem.enumerateFiles(TGen::getFolder(name) + "/", files);
		
		for (int i = 0; i < files.size(); ++i)
			std::cout << files[i] << std::endl;
		
		
		TGen::MD5::ModelParser modelParser;
		TGen::MD5::ModelFile * md5mesh = modelParser.parse(*file);
		//md5mesh->printInfo(std::cout);
		newModel = md5mesh->createModel(renderer, TGen::getFile(name), scale);
		delete md5mesh;		
	}
	else {
		throw TGen::RuntimeException("ResourceManager::instantiateModel", "file extension is unfamiliar: '" + name + "'");
	}
	
	
	*/
	
	// createModel skapar en anim model eller static
	
	
	/*
	 
	 TGen::MD5::Parser modelParser;
	 TGen::MD5::File * md5file = modelParser.parse("zfat.md5mesh");
	 
	 
	 TGen::MD5::AnimParser animParser;
	 TGen::MD5::AnimFile * md5anim = animParser.parse("idle.md5anim");
	 
	 // här måste anim-info finnas, eller? man borde kunna skapa en animerande modell utan att ha animinfo 
	 //TGen::NewModel * newModel = md5file->createModel(renderer, 0.001);
	 TGen::MD5::AnimatingModel * newModel = md5file->createAnimatingModel(renderer, 0.001);
	 
	 newModel->setAnimationBase("idle", md5anim->createAnimation());
																		
	 */
	
	
	
	/*delete file;
	
	models.insert(ModelMap::value_type(name, newModel));
	
	return newModel->instantiate();*/
}

TGen::Material * TGen::Engine::ResourceManager::getMaterial(const std::string & name) {
	MaterialMap::iterator iter = materials.find(name);
	
	if (iter != materials.end()) {
		TGen::Material * material = iter->second;
		
		if (!material->isLinked())
			material->link(*this);

		return material;
	}
	
	logs.warning["res"] << "material '" << name << "' is not loaded, using 'nomat'" << TGen::endl;
	
	iter = materials.find("nomat");
	if (iter != materials.end()) {
		if (!iter->second->isLinked())
			iter->second->link(*this);
		
		return iter->second;
	}
	
	throw TGen::RuntimeException("ResourceManager::getMaterial", "material '" + name + "' is not found and there is no fallback (nomat)!");
}

void TGen::Engine::ResourceManager::loadMaterials(const std::string & filename) {
	logs.info["res"] << "loading materials from '" << filename << "'..." << TGen::endl;
	
	// read file contents
	std::string contents;
	
	TGen::auto_ptr<TGen::Engine::File> file = filesystem.openRead(filename);
	contents = file->readAll();
	
	
	// preprocess content
	TGen::Engine::TextPreprocessor processor; 
	std::string fixedContents = processor.process(contents, "RENDERER=" + variables["r_renderer"].getValue(), true, false);		

	
	// setup parser
	TGen::MaterialParser parser;
	
	
	// add shader permutations for all shader requests except fixed function. ff is undefined, btw
	parser.addShaderPermutation("LIGHT_OMNI,SHADOWMAP_SINGLE", 0);
	parser.addShaderPermutation("LIGHT_AMBIENT,LIGHT_OMNI,SHADOWMAP_SINGLE,LIGHT_FILTER", 2);
	parser.addShaderPermutation("LIGHT_AMBIENT,LIGHT_SPOT,SHADOWMAP_SINGLE,LIGHT_FILTER", 3);

	// TODO: mount complete filesystem on map load, mount /map/blabla/*/ as root also
	//			create function that checks whether a string starts with a string, if it doesn't, return new string with it prepended
	//           useful for resource manager when loading resources so not to duplicate directory, for example, /shaders/shaders/
	
	
	std::list<TGen::Material *> materialsLoaded;		// TODO: vector
	
	parser.parse(fixedContents.c_str(), materialsLoaded);
	
	logs.info["res"] << "loaded " << materialsLoaded.size() << " materials:\n";
	
	
	// delete materials that have already been loaded into the resource manager, also print them out
	int i = 0;
	
	for (std::list<TGen::Material *>::iterator iter = materialsLoaded.begin(); iter != materialsLoaded.end(); ++iter) {
		MaterialMap::iterator iter2 = materials.find((*iter)->getName());
		if (iter2 != materials.end())
			delete iter2->second;					// the material is already loaded and is deleted
		
		(*iter)->setMaximumTechnique(9);			// set default technique setting, TODO: read from setting register
		
		materials[(*iter)->getName()] = *iter;
		logs.info["res"] << (*iter)->getName() << "  ";
		
		if (i++ > 4) {		// break output line after four material names...
			logs.info["res"] << "\n";
			i = 0;
		}
	}
	
	logs.info["res"] << TGen::endl;
}

void TGen::Engine::ResourceManager::updateMaterials(scalar time) {	// TODO: this isn't really the place for this...
	MaterialMap::iterator iter = materials.begin();
	for (; iter != materials.end(); ++iter) {
		iter->second->update(time);
	}
}
