#include "D3D_Mesh.h"

/*
	D3D_Mesh functions 
---------------------------------*/

//Initialize the structure
void D3D_Mesh::Init(Texture_Controller_Class* tc)
{

	pAllocHierarchy = new D3D_AllocHierarchy();
	pAllocHierarchy->Set_TexControllerPointer( tc );
	pAllocHierarchy->Set_Wrapper(this);//pWrapperMesh = &this;
	this->pAnimController = NULL;
	this->pFrameRoot = NULL;
	pBoundCube = new Cube_Class();

	//Tex_IDs=NULL; - old
}

//Free the resources
void D3D_Mesh::Free()
{
	//There is an error around this code!
	//if(pAllocHierarchy) delete pAllocHierarchy;
	//if(Tex_IDs) delete  Tex_IDs;
}

// Returns a pointer to the animation class ( for use with D3DXLoadMeshHierarchyFromX() )
D3D_AllocHierarchy* D3D_Mesh::Get_pAllocHierarchy()
{ 
	return pAllocHierarchy; 
}	

//Sets the unique identifyer for this structure
void D3D_Mesh::Set_ID( DWORD id )
{ 
	ID = id; 
}

//Gets the unique identifyer for this structure
DWORD D3D_Mesh::Get_ID()
{ 
	return ID; 
}

//Constructor, zeroes the structure
D3D_Mesh::D3D_Mesh()
{
	memset(this,0,sizeof(D3D_Mesh));
}

/*------------------------------------------------------------
	Updates the transformation matrix for this frame,
	all it's children, and all it's siblings.
	This should be called on the root frame of a mesh to
	update the animation sequence.
	Params:
	- pParentMatrix: matrix of the parent frame 
		( or the root frame's matrix ).
------------------------------------------------------------*/
void D3D_Mesh::UpdateTransform( D3DXFRAME* pFrame, D3DXMATRIX* pParentMatrix )
{
	D3D_Frame* pTemp = (D3D_Frame*)pFrame;

	/* Multiplies the matrices into the global combination matrix */
	if( pParentMatrix != NULL ) D3DXMatrixMultiply( &pTemp->CombinationMatrix, 
													&pTemp->TransformationMatrix, 
													pParentMatrix );

	else pTemp->CombinationMatrix = pTemp->TransformationMatrix;

	/* Call the function on all the child frame */
	if( pTemp->pFrameFirstChild ) UpdateTransform( pTemp->pFrameFirstChild, &pTemp->CombinationMatrix );

	/* Call the function on all the sibling frame */
	if( pTemp->pFrameSibling ) UpdateTransform( pTemp->pFrameSibling, pParentMatrix );

}
/*---------------------------------------------------------------
	Render: Renders the mesh
---------------------------------------------------------------*/
void D3D_Mesh::Render( D3D_Frame * pRoot_Frame )
{

	/* Call the UpdateTransform function, to update all the frame matrices */
	D3DXMATRIX matWorld;
	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_WORLD, &matWorld );

	//Disabling this disables the frame hierarchy concatenation
	if(pAnimController)
		Update_Animation( &matWorld );

	//D3DXMatrixIdentity( &matWorld );
	//g_D3D_Struct->g_pD3DDevice->SetTransform( D3DTS_WORLD,  &matWorld );

	DrawFrames( pRoot_Frame );

	//if( pRoot_Frame->pMeshContainer )
		//DrawFrameMesh( pRoot_Frame->pFrameFirstChild->pFrameSibling);
		//DrawFrameMesh( pRoot_Frame->pFrameFirstChild);
	//Cur_Time++;
}

/*---------------------------------------------------------------
	DrawFrames: Recursively draws each frame of the mesh.
---------------------------------------------------------------*/
void D3D_Mesh::DrawFrames( LPD3DXFRAME pFrame )
{
	D3D_Frame* pTemp = (D3D_Frame*)pFrame;

	if( pTemp->pMeshContainer )
		DrawFrameMesh( pTemp );

	if( pTemp->pFrameFirstChild )
		DrawFrames( (D3D_Frame*)pTemp->pFrameFirstChild );

	if( pTemp->pFrameSibling )
		DrawFrames( (D3D_Frame*)pTemp->pFrameSibling );

}
/*---------------------------------------------------------------
	DrawFrameMesh: Draws the mesh contained by a single frame.
---------------------------------------------------------------*/
HRESULT D3D_Mesh::DrawFrameMesh( LPD3DXFRAME pFrame )
{
	HRESULT hr;
	D3D_MeshContainer* pMeshContainer = (D3D_MeshContainer*)pFrame->pMeshContainer;
	D3D_Frame* pTempFrame = (D3D_Frame*)pFrame;
	//D3DXMatrixIdentity( &pTempFrame->CombinationMatrix );

	/* If there is no skin info, then just render the mesh */
	if( pMeshContainer->pSkinInfo == NULL )
	{

		/* Reset the world matrix? mabe this will solve the problem! */
		/* Without this line of code, there is no animation! */
		g_D3D_Struct->g_pD3DDevice->SetTransform( D3DTS_WORLD, &pTempFrame->CombinationMatrix );

		for( unsigned int i=0; i<pTempFrame->pMeshContainer->NumMaterials; ++i )
		{
 			//g_D3D_Struct->g_pD3DDevice->SetMaterial( &pTempFrame->pMeshContainer->pMaterials[i].MatD3D );
			if(!pMeshContainer->No_Texture)
				pAllocHierarchy->p_TexController->Set_Texture( pMeshContainer->Textures[i] );
			else
			{
				//Set the first texture stage to be a color operation instead
				//g_D3D_Struct->g_pD3DDevice->GetTextureStageState( 0, 

				//Disables texture mapping
				//g_D3D_Struct->g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );//Disables texture mapping

				//g_D3D_Struct->g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
				//g_D3D_Struct->g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
				pAllocHierarchy->p_TexController->Set_Texture( NULL );
				hr = g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
				if(FAILED(hr))
					NonFatal_Error(hr,__LINE__,__FILE__);
				hr = g_D3D_Struct->g_pD3DDevice->SetMaterial( &pTempFrame->pMeshContainer->pMaterials[i].MatD3D );
				if(FAILED(hr))
					NonFatal_Error(hr,__LINE__,__FILE__);
			}
			g_D3D_Struct->g_pD3DDevice->SetMaterial( &pTempFrame->pMeshContainer->pMaterials[i].MatD3D );
			//If doing skinning make sure to set this to the skinned mesh, not the regular mesh
			pTempFrame->pMeshContainer->MeshData.pMesh->DrawSubset( i );
		}
	}
	/* else render the mesh's skin and etc */
	else
	{

	}//if(skininfo=null)else

	//Success
	return S_OK;
}
/*----------------------------------------------------------------------
						Animation -- works
----------------------------------------------------------------------*/
void D3D_Mesh::SetTrack( int Index )
{
	Cur_Animation = Index;

	/* -- How D3DX does animations--
		Method:
		- Get the animation set, which is stored as an index 
		  value inside the animation controller.					(GetAnimationSet(...))
		- Set the animation to the newly obtained animation set.	(SetTrackAnimationSet(...))
		- Call AdvanceTime() to update the animation				(AdvanceTime(...))

		Notes:
		- Tracks are used to interpolate between animations
	*/

	LPD3DXANIMATIONSET AnimationSet;

	HRESULT hr;

	/* Gets the animation set specified by 'Index' */
	hr = pAnimController->GetAnimationSet( Index, &AnimationSet );	
			if(FAILED(hr))NonFatal_Error(hr, __LINE__, __FILE__);

	/* This function applies the animation obtained from
		'GetAnimationSet()' to the specified animation track. */
	hr = pAnimController->SetTrackAnimationSet( 0, AnimationSet );
			if(FAILED(hr))NonFatal_Error(hr, __LINE__, __FILE__);

	/* Reset the track position. Resets the animation to the beginning */
	hr = pAnimController->SetTrackPosition( 0, 0.00 );
			if(FAILED(hr))NonFatal_Error(hr, __LINE__, __FILE__);

	/* Release the animation object */
    AnimationSet->Release();	
/*
	// Clear any track events currently assigned to our two tracks
	hr = pAnimController->UnkeyAllTrackEvents( Cur_Track ); 		
	if(FAILED(hr))NonFatal_Error(hr, __LINE__, __FILE__);
    hr = pAnimController->UnkeyAllTrackEvents( NewTrack ); 		
	if(FAILED(hr))NonFatal_Error(hr, __LINE__, __FILE__);

	// Add an event key to disable the currently playing track kMoveTransitionTime seconds in the future
    pAnimController->KeyTrackEnable( Cur_Track, FALSE, Cur_Time + Transition_Time );
	// Add an event key to change the speed right away so the animation completes in kMoveTransitionTime seconds
    pAnimController->KeyTrackSpeed( Cur_Track, 0.0f, Cur_Time, Transition_Time, D3DXTRANSITION_LINEAR );
	// Add an event to change the weighting of the current track (the effect it has blended with the secon track)
    pAnimController->KeyTrackWeight( Cur_Track, 0.0f, Cur_Time, Transition_Time, D3DXTRANSITION_LINEAR );

	// Enable the new track
    hr = pAnimController->SetTrackEnable( NewTrack, TRUE );
		if(FAILED(hr))
			NonFatal_Error(hr, __LINE__, __FILE__);

	// Add an event key to set the speed of the track
    pAnimController->KeyTrackSpeed( NewTrack, 1.0f, Cur_Time, Transition_Time, D3DXTRANSITION_LINEAR );
	// Add an event to change the weighting of the current track (the effect it has blended with the first track)
	// As you can see this will go from 0 effect to total effect(1.0f) in kMoveTransitionTime seconds and the first track goes from 
	// total to 0.0f in the same time.
    pAnimController->KeyTrackWeight( NewTrack, 1.0f, Cur_Time, Transition_Time, D3DXTRANSITION_LINEAR );
*/
	// Remember current track
//    Cur_Track = NewTrack;

}

/*-------------------------------------------------
		Update the matrices in the frame
		hierarchy.
-------------------------------------------------*/
int ccDOWN=0;
void D3D_Mesh::Update_Animation( D3DXMATRIX* matWorld )
{

	if( (Keydown( VK_UP )))
	{ 	
		if(ccDOWN==0)
		{
			if((unsigned)Cur_Track<(pAnimController->GetMaxNumAnimationSets()-1))
			Cur_Track+=1;
			else Cur_Track=0;
			SetTrack(Cur_Track);
			ccDOWN=1;
		}
	}else ccDOWN=0;

	if(pAnimController->GetTime()>=1.0f) pAnimController->ResetTime();

	// Advance the time and set in the controller
    if (pAnimController != NULL)
         pAnimController->AdvanceTime(0.02f, 0);

	// Now update the model matrices in the hierarchy
	UpdateTransform(pFrameRoot, matWorld);

	// If the model contains a skinned mesh update the vertices
	//D3DXMESHCONTAINER_EXTENDED* pMesh = m_firstMesh;
	//if(pMesh && pMesh->pSkinInfo)
	{
		//Which it doesn't!
	}

}
/*-------------------------------------------
	Calculate the bounding box for the mesh.
	Slow operation, recursive.
	Call sparingly.
	--Need to apply the transformation matrix 
	on the mesh before passing the vertices
-------------------------------------------*/
void D3D_Mesh::Calc_BoundBox( LPD3DXFRAME pFrame )
{
	D3D_Frame* pTemp = (D3D_Frame*)pFrame;

	D3DXMATRIX matWorld;
	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_WORLD, &matWorld );

	//Disabling this disables the frame hierarchy concatenation
	if(pAnimController)
		Update_Animation( &matWorld );

	if(pTemp->pMeshContainer )
		pBoundCube->_ReCalc( pTemp->pMeshContainer->MeshData.pMesh, &pTemp->CombinationMatrix );

	if( pTemp->pFrameSibling )
		Calc_BoundBox( pTemp->pFrameSibling );

	if( pTemp->pFrameFirstChild )
		Calc_BoundBox( pTemp->pFrameFirstChild );

}
/*-------------------------------------------
	Check if the mesh is completely visib
	le and set the bCanDraw flag
	accordingly. This is representative
	of the Vis_Quick() procedure in the
	"Mesh_Class" class.
-------------------------------------------*/
void D3D_Mesh::CheckVisible(Frustum_Class* pFrustum, D3DXVECTOR3* Pos)
{
	pBoundCube->_Update( Pos );

	if( pBoundCube->_Visible( pFrustum ) )
		bCanDraw=true;
	else
		bCanDraw=false;
}

void D3D_Mesh::CheckVisible(Frustum_Class* pFrustum, D3DXMATRIX* Mat)
{
	pBoundCube->_Update( Mat );

	if( pBoundCube->_Visible( pFrustum ) )
		bCanDraw=true;
	else
		bCanDraw=false;
}




