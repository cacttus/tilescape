/*------------------------------------

	Controls the tiles.

	Derek Page - Programmer
	Sept 9, 2007

	tiles on one row are numbered 
	from 0 to 15 or 0 to SubAxis_Tiles-1.

	Tiles are created per-mesh instead
	of per-landscape e.g. 0-15 per row of each mesh


------------------------------------*/

#ifndef __TILE__CONTROLLER_CLASS__H__
#define __TILE__CONTROLLER_CLASS__H__


#include "Tile_Class.h"
#include "InitD3D.h"
#include "Mesh_Class.h"
#include "stdlib.h"

/*
#include <vector>
#include <map>
*/
/* Quick structure to hold 12 float values */
struct Float12{
	FLOAT Val[12];
};
struct Float12Ptr{
	FLOAT* Val[12];
};

/* Quick little structure to hold the contents of a tile hit */
struct Hit_Struct {//24 bytes

	FLOAT		U, V, UV;	// - The barycentric coords.
	DWORD		Offset;		// - Offset into the array of vertexes.
	BOOL		Tri;		// - Which triangle was hit, 0 or 1.
	Mesh_Class* pMesh;		// - Pointer to the mesh that contains the vertexes.
	int			TriVert;	// - Which vertex (0 to 3, 4 is all) was hit on the tile.

};

class Tile_Controller_Class {
	
	Texture_Controller_Class*	pTC;		// - Pointer to the texture controller class.
	Frustum_Class*				pFR;		// - Pointer to the frustum class.

	Tile_Class*					pTiles;		// - The array of tile structures.
	Mesh_Class**				pMeshes;	// - Array of meshes, to which the whole land is split.

	UINT	Num_Meshes;			// - Number of submeshes.

	DWORD	Num_Tiles;			// - Number of tiles.
	DWORD	Axis_Tiles;			// - Number of tiles on one axis.

	DWORD	SubAxis_Tiles;		// - Number of axis tiles on one mesh.

	FLOAT	Tile_Width;			// - Width of a tile.
	DOUBLE	Base_Height;		// - Height of base tiles.

	void Generate( int Axis );	// - Generates the meshes for this structure Axis is the max number of tiles on x/z axis.
	
	void MeshVisibility();		// - Checks mesh visibility, updating the boolean buffer.
	void PostMeshVisFn( );		// - Called after the mesh's first visibility function.
	void PostFormUpdate();		// - Update the mesh buffers relative to the tiles, if the topology of the mesh(s) have changed
	
	void RaiseVert_TL(Mesh_Class* pm, DWORD offset );	// - Raise the top left vertex of a tile
	void RaiseVert_TR(Mesh_Class* pm, DWORD offset );	// - Raise the top right vertex of a tile
	void RaiseVert_BL(Mesh_Class* pm, DWORD offset );	// - Raise the Bottom left vertex of a tile
	void RaiseVert_BR(Mesh_Class* pm, DWORD offset );	// - Raise the Bottom right vertex of a tile.
	
	void CheckBasesVisible( Mesh_Class* pm, DWORD offset );		// - Checks to see if the tile at Offset's bases are visible.
	void CheckBaseCull( Mesh_Class *pm, DWORD offset, DWORD v0, DWORD dv0, DWORD v1, DWORD dv1, WORD side );
	void CullBases( Mesh_Class* pm, DWORD offset, DWORD plane );

	void Check_MeshTilesVisible( Mesh_Class* pm );	// - if the mesh is visible, Checks to see if the tiles in a mesh are visible


public:
	void Init2( int Num_Axis_Tiles, Texture_Controller_Class* ptc, Frustum_Class* pfr   ); // - Must be called before the structure is used.
	
	void RaiseSingleTile(  Hit_Struct* phs );		// - Raise a tile, not tile dependant(hill).
	void LowerSingleTile(  Hit_Struct* phs );		// - Lower a tile, not tile dependant(hill).
	
	void RaiseHillTile( Hit_Struct* phs );	// - Raises a single tile, as if it were a hill
	//void LowerHillTile( Hit_Struct* phs );

	void PickTile( POINT* pCursorPos, Hit_Struct* &pHitStruct ); // - Picks a tile based on the location of the cursor, returns an offset into, the returned mesh
	void Update();
	void Render();
	
	void Free();
	
};




#endif