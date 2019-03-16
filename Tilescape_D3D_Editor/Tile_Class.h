/*------------------------------------

	New class to hold tile information.
	Derek Page - Programmer
	9-6-07

------------------------------------*/
#ifndef __TILE__CLASS__H__
#define	__TILE__CLASS__H__

#include "initD3D.h"
#include "object_class.h"
//#include "Polygon_Class.h"
#include "IPolygon_Class.h"

class Tile_Controller_Class;

class Tile_Class {
	
	friend class Tile_Controller_Class;

	bool bCanDraw;			// - If the tile can be drawn

	DWORD Index;			// - The tile index. The offset into the Tile_Controller's tile array.
	
	DWORD Ind_Tex1;			// - Index of the first texture in the Texture_Controller (the ground)
	DWORD Ind_Tex2;			// - Index of the second texture (the path)
	DWORD Ind_Tex3;			// - Index of the third texture (the effect, like snow, leaves etc.)

	DWORD Ind_BTex1;		// - Index of the first base texture (only one i guess)

	IPolygon16_Class TopPolys[2];		// - Indexed top polygons.
	IPolygon16_Class BasePolys[8];		// - Indexed base polygons

	Object_Class* pObject;	// - The object that resides on the tile.

	void Gen_20( /*int I, int J, int W, int BH,*/ DWORD IND, DWORD &REL_IND  );

};


#endif