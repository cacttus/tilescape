/*---------------------------------
|	Direct3D Vertex definition

	note: Changing this will change
	all the vertices of the whole
	program.  All structures that
	define vertices explicitly use
	this structure.
---------------------------------*/


#ifndef __VERTEX__H__
#define __VERTEX__H__

#include "InitD3D.h"

//FVF_B1 are blending weights
#define FVF_DESCRIPTOR ( D3DFVF_XYZ | D3DFVF_TEX3  )// | D3DFVF_DIFFUSE ) D3DFVF_NORMAL

struct Vertex 
{
	float x, y, z; //some errors might occour, changed to double
	float u, v;
	float u2, v2;
	float u3, v3;
	//D3DXVECTOR3 Normal;// nx, ny, nz;
	//DWORD color;
	Vertex operator=(Vertex V)
	{
		x=V.x; 
		y=V.y; 
		z=V.z;
		u=V.u; 
		v=V.v;
		return V;
	}
	Vertex operator-(float f)
	{
		Vertex v;
		v.x=(float)x-f; v.y=(float)y-f; v.z=(float)z-f;
		return v;
	}
};

#endif