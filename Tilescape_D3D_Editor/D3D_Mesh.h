#ifndef __D3D__MESH__H__
#define __D3D__MESH__H__

#include "InitD3D.h"
#include "D3D_AllocHierarchy.h"
#include "Texture_Controller.h"
#include "Cube_Class.h"
#include "Frustum_Class.h"

class Mesh_Controller_Class; //forward decl

struct D3D_Frame;	//forward decl

/*
	D3D_Mesh:	Structure to contain mesh attributes.
	The highest structure, submeshes are stored
	in D3D_MeshContainer
	Derek Page sept 2 2007
*/
class D3D_Mesh
{
	friend class				D3D_AllocHierarchy;
	friend class				Mesh_Controller_Class;

	D3D_AllocHierarchy*			pAllocHierarchy;	// - Hierarchy allocation class.

	int							Num_Animations;		// - Number of animations.
	int							Cur_Animation;		// - Current animation.
	int							Cur_Track;			// - Current track being played.

	int							Ref_Count;			// - Reference count, when it equals 0 the mesh is released.
	DWORD						ID;					// - Mesh Unique identifyer.
	D3D_Mesh*					pNext;				// - Pointer to the next mesh.

	bool						bCanDraw;			// - If the mesh can be drawn.

	/* Drawing */
	void		DrawFrames( LPD3DXFRAME pFrame );				// - Draws all the frames of pFrame( all children and siblings ).
	HRESULT		DrawFrameMesh( LPD3DXFRAME pFrame );			// - Draws the mesh of pFrame (a single frame).

	/* Transformation */
	void		UpdateTransform( D3DXFRAME* pFrame, D3DXMATRIX* ParentMatrix ); //called in UpdateFrames.

	/* Animation */
	//void GetTrackIDByName( char* Name );						// - can be used to retrieve a track id by it's name.
	void		Update_Animation(  D3DXMATRIX* matWorld );		// - Updates all the matrices in all the frames, for animation.

	/* Calculations */
	void		Calc_BoundBox( LPD3DXFRAME pFrame );		// - Calculate the bound box for the mesh.
	
	void		CheckVisible(Frustum_Class* pFrustum, D3DXVECTOR3* Pos);
	void		CheckVisible(Frustum_Class* pFrustum, D3DXMATRIX* Mat);

protected:
	Cube_Class*					pBoundCube;

public:
	LPD3DXANIMATIONCONTROLLER	pAnimController;// - Pointer to the animation controller returned by D3DXLoadMeshHierarchyFromX().
	D3D_Frame *					pFrameRoot;		// - Pointer to a hierarchy of frames loaded with D3DXLoadMeshHierarchyFromX().

	void						Init(Texture_Controller_Class* tc);
	void						Render( D3D_Frame* pRoot_Frame );		// - Renders the mesh
	
	void						SetTrack( int Index );		// - Change the animation track or start one up.
	D3D_AllocHierarchy*			Get_pAllocHierarchy();	// - Returns the animation class ( for use with D3DXLoadMeshHierarchyFromX() ).
	void						Set_ID( DWORD id );
	DWORD						Get_ID();

	int							Get_NumAnimations(){ return Num_Animations; }
	int							Get_CurrentAninimation(){ return Cur_Animation; }
	int							Get_CurTrack(){ return Cur_Track; }

	void						Free();

	D3D_Mesh();
};

#endif