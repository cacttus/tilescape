/*

		Polygon structure
		Derek Page
		Aug 4 07'
-------------------------------------------*/



#ifndef __POLYGON_CLASS__H__
#define __POLYGON_CLASS__H__

#include "Vertex.h"

class Polygon_Class {
public:
	Vertex	p1, p2, p3;		// - Verts.
	bool	bDrawn;			// - if the polygon has been drawn.
	bool	bCanDraw;		// - if the polygon can be drawn.
	DWORD	Group;			// - Texture group this poly belongs to.

	Polygon_Class();
	void operator=(Polygon_Class p);
};

#endif