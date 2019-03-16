/*
	Boundary Structure
----------------------------------------*/

#ifndef __BOUNDARY_CLASS__H__
#define __BOUNDARY_CLASS__H__

//#include <Vector3.h>
#include "InitD3D.h"
#include "polygon.h"

class Boundary_Class{
	float Radius;		//Radius(sphere), or length from center to corner(box)
	float YRadius;		//Y radius (ellipse)
	Plane* Sides;		//sides of volume, can be cube, irregular, etc.
	DWORD Flags;
	Boundary_Class* Parent;	//Parent volume, if any	
	Boundary_Class** Child;	//child volume if any
public:
	float Get_Radius(){ return Radius; }
	float Get_YRadius(){ return YRadius; }

	void Add_Child(){;}
	void Del_Child(){;}
	
	void Create(DWORD Type, DWORD Attribs);

	Boundary_Class(){ memset( this, 0, sizeof( Boundary_Class ) ); }
	
};


#endif