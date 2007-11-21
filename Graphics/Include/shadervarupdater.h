/*
 *  shadervarupdater.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 11/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_SHADERVARUPDATER_H
#define _TGEN_SHADERVARUPDATER_H

#include <tgen_core.h>

namespace TGen {
	class ShaderVariableUpdater {
	public:
		virtual ~ShaderVariableUpdater() {}
		
		virtual void updateShaderVariable(TGen::ShaderVariable & var, const std::string & name) abstract;
		
	};

	class PassShaderVariable {
	public:
		PassShaderVariable()
			: linkedVar(NULL)
		{
		}
		
		std::string name, linkId;
		TGen::ShaderVariable * linkedVar;
	};
}

#endif // !_TGEN_SHADERVARUPDATER_H
