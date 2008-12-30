/*
 *  stringman.cpp
 *  TGen Core
 *
 *  Created by Peter Backman on 7/17/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "stringman.h"
#include <iomanip>
#include <sstream>

std::string TGen::toLower(const std::string & from) {
	std::string ret;
	std::string::const_iterator it = from.begin();
	
	while (it != from.end()) { 
		ret += ::tolower(*it);	
		++it;
	}
	
	return ret;
}

std::string TGen::toUpper(const std::string & from) {
	std::string ret;
	std::string::const_iterator it = from.begin();
	
	while (it != from.end()) { 
		ret += ::toupper(*it);	
		++it;
	}
	
	return ret;	
}

// Input: /blaha/hehe/foo/loek.md3   = /blaha/hehe/foo/
std::string TGen::getFolder(const std::string & path) {
	return path.substr(0, path.rfind("/"));
}

std::string TGen::getFile(const std::string & path) {
	return path.substr(path.rfind("/") + 1);
}

std::string TGen::getSize(uint64 bytes) {
	std::stringstream ss;
	ss << std::setprecision(3);
	
	if (bytes > 1024 * 1024 * 1024)
		ss << double(bytes) / 1024.0 / 1024.0 / 1024.0 << " GB";
	else if (bytes > 1024 * 1024)
		ss << double(bytes) / 1024.0 / 1024.0 << " MB";
	else if (bytes > 1024)
		ss << double(bytes) / 1024.0 << " KB";
	else
		ss << bytes << " B";
	
	return ss.str();
}

std::string TGen::getExtension(const std::string & path) {
	return path.substr(path.rfind(".") + 1);
}

std::string TGen::prependString(const std::string & str, const std::string & prefix) {
	return (str.substr(0, prefix.size()) == prefix ? str : prefix + str);
}

std::string TGen::prependPath(const std::string & path, const std::string & prefix) {
	return (path[0] == '/' ? path : prependString((path[0] == '/' ? path.substr(1) : path), prefix));
}
