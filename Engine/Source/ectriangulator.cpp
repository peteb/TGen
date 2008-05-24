/*
 *  ectriangulator.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "ectriangulator.h"

void TGen::Engine::EarClippingTriangulator::add(const TGen::Vector3 & point) {
	points.push_back(point);
}

void TGen::Engine::EarClippingTriangulator::triangulate(std::vector<unsigned int> & indices) {
	process(indices);
}

bool TGen::Engine::EarClippingTriangulator::insideTriangle(const TGen::Vector3 & A, const TGen::Vector3 & B, const TGen::Vector3 & C, const TGen::Vector3 & P) {
	float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
	float cCROSSap, bCROSScp, aCROSSbp;
	
	ax = C.x - B.x;  ay = C.y - B.y;
	bx = A.x - C.x;  by = A.y - C.y;
	cx = B.x - A.x;  cy = B.y - A.y;
	apx = P.x - A.x;  apy = P.y - A.y;
	bpx = P.x - B.x;  bpy = P.y - B.y;
	cpx = P.x - C.x;  cpy = P.y - C.y;
	
	aCROSSbp = ax * bpy - ay * bpx;
	cCROSSap = cx * apy - cy * apx;
	bCROSScp = bx * cpy - by * cpx;
	
	return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
}

float TGen::Engine::EarClippingTriangulator::area() {
	int n = points.size();
	float A = 0.0f;
	
	for (int p = n - 1, q = 0; q < n; p = q++) {
		const TGen::Vector3 & pval = points[p];
		const TGen::Vector3 & qval = points[q];
		A += pval.x * qval.y - qval.x * pval.y;
	}
	
	return A * 0.5f;
}

bool TGen::Engine::EarClippingTriangulator::snip(int u, int v, int w, int n, int * V) {
	const TGen::Vector3 & A = points[(V[u])];
	const TGen::Vector3 & B = points[(V[v])];
	const TGen::Vector3 & C = points[(V[w])];
	
	if (std::numeric_limits<float>::epsilon() > abs(((B.x - A.x) * (C.y - A.y)) - ((B.y - A.y) * (C.x - A.x))) )
		return true;
	
	for (int p = 0; p < n; p++) {
		if ((p == u) || (p == v) || (p == w))
			continue;
		
		const TGen::Vector3 & P = points[(V[p])];
		
		if (insideTriangle(A, B, C, P))
			return false;
	}
	
	return true;
}

void TGen::Engine::EarClippingTriangulator::process(std::vector<unsigned int> & indices) {
	const int n = points.size();
	if (n < 3)
		return;
	
	int *V = new int[n];
	if (0.0f < area()) {
		for (int v = 0; v < n; v++)
			V[v] = v;
	}
	else {
		for (int v = 0; v < n; v++)
			V[v] = (n - 1) - v;
	}
	
	int nv = n;
	int count = 2 * nv;
	
	for (int m = 0, v = nv - 1; nv > 2;) {
		if (0 >= (count--))
			return;
		
		int u = v;
		if (nv <= u)
			u = 0;
		v = u + 1;
		if (nv <= v)
			v = 0;
		int w = v + 1;
		if (nv <= w)
			w = 0;
		
		if (snip(u, v, w, nv, V)) {
			int a, b, c, s, t;
			a = V[u];
			b = V[v];
			c = V[w];
			
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			m++;
			
			for (s = v, t = v + 1; t < nv; s++, t++)
				V[s] = V[t];
			
			nv--;
			count = 2 * nv;
		}
	}
	
	delete V;
}

