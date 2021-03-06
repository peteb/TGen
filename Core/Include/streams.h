/*
 *  streams.h
 *  TGen Core
 *
 *  Created by Peter Backman on 11/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_STREAMS_H
#define _TGEN_STREAMS_H

namespace TGen {
	enum Seek {
		beg = 1,
		end,
		cur,
	};
	
	class InputStream {
	public:
		InputStream() {}
		virtual ~InputStream() {}
		
		virtual uint getReadPos() abstract;
		virtual void seekReadPos(uint pos, TGen::Seek rel) abstract;
		virtual int read(char * data, uint size) abstract;
		virtual uint getSize() abstract;
	};
	
	class OutputStream {
	public:
		OutputStream() {}
		virtual ~OutputStream() {}
		
		virtual int write(char * data, uint size) abstract;
	};
	
	class IstreamAdaptor : public TGen::InputStream {
	public:
		IstreamAdaptor(std::istream & stream)
		: stream(stream)
		{
		}
		
		uint getReadPos() {
			return stream.tellg();
		}
		
		void seekReadPos(uint pos, TGen::Seek rel) {
			if (rel == TGen::beg)
				stream.seekg(pos, std::ios::beg);
			else if (rel == TGen::end)
				stream.seekg(pos, std::ios::end);
			else if (rel == TGen::cur)
				stream.seekg(pos, std::ios::cur);
			else
				throw TGen::RuntimeException("IstreamAdaptor::seekReadPos", "relative type invalid");
		}
		
		int read(char * data, uint size) {
			stream.read(data, size);
			
			return 0; // TODO: fix!!!!!!!!
		}
		
		uint getSize() {
			uint startPos = stream.tellg();
			
			stream.seekg(0, std::ios::beg);
			uint s1 = stream.tellg();
			stream.seekg(0, std::ios::end);
			uint s2 = stream.tellg();
			stream.seekg(startPos, std::ios::beg);
			
			return s2 - s1;
		}
		
	private:
		std::istream & stream;
	};	
} // !TGen

#endif // !_TGEN_STREAMS_H
