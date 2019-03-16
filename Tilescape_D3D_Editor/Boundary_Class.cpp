#include "Boundary_Class.h"
/*
	Volume Type flags
-------------------------------*/
#define VOLUME_SPHERE	0x00000001	//Sphere bounding volume`	``
#define VOLUME_BOX		0x00000002	//Box bounding volume
#define VOLUME_PERPOL	0x00000004	//Per-Polygon volume
#define VOLUME_ELLIPSE	0x00000008	//Elliptical volume
#define VOLUME_LINE		0x00000010	//Line volume
#define VOLUME_PLANE	0x00000020	//Plane volume

/*
	Volume Attribute flags
---------------------------------------*/
#define VOLUME_AA	0x00000100	//Volume is axis aligned


void Boundary_Class::Create( DWORD Type, DWORD Attribs )
{
	Flags |= Type;
	Flags |= Attribs;
/*
	if(Flags & VOLUME_SPHERE) ;
	else if(Flags & VOLUME_BOX) ;
	else if(Flags & VOLUME_PERPOL) ;
	else if(Flags & VOLUME_ELLIPSE) ;
	else if(Flags & VOLUME_LINE) ;
	else if(Flags & VOLUME_PLANE) ;*/

}