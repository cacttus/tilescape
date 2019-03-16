/*-------------------------------------

-------------------------------------*/

#ifndef __D3D__AllocHierarchy__H__
#define __D3D__AllocHierarchy__H__
#define _CRT_SECURE_NO_DEPRECATE
#include <assert.h>

#include "InitD3D.h"
#include "D3D_Mesh.h"

#include "Texture_Controller.h"

#include "Commdef.h"

class D3D_Mesh;

/*
	D3D_Frame
	-derived from ID3DXFrame
	-used to hold the transform matrix
	 of each frame.(like when traversing a model)
*/
struct D3D_Frame : public D3DXFRAME
{
	D3DXMATRIX CombinationMatrix;
};

/*

	Mesh Container to hold
	mesh and animation data

-------------------------------*/
struct D3D_MeshContainer : D3DXMESHCONTAINER
{

    /// The mesh that does the dirty work for this container.
	// This mesh is a blended mesh with vertex weights
    ID3DXMesh* pMesh;

    // Bone offset matrices retrieved from the D3DXMESHCONTAINER::pSkinInfo interface
    D3DXMATRIX* pBoneMatrixOffsets;

    /// Array used to look up transformation matrices.  The only function in this class,
    /// createBonePointers, sets up this array.
    ///     @see createBonePointers
    D3DXMATRIX** ppBoneMatrixPointers;

    /// Maximum number of matrix influences on a single face
    DWORD dwMaxFaceInfluences;

    /// How many different subsets the mesh has
    DWORD dwNumAttributeGroups;

    /// Buffer in which bones are combined to do animation
    ID3DXBuffer* pBoneCombinationBuffer;

    /// Which subset of the mesh to begin rendering with software
    DWORD dwStartSoftwareRenderAttribute;

	//Variables to reference textures from a Texture_Controller structure
	int Num_Textures;
	int* Textures;
	bool No_Texture; //specifies if the sub-mesh has no texture

    /**
     * Utility function to initialize the bone pointers for this mesh container
     *   @param pFrameRoot Root frame from which to generate pointers
     *   @return Result of the function
     */
    HRESULT CreateBonePointers( D3D_Frame * pFrameRoot );
};

/* 
	
	Class that is used in the fn
	D3DXLoadMeshHierarchyFromX()
	it MUST be derived.
	Holds all animation related data
	for a DirectX .X mesh.
	Warning! do not use memset in the
	constructor of this class!

----------------------------------*/

class D3D_AllocHierarchy : public ID3DXAllocateHierarchy 
{
	STDMETHOD(CreateFrame)( THIS_ LPCTSTR Name, LPD3DXFRAME *ppFrame );

	STDMETHOD(CreateMeshContainer)( THIS_  LPCSTR Name,
	  const D3DXMESHDATA * pMeshData,
	  const D3DXMATERIAL * pMaterials,
	  const D3DXEFFECTINSTANCE * pEffectInstances,
	  DWORD pNumMaterials,
	  const DWORD * pAdjacency,
	  LPD3DXSKININFO pSkinInfo,
	  LPD3DXMESHCONTAINER * ppNewMeshContainer );

	STDMETHOD(DestroyFrame)( THIS_  LPD3DXFRAME pFrameToFree );

	STDMETHOD(DestroyMeshContainer)( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree );
public:

	Texture_Controller_Class*	p_TexController;
	D3D_Mesh*					pWrapperMesh;	//the parent mesh controller

	void Set_TexControllerPointer( Texture_Controller_Class* ptc );
	void Set_Wrapper( D3D_Mesh* m );
    DWORD m_dwMaxBlendedMatrices;//don't know, used in the allochierarchy mesh function
}; 


#endif