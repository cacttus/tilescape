/*

		Polygon structure
		Derek Page
		Aug 4 07'
-------------------------------------------*/



#ifndef __POLYGON__H__
#define __POLYGON__H__

#include "Vertex.h"

class Plane{
public:
	Vertex	p1, p2, p3;		// - Verts.
	Plane(){memset(this,0,sizeof(Plane));}
	void operator=(Plane p){

		p1=p.p1;
		p2=p.p2;
		p3=p.p3;

	}
};

#define Polygon Plane

#endif