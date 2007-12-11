/*
 *  log.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_LOG_H
#define _TGEN_ENGINE_LOG_H

#include <string>
#include <vector>
#include <sstream>
#include <tgen_core.h>
#include "textoutput.h"

namespace TGen {
	class separator {
	public:
		separator(const std::string & title) : title(title) {}
		
		std::string getString() const {
			std::string ret;
			ret += "-- ";
			ret += title + " ";
			
			for (int i = 0; i < 100 - title.size(); ++i)
				ret += "-";
			
			return ret;
		}
		
		std::string title;
	};
	
	namespace Engine {
		class LogTarget;
		
		enum LogMessageType {
			Info = 1,
			Warning,
			Error,			
		};
		
		class Endl {
		public:
			
		};
		

		
		class Log : public TGen::Engine::TextOutputer {
		public:	
			Log(LogMessageType type);
			~Log();
			
			void addTarget(TGen::Engine::LogTarget * target);
			TGen::Engine::Log & output(const std::string & area, const std::string & text);
			void outputText(const std::string & text, uint user);
			
			
			template<typename T>
			TGen::Engine::Log & operator << (T val) {
				ss << val;
					
				return *this;				
			}
			
			TGen::Engine::Log & operator << (const Endl & end) {
				output(area, ss.str());
				ss.str("");
				ss.clear();
				area = "";
				
				return *this;
			}
			
			TGen::Engine::Log & operator << (const TGen::RuntimeException & exception) {
				if (ss.str().empty()) {
					output(exception.getWhere(), exception.getDescription());
					ss.str("");
					ss.clear();
					area = "";
				}
				else {
					ss << exception.getDescription();
					
					output(area, ss.str());
					ss.str("");
					ss.clear();
					area = "";
				}
				
				return *this;
			}
			
			TGen::Engine::Log & operator << (const TGen::separator & sep) {
				output("", sep.getString());
				return *this;
			}
			
			TGen::Engine::Log & operator [] (const std::string & name) {
				area = name;
				return *this;
			}
			
		private:
			typedef std::vector<TGen::Engine::LogTarget *> TargetList;
			TargetList targets;
			
			LogMessageType type;
			
			std::string area;
			std::stringstream ss;
		};
		
		class CombinedLogTargets;
		
		class StandardLogs : public TGen::Engine::TextOutputer {
		public:	
			StandardLogs();
			~StandardLogs();
			
			void outputText(const std::string & text, uint user);
			
			TGen::Engine::Log info, warning, error;
			
		private:
			TGen::Engine::CombinedLogTargets * logtargets;
		};
	} // !Engine
	
	extern TGen::Engine::Endl endl;
	//using TGen::Engine::endl;
	//using TGen::Engine::separator;	
	//typedef separator TGen::Engine::separator;
} // !TGen


#endif // !_TGEN_ENGINE_LOG_H
