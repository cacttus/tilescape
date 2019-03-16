/*

	Helicopter camera structure
	Derek page
	Aug 12, 2007

a camera structure focused on one point
*cylindrical*
--------------------------------------*/

#ifndef __HELICAM__CLASS__H__
#define __HELICAM__CLASS__H__

#include "InitD3D.h"

class Helicam_Class{
private:
	D3DXVECTOR3 pos;		//camera position
	D3DXVECTOR3 lookat;		//what the camera looks at
	D3DXVECTOR3 up;
	float Rho,Theta,Height;	//Radius, XY rotation, height. In Radians.
	
	float Zoom_MaxDist, Zoom_MinDist; //Max and Min zoom distances.  Help control limits and calibrate zoom incraments.
	
	POINT MouseLastPos;

public:
	void Init(D3DXVECTOR3 Pos);
	void Rotate(float Amt);				//Rotate camera, in degrees
	void Zoom(float Amt);				//Zoom camera out/in
	void Update_Mouse(int w, int h);	//Calculates camera based on mouse movement
	void Update(int w, int h);			//Update the camera, w,h = screen dimensions

};

#endif