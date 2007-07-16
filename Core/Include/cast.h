/*
 *  cast.h
 *  TGen Core
 *
 *  Created by Peter Backman on 7/17/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_CAST_H
#define _TGEN_CAST_H

namespace TGen {
	template<typename to, typename from>
	to lexical_cast(const from & _from) {
		std::stringstream ss;
		ss << _from;
		to ret;
		ss >> ret;
		return ret;
	}
} // !Tgen

#endif // !_TGEN_CAST_H
