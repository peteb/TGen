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
#include "renderer_types.h"
#include <vector>

namespace TGen {
	enum VertexElementType {
		CoordElement = 1,
		NormalElement,
		ColorElement,
		ColorIndexElement,
		EdgeFlagElement,
		TexCoordElement,
	};
		
	class VertexElement {
	public:
		VertexElement() {}
		VertexElement(TGen::VertexElementType type, FormatType dataType, uchar count, bool shared = false, uchar unit = 0);
		
		VertexElementType type;
		FormatType dataType;
		uchar count, unit;
		bool shared;
	};
	
	class VertexStructure {
	public:	
		VertexStructure();
		VertexStructure(TGen::VertexElement * elements);
		~VertexStructure() {}
		
		int getElementCount() const;
		FormatType getElementDataType(int num) const;
		VertexElement getElementAtComponent(int component) const;
		void getElement(int num, TGen::VertexElement & ret);
		void getElement(int num, TGen::VertexElement & ret) const;
		int getStride() const;
		int getSize() const;
		int getComponentCount() const;
		void AddElement(TGen::VertexElementType type, FormatType dataType, uchar count, bool shared = false, uchar unit = 0);
		void AddElement(const std::string & format);
		
	private:
		std::vector<TGen::VertexElement> elements;
		int align;
	};
	
	// TODO: en public bool isExtensionAvailable(), men bara i OpenGL::Renderer
	
	template<typename T>
	class Vertex4 : public virtual TGen::VertexStructure {
	public:		
		Vertex4() {AddElement(TGen::CoordElement, TGenDataType<T>().type, 4); }
			
		class Type {
		public:
			Type() {}
			Type(const TGen::Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z), w(0.0) {}
				
			T x, y, z, w;
		};
	};	
	
	template<typename T>
	class Vertex3 : public virtual TGen::VertexStructure {
	public:		
		Vertex3() {AddElement(TGen::CoordElement, TGenDataType<T>().type, 3); }
		
		class Type {
		public:
			Type() {}
			Type(const TGen::Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z) {}
			
			T x, y, z;
		};
	};	
	
	template<typename T>
	class Vertex2 : public virtual TGen::VertexStructure {
	public:		
		Vertex2() {AddElement(TGen::CoordElement, TGenDataType<T>().type, 2); }
			
		class Type {
		public:
			Type() {}
			Type(const TGen::Vector2 & vector) : x(vector.x), y(vector.y) {}
				
			T x, y;
		};
	};	
	
	
	template<typename T>
	class Color3 : public virtual TGen::VertexStructure {
	public:
		Color3() {AddElement(TGen::ColorElement, TGenDataType<T>().type, 3); }
			
		class Type {
		public:
			Type() {}
			Type(const TGen::Color & color) : r(color.r), g(color.g), b(color.b) {}
				
			T r, g, b;
		};
	};	
	
	template<typename T>
	class Color4 : public virtual TGen::VertexStructure {
	public:
		Color4() {AddElement(TGen::ColorElement, TGenDataType<T>().type, 4); }

		class Type {
		public:
			Type() {}
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
			Type() {}
			Type(const TGen::Vector3 & normal) : nx(normal.x), ny(normal.y), nz(normal.z) {}
				
			T nx, ny, nz;
		};
	};
	
	template<typename T>
	class Index : public virtual TGen::VertexStructure {
	public:
		Index() {AddElement(TGen::ColorIndexElement, TGenDataType<T>().type, 1); }
			
		class Type {
		public:
			Type() {}
			Type(const T & index) : index(index) {}
				
			T index;
		};
	};
	
	template<typename T, int unit>
	class TexCoord2 : public virtual TGen::VertexStructure {
	public:
		TexCoord2() {AddElement(TGen::TexCoordElement, TGenDataType<T>().type, 2, false, unit); }
			
		class Type {
		public:
			Type() {}
			Type(const TGen::Vector2 & texcoord) : u(texcoord.x), v(texcoord.y) {}
			
			T u, v;
		};
	};
	
	template<typename T, int unit>
	class SharedTexCoord2 : public virtual TGen::VertexStructure {
	public:
		SharedTexCoord2() {AddElement(TGen::TexCoordElement, TGenDataType<T>().type, 2, true, unit); }
			
		class Type {
		public:
			Type() {}
			Type(int hullo) {}
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
	
	
	typedef JoinVertex2<Vertex3<float>, Color4<float> > Vertex3Color4;
	typedef JoinVertex3<Vertex3<float>, Color4<float>, TexCoord2<float, 0> > Vertex3Color4TexCoord02;
	typedef JoinVertex4<Vertex3<float>, Color4<float>, TexCoord2<float, 0>, Normal3<float> > Vertex3Color4TexCoord02Normal3;
	typedef JoinVertex2<Vertex3<float>, TexCoord2<float, 0> > Vertex3TexCoord02;
	typedef JoinVertex3<Vertex3<float>, TexCoord2<float, 0>, Normal3<float> > Vertex3TexCoord02Normal3;
	typedef Index<unsigned int> Index32;
	typedef Index<unsigned short> Index16;
	typedef Index<unsigned char> Index8;
	
	
} // !TGen

#endif // !_TGEN_VERTEXSTRUCTURE_H

