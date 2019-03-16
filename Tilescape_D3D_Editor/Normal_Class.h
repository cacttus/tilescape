//handles normals!


#ifndef __NORMAL__H__
#define __NORMAL__H__

#include "InitD3D.h"

class Normal_Class
{
	D3DXVECTOR3 Normal;
public:
	void Make_From_Plane( D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3 );		//Makes a plane normal from three points
	void Make_From_Vertex( D3DXVECTOR3 PlaneNormal, D3DXVECTOR3 Vertice );		//Makes a vertex normal, from a vertice and a plane normal
	D3DXVECTOR3* Get_Normal(){ return &Normal; }
	Normal_Class(){ memset(this,0,sizeof(Normal_Class)); }

};

#endif