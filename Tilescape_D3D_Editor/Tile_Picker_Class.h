/*---------------------------------------
	Derek Page
	Aug ?? 2007
	Tile Picker Class
	A project dependent class.
	This class controls the drawing
	of outlines on tiles.
	The outlines on the tiles aid the
	user in tile selection and mani-
	pulation.

---------------------------------------*/


#ifndef __TILE__PICK__CLASS__H__
#define __TILE__PICK__CLASS__H__

#include "InitD3D.h"
#include "Vertex.h"
#include "Tile_Controller_Class.h"

class Tile_Picker_Class 
{


IDirect3DVertexBuffer9* D3DLineBuffer; 		// - The buffer to hold the grid.
Vertex*					LineBuffer;			// - The line vertices for the grid buffer, Pointers to the actual Polygonal data.
Tile_Controller_Class*	pTileController;	// - Pointer to the global tile controller.

UINT					VertsInBuf;			// - Number of verts in the buffer.
UINT					PrimCount;			// - Number of primitives to render.
UINT					LastVertSelected;	// - The last vertex selected. 0, 1, 2, 3 or 4 for all of them.

BOOL					b_VertexMode;		// - Specifies if the tile picker is drawing independent vertices.
BOOL					b_MultiTileMode;	// - Specifies that the tile picker should draw multiple tiles.
UINT					nSelectedAxis;		// - Number of tiles on a single axis of selection.


public:
	Hit_Struct*				pHitStruct;		// - Pointer to the hit structure to hold tile hit information.

	Tile_Picker_Class();
	void Init( Tile_Controller_Class* ptc );
	UINT GetLastVertSelected(){ return LastVertSelected; }
	void Update();
	void Render();
	void Free();
};


#endif