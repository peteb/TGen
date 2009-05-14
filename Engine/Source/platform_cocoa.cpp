/*
 *  platform_cocoa.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "platform_cocoa.h"

void TGen::Engine::Platform::DisplayExceptionWindow(const TGen::RuntimeException & e, const std::string & title) {
	std::string errorTitle;
	if (title.empty())
		errorTitle = "Exception";
	else
		errorTitle = title;
	
	std::string errorDescription;
	
	if (!e.getWhere().empty())
		errorDescription = "In '" + e.getWhere() + "':\n";
	
	errorDescription += e.getDescription() + "\n\n";
	errorDescription += "TGen is unable to continue. Click Report to send a report.";
	
	DisplayErrorWindow(errorTitle.c_str(), errorDescription.c_str(), true);
}

void TGen::Engine::Platform::DisplayMessage(const std::string & title, const std::string & text) {
	DisplayErrorWindow(title.c_str(), text.c_str(), false);
}
