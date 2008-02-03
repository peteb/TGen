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
#include "vector4.h"
#include "vector3.h"
#include "vector2.h"
#include "color.h"
#include "renderer_types.h"
#include <vector>

// TODO: aligna i vertex data på olika boundaries, vanligast 32bit

namespace TGen {
	enum VertexElementType {
		VertexElementCoord = 1,
		VertexElementNormal,
		VertexElementColor,
		VertexElementColorIndex,
		VertexElementEdgeFlag,
		VertexElementTexCoord,
		VertexElementAttribute,
		VertexElementEnd = 0xFF,
	};
		
	class VertexElement {
	public:
		VertexElement();
		VertexElement(TGen::VertexElementType type, TGen::FormatType dataType, uchar count, bool shared = false, uchar unit = 0, bool normalize = false);
		
		VertexElementType type;
		FormatType dataType;
		uchar count, unit;
		bool shared, bound, normalize;
		uint boundValue;
	};
	
	class VertexStructure {
	public:	
		VertexStructure();
		//VertexStructure(TGen::VertexElement * elements);
		~VertexStructure();
		
		int getElementCount() const;
		FormatType getElementDataType(int num) const;
		VertexElement getElementAtComponent(int component) const;
		void getElement(int num, TGen::VertexElement & ret);
		void getElement(int num, TGen::VertexElement & ret) const;
		TGen::VertexElement & getElementAt(int num);
		
		int getStride() const;
		int getSize() const;
		int getComponentCount() const;
		void addElement(TGen::VertexElementType type, FormatType dataType, uchar count, bool shared = false, uchar unit = 0, bool normalize = false);
		void addElement(const std::string & format);
		const std::string & getStringRepresentation() const;
		
		bool hasCoordElements, hasNormalElements, hasColorElements, hasEdgeElements;
		bool hasTexCoordUnitElements[8];
		
	private:
		std::vector<TGen::VertexElement> elements;
		int align;
		mutable bool changed;
		mutable std::string cachedRepresentation;
	};
		
	template<typename T>
	class Vertex4 : public virtual TGen::VertexStructure {
	public:		
		Vertex4() {addElement(TGen::VertexElementCoord, TGenDataType<T>().type, 4); }
			
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
		Vertex3() {addElement(TGen::VertexElementCoord, TGenDataType<T>().type, 3); }
		
		class Type {
		public:
			Type() {}
			Type(const TGen::Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z) {}
			
			operator TGen::Vector3() const {
				return TGen::Vector3(x, y, z);
			}
			
			T x, y, z;
		};
	};	
	
	template<typename T>
	class Vertex2 : public virtual TGen::VertexStructure {
	public:		
		Vertex2() {addElement(TGen::VertexElementCoord, TGenDataType<T>().type, 2); }
			
		class Type {
		public:
			Type() {}
			Type(const TGen::Vector2 & vector) : x(vector.x), y(vector.y) {}
			
			operator TGen::Vector2() const {
				return TGen::Vector2(x, y);
			}
			
			T x, y;
		};
	};	
	
	
	template<typename T>
	class Color3 : public virtual TGen::VertexStructure {
	public:
		Color3() {addElement(TGen::VertexElementColor, TGenDataType<T>().type, 3); }
			
		class Type {
		public:
			Type() {}
			Type(const TGen::Color & color) : r(color.r), g(color.g), b(color.b) {}
				
			operator TGen::Color() const {
				return TGen::Color(r, g, b);
			}
			
			T r, g, b;
		};
	};	
	
	template<typename T>
	class Color4 : public virtual TGen::VertexStructure {
	public:
		Color4() {addElement(TGen::VertexElementColor, TGenDataType<T>().type, 4); }

		class Type {
		public:
			Type() {}
			Type(const TGen::Color & color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
			
			operator TGen::Color() const {
				return TGen::Color(r, g, b, a);
			}			
			
			T r, g, b, a;
		};
	};
	
	template<typename T>
	class Normal3 : public virtual TGen::VertexStructure {
	public:
		Normal3() {addElement(TGen::VertexElementNormal, TGenDataType<T>().type, 3); }
			
		class Type {
		public:
			Type() {}
			Type(const TGen::Vector3 & normal) : nx(normal.x), ny(normal.y), nz(normal.z) {}

			operator TGen::Vector3() const {
				return TGen::Vector3(nx, ny, nz);
			}
			
			T nx, ny, nz;
		};
	};
	
	template<typename T>
	class Index : public virtual TGen::VertexStructure {
	public:
		Index() {addElement(TGen::VertexElementColorIndex, TGenDataType<T>().type, 1); }
			
		class Type {
		public:
			Type() {}
			Type(const T & index) : index(index) {}
				
			operator T() const {
				return index;
			}
			
			Type & operator += (const T & value) {
				index += value;
				return *this;
			}

			Type & operator -= (const T & value) {
				index -= value;
				return *this;
			}
			
			T index;
		};
	};
	
	template<typename T, int unit>
	class TexCoord2 : public virtual TGen::VertexStructure {
	public:
		TexCoord2() {addElement(TGen::VertexElementTexCoord, TGenDataType<T>().type, 2, false, unit); }
			
		class Type {
		public:
			Type() {}
			Type(const TGen::Vector2 & texcoord) : u(texcoord.x), v(texcoord.y) {}
			
			operator TGen::Vector2() const {
				return TGen::Vector2(u, v);
			}
			
			T u, v;
		};
	};
	
	template<typename T, int unit>
	class TexCoord3 : public virtual TGen::VertexStructure {
	public:
		TexCoord3() {addElement(TGen::VertexElementTexCoord, TGenDataType<T>().type, 3, false, unit); }
		
		class Type {
		public:
			Type() {}
			Type(const TGen::Vector3 & texcoord) : u(texcoord.x), v(texcoord.y), s(texcoord.z) {}
			
			operator TGen::Vector3() const {
				return TGen::Vector3(u, v, s);
			}
			
			T u, v, s;
		};
	};
	
	template<typename T, int unit>
	class SharedTexCoord2 : public virtual TGen::VertexStructure {
	public:
		SharedTexCoord2() {addElement(TGen::VertexElementTexCoord, TGenDataType<T>().type, 2, true, unit); }
			
		class Type {
		public:
			Type() {}
			Type(int hullo) {}
		};
	};

	template<int name, typename T, bool normalize = false>
	class VertexAttribute1 : public virtual TGen::VertexStructure {
	public:
		VertexAttribute1() {addElement(TGen::VertexElementAttribute, TGenDataType<T>().type, 1, false, name, normalize); }
		
		class Type {
		public:
			Type() {}
			Type(T value) : value(value) {}
			
			T value;			
		};			
	};
	
	template<int name, typename T, bool normalize = false>
	class VertexAttribute2 : public virtual TGen::VertexStructure {
	public:
		VertexAttribute2() {addElement(TGen::VertexElementAttribute, TGenDataType<T>().type, 2, false, name, normalize); }
			
		class Type {
		public:
			Type() {}
			Type(T x, T y) : x(x), y(y) {}
			Type(const TGen::Vector2 & vector) : x(vector.x), y(vector.y) {}
			
			operator TGen::Vector2() const {
				return TGen::Vector2(x, y);
			}
			
			T x, y;			
		};			
	};

	template<int name, typename T, bool normalize = false>
	class VertexAttribute3 : public virtual TGen::VertexStructure {
	public:
		VertexAttribute3() {addElement(TGen::VertexElementAttribute, TGenDataType<T>().type, 3, false, name, normalize); }
		
		class Type {
		public:
			Type() {}
			Type(T x, T y, T z) : x(x), y(y), z(z) {}
			Type(const TGen::Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z) {}
			
			operator TGen::Vector3() const {
				return TGen::Vector3(x, y, z);
			}
			
			T x, y, z;
		};			
	};
	
	template<int name, typename T, bool normalize = false>
	class VertexAttribute4 : public virtual TGen::VertexStructure {
	public:
		VertexAttribute4() {addElement(TGen::VertexElementAttribute, TGenDataType<T>().type, 4, false, name, normalize); }
			
		class Type {
		public:
			Type() {}
			Type(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
			Type(const TGen::Vector4 & vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}
			Type(const TGen::Color & color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
			
			operator TGen::Vector4() const {
				return TGen::Vector4(x, y, z, w);
			}
			
			operator TGen::Color() const {
				return TGen::Color(r, g, b, a);
			}
			
			union {T x, r; };
			union {T y, g; };
			union {T z, b; };
			union {T w, a; };
		};			
	};
	
	template<class A, class B>
	class JoinVertexElements2 : public A, public B {
	public:
		JoinVertexElements2() : A(), B() {}
			
		class Type : public A::Type, public B::Type {
		public:
			Type() : A::Type(), B::Type() {}			
			template<typename p1, typename p2> Type(p1 a, p2 b) : A::Type(a), B::Type(b) {}
		};
	};
	
	template<class A, class B, class C>
	class JoinVertexElements3 : public A, public B, public C {
	public:
		JoinVertexElements3() : A(), B(), C() {}
			
		class Type : public A::Type, public B::Type, public C::Type {
		public:
			Type() : A::Type(), B::Type(), C::Type() {}			
			template<typename p1, typename p2, typename p3> Type(p1 a, p2 b, p3 c) : A::Type(a), B::Type(b), C::Type(c) {}
		};
	};
	
	template<class A, class B, class C, class D>
	class JoinVertexElements4 : public A, public B, public C, public D {
	public:
		JoinVertexElements4() : A(), B(), C(), D() {}
			
		class Type : public A::Type, public B::Type, public C::Type, public D::Type {
		public:
			Type() : A::Type(), B::Type(), C::Type(), D::Type() {}			
			template<typename p1, typename p2, typename p3, typename p4> Type(p1 a, p2 b, p3 c, p4 d) : A::Type(a), B::Type(b), C::Type(c), D::Type(d) {}
		};
	};

	template<class A, class B, class C, class D, class E>
	class JoinVertexElements5 : public A, public B, public C, public D, public E {
	public:
		JoinVertexElements5() : A(), B(), C(), D(), E() {}
			
		class Type : public A::Type, public B::Type, public C::Type, public D::Type, public E::Type {
		public:
			Type() : A::Type(), B::Type(), C::Type(), D::Type(), E::Type() {}			
			template<typename p1, typename p2, typename p3, typename p4, typename p5> Type(p1 a, p2 b, p3 c, p4 d, p5 e) : A::Type(a), B::Type(b), C::Type(c), D::Type(d), E::Type(e) {}
		};
	};	
	
	
	typedef JoinVertexElements2<Vertex3<float>, Color4<float> > Vertex3Color4;
	typedef JoinVertexElements3<Vertex3<float>, Color4<float>, TexCoord2<float, 0> > Vertex3Color4TexCoord02;
	typedef JoinVertexElements4<Vertex3<float>, Color4<float>, TexCoord2<float, 0>, Normal3<float> > Vertex3Color4TexCoord02Normal3;
	typedef JoinVertexElements2<Vertex3<float>, TexCoord2<float, 0> > Vertex3TexCoord02;
	typedef JoinVertexElements3<Vertex3<float>, TexCoord2<float, 0>, Normal3<float> > Vertex3TexCoord02Normal3;
	typedef Index<unsigned int> Index32;
	typedef Index<unsigned short> Index16;
	typedef Index<unsigned char> Index8;
	
	
} // !TGen

#endif // !_TGEN_VERTEXSTRUCTURE_H

