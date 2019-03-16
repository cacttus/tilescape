/*----------------------------------------
	View frustum class.
	Calculates the view frustum each
	frame.
	Programmer: Derek Page
	Sept 15, 2007
	Planes:
	n,f,l,r,t,b
	For detail see documentation.
----------------------------------------*/
#ifndef __FRUSTUM__CLASS__H__
#define __FRUSTUM__CLASS__H__

#include "InitD3D.h"


class Frustum_Class {
	
	friend class Cube_Class;

	D3DXPLANE Planes[6];

public:

	void Update();
	BOOL _Point( D3DXVECTOR3 p );		// - Checks for a point inside the frustum

};


#endif