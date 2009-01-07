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
#include "shaderprogram.h"
#include "shadervariable.h"

namespace TGen {
	//class ShaderVariable;
	
	// Used for VBO-embedded attributes etc
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
	
	enum ShaderVarType {
		ShaderVarInt = 1,
		ShaderVarFloat,
		ShaderVarFloat2,
		ShaderVarFloat3,
		ShaderVarFloat4,
	};
	
	// testa spara en länk till varje variabel i varje shader, dvs, 3 stycken samtidigt. (för varje texunit som transformerar texcoords). or just clone the shadervar :P
	class ShaderUpdater {
	public:
		ShaderUpdater(const std::string & varName, ShaderVarType type, float value);
		ShaderUpdater(const ShaderUpdater & other);
		
		virtual ~ShaderUpdater();
		
		void link(TGen::ShaderProgram * shader);
		ShaderUpdater * clone() const;
		void update();
		
	protected:
		TGen::ShaderVariable * linkedVar;
		std::string varName;
		ShaderVarType type;
		float value;
	};
}

#endif // !_TGEN_SHADERVARUPDATER_H
