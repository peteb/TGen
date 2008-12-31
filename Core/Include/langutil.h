/*
 *  langutil.h
 *  TGen Core
 *
 *  Created by Peter Backman on 12/20/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_LANGUTIL_H
#define _TGEN_LANGUTIL_H

namespace TGen {
	template<class T, class B> struct derived_from {
		static void constraints(T* p) { B* pb = p; }
		derived_from() { void(*p)(T*) = constraints; }
	};

	template<class T1, class T2> struct can_copy {
		static void constraints(T1 a, T2 b) { T2 c = a; b = a; }
		can_copy() { void(*p)(T1,T2) = constraints; }
	};

	template<class T1, class T2 = T1> struct can_compare {
	static void constraints(T1 a, T2 b) { a==b; a!=b; a<b; }
		can_compare() { void(*p)(T1,T2) = constraints; }
	};

	template<class T1, class T2, class T3 = T1> struct can_multiply {
		static void constraints(T1 a, T2 b, T3 c) { c = a*b; }
		can_multiply() { void(*p)(T1,T2,T3) = constraints; }
	};
	
	template<typename T>
	class auto_ptr {
		mutable std::auto_ptr<T> ptr;
		
	public:
		auto_ptr(T * obj) {ptr.reset(obj); }
		auto_ptr(const auto_ptr & other) {ptr.reset(other.ptr.release()); }
		
		auto_ptr & operator = (T * obj) {ptr.reset(obj); return *this; }
		
		T * operator * () {return get(); }
		T * operator -> () {return get(); }
		T * get() const {TGenAssert(ptr.get()); return ptr.get(); }
		T * release() {return ptr.release(); }
	};
	
	class ScopeDebugger {
	public:
		ScopeDebugger(const std::string & message)
			: message(message)
		{
			std::cerr << "+" << message << std::endl;
		}
		
		~ScopeDebugger() {
			std::cerr << "-" << message << std::endl;
		}
		
	private:
		std::string message;
	};
}

#endif // !_TGEN_LANGUTIL_H
