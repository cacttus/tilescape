/*
	The following code is a combination of code
	from gamedev.net and other websites, and my
	own.
*/

#include "D3D_AllocHierarchy.h"
#define _CRT_SECURE_NO_DEPRECATE


//------------------------------------------------------------------------------------------------
// Name:  CreateBonePointers
// Desc:  Sets up bone pointers in this container
//------------------------------------------------------------------------------------------------
HRESULT D3D_MeshContainer::CreateBonePointers( D3D_Frame * pFrameRoot )
{
    // If skinning information exists, set up bones
    if( pSkinInfo )
    {
        // Get rid of the array if it exists
        //SAFE_DELETE_ARRAY( ppBoneMatrixPointers );
        // Get the number of bones this mesh has
        DWORD dwNumBones = pSkinInfo->GetNumBones();

        // Set up the array
        ppBoneMatrixPointers = new D3DXMATRIX*[ dwNumBones ];

		if(ppBoneMatrixPointers==NULL)
            return E_OUTOFMEMORY;

        // Set up the pointers using frames
        for( DWORD i = 0; i < dwNumBones; ++i )
        {
			//this->
            // Get the frame for this bone
            D3DXFRAME* pFrame = D3DXFrameFind( pFrameRoot, pSkinInfo->GetBoneName(i) );

            // Make sure the frame was found
            if( !pFrame ) return E_FAIL;

            // Set the pointer
            ppBoneMatrixPointers[ i ] = &((D3D_Frame*)(pFrame))->CombinationMatrix;
        }
    }
    else
    {
        // Output information
        //DEBUG_MSG( "MeshContainer::CreateBonePointers:  Mesh has no skinning information" );
		Fatal_Error("Mesh has no skinning information.");
    }

    // Success
    return S_OK;
}





/*------------------------------------------------------------------

	Creates a new frame in the frame hierarchy.

------------------------------------------------------------------*/
HRESULT D3D_AllocHierarchy::CreateFrame( THIS_  LPCTSTR Name, LPD3DXFRAME * ppFrame )
{
	assert(p_TexController);			//make sure that the texture controller is poiting to something
	assert(pWrapperMesh);

	*ppFrame = NULL;						//set to null
	D3D_Frame* frame = NULL;

	frame = new D3D_Frame();		//Allocate the frame and the frame's name

	if( !frame )
	{
		DestroyFrame( (LPD3DXFRAME)frame );
		return E_OUTOFMEMORY;
	}

	memset(frame,0,sizeof(D3D_Frame));
	if( Name )
	{
		//DestroyFrame( (LPD3DXFRAME)frame );
		//return E_OUTOFMEMORY;
		frame->Name = new char[strlen(Name)+1];
		memset(frame->Name, 0, strlen(Name)+1 );
		memcpy(frame->Name, Name, strlen(Name));
	}
	*ppFrame = frame;

	return S_OK;
}

/*---------------------------------------------------------------

	Creates a container to hold mesh and animation data.

---------------------------------------------------------------*/
HRESULT D3D_AllocHierarchy :: CreateMeshContainer (
	 THIS_ LPCSTR Name,
	const D3DXMESHDATA * pMeshData,
	const D3DXMATERIAL * pMaterials,
	const D3DXEFFECTINSTANCE * pEffectInstances,
	DWORD pNumMaterials,
	const DWORD * pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER * ppNewMeshContainer )
{ 
	HRESULT hr;

	//assert( p_TexController); //make sure that a texture controller is created

	*ppNewMeshContainer = NULL;

	//Create a new mesh container.
	D3D_MeshContainer* pMeshContainer = new D3D_MeshContainer();
	memset( pMeshContainer, 0, sizeof(D3D_MeshContainer) );
	
	//Set the name of the mesh container.
	pMeshContainer->Name = new char[strlen(Name)+1];
	memset(pMeshContainer->Name, 0, strlen(Name)+1);
	if(strlen(Name)!=0)memcpy(pMeshContainer->Name, Name, strlen(Name) );

	// copy the mesh over
	pMeshContainer->MeshData.Type = pMeshData->Type;		//copies the type of mesh (a DWORD value)	
	pMeshContainer->MeshData.pMesh = pMeshData->pMesh;		//copies the actual ID3DXMesh or ID3DXPMesh
	pMeshContainer->MeshData.pMesh->AddRef(); //incraments the iunknown interface's pointer; must be called. See http://msdn2.microsoft.com/en-us/library/ms691379.aspx

	// copy the adjacency data over
	DWORD dwNumFaces = pMeshContainer->MeshData.pMesh->GetNumFaces();
	pMeshContainer->pAdjacency = new DWORD[ 3 * dwNumFaces ];
	if( pMeshContainer->pAdjacency == NULL )
	{ 
		hr = E_OUTOFMEMORY; 
		return hr;
	}
	memcpy( pMeshContainer->pAdjacency, pAdjacency, 3 * sizeof( DWORD ) * dwNumFaces );

	/*
		-----Load in all the materials
	*/	
	DWORD nMaterials = pNumMaterials;
	pMeshContainer->pMaterials = new D3DXMATERIAL[max(1,pNumMaterials)];

	/* Identifiers for the texture controller */
	pMeshContainer->Num_Textures = pNumMaterials;
	pMeshContainer->Textures = new int[max(1,pNumMaterials)];//Don't try to allocate nothing

	char TempFileName[400];

	if(nMaterials>0)
	{

		//Copy over all of the materials
		memcpy(pMeshContainer->pMaterials, pMaterials, pNumMaterials * sizeof(D3DXMATERIAL));

		/*
			Check to see if the texture(s) for this is already loaded,
			if so set the identifyer to point to it; else load it 
			and THEN set the identifyer to point to it.
		*/
		if(pMaterials->pTextureFilename)
		{
			for( unsigned int i=0; i<pNumMaterials; ++i )
			{
				if(pMeshContainer->pMaterials[i].pTextureFilename)
				{
					memset(TempFileName,0,400);
					strcat(TempFileName,"Textures\\");
					strcat(TempFileName,pMeshContainer->pMaterials[i].pTextureFilename);
					pMeshContainer->Textures[i] = p_TexController->Add_Texture( TempFileName, TEXID_DEFAULT );
					pMeshContainer->Num_Textures++;

				}
			}
			pMeshContainer->No_Texture=FALSE;
		}
		else	//there is no texture for this sub-mesh
		{
			pMeshContainer->No_Texture=TRUE;
		}
	}
	else//make the default material
	{
		memset( &pMeshContainer->pMaterials[ 0 ].MatD3D, 0, sizeof( D3DMATERIAL9 ) );
        pMeshContainer->pMaterials[ 0 ].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[ 0 ].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[ 0 ].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[ 0 ].MatD3D.Specular = pMeshContainer->pMaterials[ 0 ].MatD3D.Diffuse;
        pMeshContainer->pMaterials[ 0 ].pTextureFilename = NULL;
	}

	pMeshContainer->NumMaterials = (nMaterials>0 ? nMaterials : 1 );

	/*
		-----Load in the effects
		-Just like pMaterials pEffects contain file names which must be loaded.
	pMeshContainer->pEffects = new D3DXEFFECTINSTANCE[ pNumMaterials ];
	if(pNumMaterials>0)
		memcpy( pMeshContainer->pEffects, pEffectInstances, sizeof(D3DXEFFECTINSTANCE)*pNumMaterials );
	else
		pMeshContainer->pEffects = NULL;
	
	//Here: loop therough pMeshContainer->pEffects[i].pEffectFileName and load the effect (have no place to store it yet )

	/*
		-----Load in the skininfo 
		- new going to try to get this to work!!
	*/	
	if(pMeshContainer->pSkinInfo)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pMeshContainer->pSkinInfo->AddRef();

		pMeshContainer->pBoneMatrixOffsets = new D3DXMATRIX[ pSkinInfo->GetNumBones() ];

		/* Check to see if there the array was allocated */
		if( pMeshContainer->pBoneMatrixOffsets == NULL )
		{
			DestroyMeshContainer( pMeshContainer );
			return E_OUTOFMEMORY;
		}

		DWORD numBones = pSkinInfo->GetNumBones();

		/* Stores the offsets of the bones into a matrix array */
		for( DWORD i = 0; i < numBones; ++i )
			pMeshContainer->pBoneMatrixOffsets[ i ] = *(D3DXMATRIX*)pSkinInfo->GetBoneOffsetMatrix( i );

		// Generate a blended mesh
		//Basically, takes a mesh and outputs another mesh with vertex weights
		hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
							pMeshContainer->MeshData.pMesh,
							D3DXMESH_MANAGED|D3DXMESHOPT_VERTEXCACHE,
							pMeshContainer->pAdjacency,
							NULL, NULL, NULL,
						   &pMeshContainer->dwMaxFaceInfluences,
						   &pMeshContainer->dwNumAttributeGroups,
						   &pMeshContainer->pBoneCombinationBuffer,
						   &pMeshContainer->pMesh );
		if(FAILED(hr))
		{
			DestroyMeshContainer( pMeshContainer );
			Fatal_Error(hr, __LINE__, __FILE__);
		}

		// Make sure the mesh has the proper vertex format
		{
			DWORD dwOldFVF = pMeshContainer->pMesh->GetFVF();
			DWORD dwNewFVF = (dwOldFVF & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1;

			// If the formats don't match, recreate the mesh
			if( dwOldFVF != dwNewFVF )
			{
				// Temporary mesh container
				ID3DXMesh* pTempMesh = NULL;

				// Clone into the temporary mesh
				hr = pMeshContainer->pMesh->CloneMeshFVF(
								pMeshContainer->pMesh->GetOptions(),
								dwNewFVF, g_D3D_Struct->g_pD3DDevice, &pTempMesh );

				// Failed?  Exit
				if( FAILED( hr ) )
				{
					DestroyMeshContainer( pMeshContainer );
					return hr;
				}

				// Release the current mesh
				//SAFE_RELEASE( pMeshContainer->pMesh );
				
				pMeshContainer->pMesh->Release();
				pMeshContainer->pMesh = 0;

				// Store the new mesh
				pMeshContainer->pMesh = pTempMesh;

				// If the original mesh lacked normals, add them
				if( !(dwOldFVF & D3DFVF_NORMAL) )
				{
					hr = D3DXComputeNormals( pMeshContainer->pMesh, NULL );
					if( FAILED( hr ) ) 
					{
						DestroyMeshContainer( pMeshContainer );
						return hr;
					}
				}
			}

			// Convert the mesh's buffers from managed into default pool
			if( !(pMeshContainer->pMesh->GetOptions() & D3DXMESH_WRITEONLY) )
			{
				// Temporary info used for conversions
				LPD3DXMESH pTempMesh = NULL;
				DWORD dwOptions = 0;

				// If this is a 32-bit mesh, set that option
				if( pMeshContainer->pMesh->GetOptions() & D3DXMESH_32BIT )
					dwOptions = D3DXMESH_32BIT;

				// Clone the mesh
				hr = pMeshContainer->pMesh->CloneMeshFVF( D3DXMESH_WRITEONLY|dwOptions,
														  pMeshContainer->pMesh->GetFVF(),
														  g_D3D_Struct->g_pD3DDevice, &pTempMesh );
				if( FAILED( hr ) )
				{
					DestroyMeshContainer( pMeshContainer );
					return hr;
				}

				// Release the current mesh
				//SAFE_RELEASE( pMeshContainer->pMesh );
				pMeshContainer->pMesh->Release();
				pMeshContainer->pMesh = 0;

				// Store the new mesh
				pMeshContainer->pMesh = pTempMesh;
			}
		}

		// Calculate bone influences and do mesh type conversion
		{
			// Get the bone combination buffer
			D3DXBONECOMBINATION* pBoneComboBuffer = reinterpret_cast<D3DXBONECOMBINATION*>(pMeshContainer->pBoneCombinationBuffer->GetBufferPointer());

			// Generate hardware-friendly bone combinations
			for( pMeshContainer->dwStartSoftwareRenderAttribute = 0;
				 pMeshContainer->dwStartSoftwareRenderAttribute < pMeshContainer->dwNumAttributeGroups;
			   ++pMeshContainer->dwStartSoftwareRenderAttribute )
			{
				   // Total number of influences on this subset
				DWORD dwTotalInfluences = 0;

				// Look through all bones
				for( DWORD i = 0; i < pMeshContainer->dwMaxFaceInfluences; ++i )
				{
					// Search this ID
					if( pBoneComboBuffer[pMeshContainer->dwStartSoftwareRenderAttribute].BoneId[i] != UINT_MAX )
					{
						// Increase influences
						++dwTotalInfluences;
					}
				}

				// If there are too many influences, split the mesh
				if( dwTotalInfluences > m_dwMaxBlendedMatrices )
					break;
			}

			// If the mesh was split, add the software processing flag
			if( pMeshContainer->dwStartSoftwareRenderAttribute < pMeshContainer->dwNumAttributeGroups )
			{
				// Construct a temporary mesh
				ID3DXMesh* pTempMesh = NULL;

				// Clone to the software mesh
				hr = pMeshContainer->pMesh->CloneMeshFVF(
						D3DXMESH_SOFTWAREPROCESSING|pMeshContainer->pMesh->GetOptions(),
						pMeshContainer->pMesh->GetFVF(), g_D3D_Struct->g_pD3DDevice, &pTempMesh );

				// Failed? Return error
				if( FAILED( hr ) )
				{
					DestroyMeshContainer( pMeshContainer );
					return hr;
				}

				// Release the current mesh
				//SAFE_RELEASE( pMeshContainer->pMesh );
				pMeshContainer->pMesh->Release();
				pMeshContainer->pMesh = 0;

				// Copy in the new mesh
				pMeshContainer->pMesh = pTempMesh;
			}
		}

	}//if(mesh has skininfo

	 * ppNewMeshContainer = pMeshContainer;
	return S_OK;
}

/*-------------------------------------------------------------------------

	Destroys a frame created with CreateFrame().

-------------------------------------------------------------------------*/
HRESULT D3D_AllocHierarchy::DestroyFrame( THIS_  LPD3DXFRAME pFrameToFree )
{
	D3D_Frame* pframe = (D3D_Frame*)pFrameToFree;
    if( pframe->Name ) delete [] pframe->Name;
    delete pframe;

    return S_OK;
}

/*---------------------------------------------------------------------------
	
	Destroys a mesh container created with CreateMeshContainer().

---------------------------------------------------------------------------*/
HRESULT D3D_AllocHierarchy::DestroyMeshContainer( THIS_  LPD3DXMESHCONTAINER pMeshContainerToFree )
{
	DWORD i=0;

	D3D_MeshContainer* pMC= (D3D_MeshContainer*)pMeshContainerToFree;

	if( pMC->Name ) delete [] pMC->Name;
	if( pMC->MeshData.pMesh) pMC->MeshData.pMesh->Release();
	if( pMC->pAdjacency ) delete [] pMC->pAdjacency;
	if( pMC->pEffects ) delete [] pMC->pEffects;
	if( pMC->pMaterials ) delete [] pMC->pMaterials;
	if( pMC->pSkinInfo ) pMC->pSkinInfo->Release();
	if( pMC->pMesh ) pMC->pMesh->Release();
	if( pMC->ppBoneMatrixPointers ) delete [] pMC->ppBoneMatrixPointers;
	if( pMC->pBoneCombinationBuffer ) pMC->pBoneCombinationBuffer->Release();
	if( pMC->pBoneMatrixOffsets ) delete [] pMC->pBoneMatrixOffsets;

	return S_OK;
}

/*---------------------------------------------------------------------------
	
	Sets a pointer to the texture controller.

---------------------------------------------------------------------------*/
void D3D_AllocHierarchy::Set_TexControllerPointer( Texture_Controller_Class* ptc )
{ 
	p_TexController = ptc; 
}

/*---------------------------------------------------------------------------
	
	Sets a pointer to the encapsulating D3D_Mesh class.

---------------------------------------------------------------------------*/
void D3D_AllocHierarchy::Set_Wrapper( D3D_Mesh* m )
{ 
	pWrapperMesh = m; 
}
