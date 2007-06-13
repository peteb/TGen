/*
 *  vertexstructure.h
 *  tgen
 *
 *  Created by Peter Backman on 6/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_VERTEXSTRUCTURE_H
#define _TGEN_VERTEXSTRUCTURE_H

#include "types.h"
#include "vector3.h"
#include "vector2.h"
#include "color.h"
#include <vector>

namespace TGen {
	enum VertexElementType {
		CoordElement = 1,
		NormalElement,
		ColorElement,
		IndexElement,
		EdgeFlagElement,
		TexCoordElement,
	};
	
	enum VertexElementDataType {
		TypeFloat = 1,
		TypeDouble,
		TypeInt,
		TypeShort,
		TypeUnsignedShort,
		TypeByte,
		TypeUnsignedByte,
		TypeUnsignedInt,
		
	};
	
	class VertexElement {
	public:
		VertexElement() {}
		VertexElement(TGen::VertexElementType type, VertexElementDataType dataType, uchar count, uchar unit = 0);
		
		VertexElementType type;
		VertexElementDataType dataType;
		uchar count, unit;
	};
	
	class VertexStructure {
	public:	
		VertexStructure();
		VertexStructure(TGen::VertexElement * elements);
		~VertexStructure() {}
		
		int getElementCount() const;
		void getElement(int num, TGen::VertexElement & ret);
		int getStride() const;
		int getSize() const;
		void AddElement(TGen::VertexElementType type, VertexElementDataType dataType, uchar count, uchar unit = 0);
		
	private:
		std::vector<TGen::VertexElement> elements;
		int align;
	};
	
	
	template<typename T> class TGenDataType {public: };
	template<> class TGenDataType<float> {public: TGenDataType() : type(TypeFloat) {} VertexElementDataType type; };
	template<> class TGenDataType<double> {public: TGenDataType() : type(TypeDouble) {} VertexElementDataType type; };
	template<> class TGenDataType<int> {public: TGenDataType() : type(TypeInt) {} VertexElementDataType type; };
	template<> class TGenDataType<short> {public: TGenDataType() : type(TypeShort) {} VertexElementDataType type; };
	template<> class TGenDataType<unsigned char> {public: TGenDataType() : type(TypeUnsignedByte) {} VertexElementDataType type; };
	template<> class TGenDataType<char> {public: TGenDataType() : type(TypeByte) {} VertexElementDataType type; };
	template<> class TGenDataType<unsigned short> {public: TGenDataType() : type(TypeUnsignedShort) {} VertexElementDataType type; };
	template<> class TGenDataType<unsigned int> {public: TGenDataType() : type(TypeUnsignedInt) {} VertexElementDataType type; };
	
	template<typename T>
	class Vertex3 : public virtual TGen::VertexStructure {
	public:		
		Vertex3() {AddElement(TGen::CoordElement, TGenDataType<T>().type, 3); }
		
		class Type {
		public:
			Type(const TGen::Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z) {}
			
			T x, y, z;
		};
	};	
	
	template<typename T>
	class Color4 : public virtual TGen::VertexStructure {
	public:
		Color4() {AddElement(TGen::ColorElement, TGenDataType<T>().type, 4); }

		class Type {
		public:
			Type(const TGen::Color & color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
			
			T r, g, b, a;
		};
	};
	
	template<typename T>
	class Normal3 : public virtual TGen::VertexStructure {
	public:
		Normal3() {AddElement(TGen::NormalElement, TGenDataType<T>().type, 3); }
			
		class Type {
		public:
			Type(const TGen::Vector3 & normal) : x(normal.x), y(normal.y), z(normal.z) {}
				
			T x, y, z;
		};
	};
	
	template<typename T, int unit>
	class TexCoord2 : public virtual TGen::VertexStructure {
	public:
		TexCoord2() {AddElement(TGen::TexCoordElement, TGenDataType<T>().type, 2, unit); }
			
		class Type {
		public:
			Type(const TGen::Vector2 & texcoord) : u(texcoord.x), v(texcoord.y) {}
			
			T u, v;
		};
	};
	
	
	template<class A, class B>
	class JoinVertex2 : public A, public B {
	public:
		JoinVertex2() : A(), B() {}
			
		class Type : public A::Type, public B::Type {
		public:
			template<typename p1, typename p2> Type(p1 a, p2 b) : A::Type(a), B::Type(b) {}
		};
	};
	
	template<class A, class B, class C>
	class JoinVertex3 : public A, public B, public C {
	public:
		JoinVertex3() : A(), B(), C() {}
			
		class Type : public A::Type, public B::Type, public C::Type {
		public:
			template<typename p1, typename p2, typename p3> Type(p1 a, p2 b, p3 c) : A::Type(a), B::Type(b), C::Type(c) {}
		};
	};
	
	template<class A, class B, class C, class D>
	class JoinVertex4 : public A, public B, public C, public D {
	public:
		JoinVertex4() : A(), B(), C(), D() {}
			
		class Type : public A::Type, public B::Type, public C::Type, public D::Type {
		public:
			template<typename p1, typename p2, typename p3, typename p4> Type(p1 a, p2 b, p3 c, p4 d) : A::Type(a), B::Type(b), C::Type(c), D::Type(d) {}
		};
	};

	template<class A, class B, class C, class D, class E>
	class JoinVertex5 : public A, public B, public C, public D, public E {
	public:
		JoinVertex5() : A(), B(), C(), D(), E() {}
			
		class Type : public A::Type, public B::Type, public C::Type, public D::Type, public E::Type {
		public:
			template<typename p1, typename p2, typename p3, typename p4, typename p5> Type(p1 a, p2 b, p3 c, p4 d, p5 e) : A::Type(a), B::Type(b), C::Type(c), D::Type(d), E::Type(e) {}
		};
	};	
} // !TGen

#endif // !_TGEN_VERTEXSTRUCTURE_H

