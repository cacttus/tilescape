/*----------------------------------------
		Directx mesh class (my own)
		Derek Page - Programmer
		Sept 9, 2007
----------------------------------------*/

#ifndef __MESH__CLASS__H__
#define __MESH__CLASS__H__
#include "InitD3D.h"
#include "Vertex.h"
#include "Polygon_Class.h"
#include "Cube_Class.h"

class Tile_Picker_Class;
class Tile_Controller_Class;
class Texture_Controller_Class;
//used to map the index buffer to
//an easier-to-manage list of
//triangles.
struct Tri_16 {
	unsigned short p1;
	unsigned short p2;
	unsigned short p3;
};
struct Tri_32 {
	unsigned int p1;
	unsigned int p2;
	unsigned int p3;
};
class Mesh_Class {

	friend class Tile_Controller_Class;
	friend class Tile_Picker_Class;

	Texture_Controller_Class*	pTC;
	IDirect3DVertexBuffer9*		pVertexBuffer;		// - Vertex Buffer.
	IDirect3DIndexBuffer9*		pIndexBuffer;		// - Index Buffer, this buffer is never actually bound for rendering.
	IDirect3DIndexBuffer9*		pRenderBuffer;		// - Buffer used for rendering, filled each frame.

	Frustum_Class*				pFrustum;			// - Pointer to the global frustum for the world. It should already be updated before calling anything that depends on it.
	Cube_Class					BoundCube;			// - The cube that bounds this mesh.  Calculated in CreateAttributes().

	Vertex*						pVertices;

	DWORD						Index;				// - Mesh index.

	DWORD*						pAttributes;		// - Attribute identifyers.
	DWORD						nAttributes;		// - Size of above array

	DWORD						nAttribSets;		// - Number of each individual attrib (e.g. 0,25,33 = 3  ) 
	DWORD*						pAttribKey;			// - A buffer containing one of each of the attributes, and the number of them e.g. [2,1024,3,56] ( created in MakeAttribKey )

	BOOL*						pCanDrawPoly;		// - Array of BOOL values specifying if the polygon can be drawn

	DWORD						Num_Vertices;		// - Number of vertices.
	DWORD						Num_Faces;			// - Number of faces.

	DWORD						Buf_Faces;			// - Number of faces in the rendering buffer.
	DWORD						Buf_Vertices;		// - Number of vertices in the rendering buffer.

	bool						bCanDraw;			// - Not accessed by the implementor, managed by the Vis_*() functions.


	void	MakeAttribKey( );	// - Returns the number of different attributes in the attribute buffer.
	void	SortAttributes();	// - Performs a linear sort of attributes.
		
	DWORD	GetNumAttribsByType( DWORD attrib );	// - Returns the number of attributes in the attribute buffer, based on the attribute type.
	DWORD	GetNumAttribs();						// - Gets the total number of attribute types.
	

	void	FillVertexBuffer( );					// - Re-Fills the vertex buffer for this mesh, should be done when the mesh is altered. - slow
	void	UpdateSingle( DWORD offset );			// - Updates the vertex buffer with one vertex at a certain offset, faster for single operations.
	void	UpdateRange( DWORD offset, DWORD range ); // - Updates the buffer at a range.
	
	void	FillRenderBuffer( DWORD AttribID );		// - Fills the rendering index buffer before the call to "Render()".
	void	CND_FillRenderBuffer( DWORD AttribID );				// - Fills the render buffer on a conditinal basis (if the polygon is visible);

	void	Vis_Quick();		// - First mesh visibility test, tests if the entire mesh is visible or not, and if it needs to be rendered
	void	Vis_PerPoly();		// - Does some Backface, occlusion, and any other per-poly test. Called second in the Render() function

	void	AddTilePolys( DWORD TileOffset );	// - Adds the polygons of a tile at the specified offst, in vertices.
	void	ResetDrawBuffer();				// - Resets the boolean polygon drawing buffer to all 1s
	void	ShiftBuf( DWORD* &a1, DWORD* &a2, Tri_16* t1, Tri_16* t2);
	//void	ShiftBuf(Tri_16* &attrib1, Tri_16* &attrib2);
public:

	void Init( DWORD nVerts, DWORD nFaces );			// - Initialize the structure
	
	void Chg_Attrib( DWORD attrib, DWORD newattrib );	// - Changes attrib into newattrib.
	void Add_Attrib( DWORD attrib );	// - Programatically adds an attribute to the buffer, and updates the key if necessary.
	void Rem_Attrib( DWORD attrib );	// - Programatically removes an attribute from the attrib buffer, and updates the key.
	
	void CreateAttributes();			// - Has the same effect as calling MakeAttribKey() then SortAttributes();

	void Render( DWORD AttribID );		// - Render the pRenderBuffer.
	
	void Free();
	
	Mesh_Class();
};



#endif