/*
 *  logtarget.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_LOGTARGET_H
#define _TGEN_ENGINE_LOGTARGET_H

#include <tgen_core.h>

#include "log.h"

namespace TGen {
	namespace Engine {
		class LogTarget {
		public:
			LogTarget() {}
			virtual ~LogTarget() {}
			
			virtual void output(LogMessageType type, const std::string & title, const std::string & text) abstract;
			
		};
		
		class StreamOutput : public TGen::Engine::LogTarget {
		public:
			StreamOutput(std::ostream & stream);
			
			void output(LogMessageType type, const std::string & title, const std::string & text);
			
		private:
			std::ostream & stream;
		};
		
		class CombinedLogTargets : public TGen::Engine::LogTarget {
		public:
			CombinedLogTargets();
			~CombinedLogTargets();
			
			void addTarget(TGen::Engine::LogTarget * target);
			void output(LogMessageType type, const std::string & title, const std::string & text);
			
		private:
			typedef std::vector<TGen::Engine::LogTarget *> TargetList;
			TargetList targets;			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_LOGTARGET_H
