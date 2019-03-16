/*
	Cube class.
	Programmer: Derek Page
	Sept 15, 2007.
	 - Note:
	Not a cube, but a hexahedron.
	Cube is smaller though.
	 - Point Reference (for detail see documentation):
	 tl=max=0, bl=3
	 tl+depth=4, bl+depth=7

*/
#ifndef __CUBE__CLASS__H__
#define __CUBE__CLASS__H__

#include "InitD3D.h"
#include "Vertex.h"
#include "Frustum_Class.h"

class Cube_Class {

	D3DXVECTOR3 P[8];	// - Individual points of the cube
	D3DXVECTOR3 V[8];	// - Individual points of the cube in viewport space, recalculated each frame.

	D3DXVECTOR3 Min, Max;

	D3DXVECTOR3 Pos;	// - Position of the cube.
	
public:
	Cube_Class(){memset(this,0,sizeof(Cube_Class)); Min=D3DXVECTOR3(1000000, 1000000, 1000000); Max=D3DXVECTOR3(-1000000, -1000000, -1000000); }
	
	/* Initialization */
	void _Calc( Vertex* verts, DWORD numverts );	// - Calculate the cube based on a set of vertices
	void _ReCalc( LPD3DXMESH pMesh, D3DXMATRIX* appmatrix );				// - Will calculate the box based on a d3dxmesh structure
	void _Rotate( float degrees );	// - Rotate the cube along it's center axis
	
	/* Runtime */
	void _Update( D3DXMATRIX* pmat );
	void _Update( D3DXVECTOR3* ppos );
	BOOL _Visible( Frustum_Class* pFrustum );	// - Checks to see if the cube is visible on the screen.
	
	void _DrawThis();
};


#endif