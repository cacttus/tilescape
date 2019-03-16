#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "Mesh_Controller.h"
#include "Texture_Controller.h"

/*
	Mesh Controller Class
	Functions
----------------------------*/
void Mesh_Controller_Class::Init(Texture_Controller_Class* tc, Frustum_Class* pf)
{
	if(Initialized) Fatal_Error("Mesh Controller already initialized!");
	p_TextureController = tc;
	pFrustum = pf;

	Initialized=true;
}
/*-------------------------------------------------------------

	Add a mesh and give it the identifier "id".
	id: used to reference the mesh from the controller

--------------------------------------------------------------*/
void Mesh_Controller_Class::Add_Mesh( char* file, DWORD id )
{
	if(!Initialized) Fatal_Error("Mesh Controller has not been initialized!");

	D3D_Mesh* pMeshPtr = pMeshes; //To run through the linked list

	//Might wanna check for pre-loaded meshes..later

	//Instantiate the class, or append to the list.
	if(pMeshes==0)
	{
		pMeshes = new D3D_Mesh();
		pMeshPtr = pMeshes;
	}
	else
	{
		D3D_Mesh* pLast;
		while(pMeshPtr!=0)
		{
			pLast=pMeshPtr;
			pMeshPtr=pMeshPtr->pNext;
		}
		pMeshPtr = new D3D_Mesh();
		if(pLast!=0) pLast->pNext = pMeshPtr;
	}

	pMeshPtr->Init( p_TextureController );

	//Load the big one.
	HRESULT hr = D3DXLoadMeshHierarchyFromX( file, 0, g_D3D_Struct->g_pD3DDevice, pMeshes->Get_pAllocHierarchy(),
		NULL, (LPD3DXFRAME*)&pMeshPtr->pFrameRoot, &pMeshPtr->pAnimController );

	if(FAILED(hr))
	{
		Fatal_Error(hr, __LINE__, __FILE__ );
		if(pMeshPtr==pMeshes) pMeshes=0;
		pMeshPtr->pAnimController->Release();
		delete pMeshPtr;
		return;
	}

	//Set initial data
	pMeshPtr->pNext = 0;
	pMeshPtr->Ref_Count++;
	pMeshPtr->Set_ID( id );
	pMeshPtr->Calc_BoundBox( pMeshPtr->pFrameRoot );

	Num_Meshes++;

	/* --OLD array method

	pMeshes[Num_Meshes] = new D3D_Mesh();
	pMeshes[Num_Meshes]->Init( p_TextureController );
	pMeshes[Num_Meshes]->Set_ID( id );


	//--- Load the mesh with animation ---
	HRESULT res;
	res = D3DXLoadMeshHierarchyFromX( file,
								0,//options
								g_D3D_Struct->g_pD3DDevice,
								pMeshes[Num_Meshes]->Get_pAnimation(),//in
								NULL, //pMeshes[Num_Meshes]->pAnimation->pUserDataLoader,//in or null
								(LPD3DXFRAME*)&pMeshes[Num_Meshes]->pFrameRoot, //out 
								&pMeshes[Num_Meshes]->pAnimController ); //out

	if(FAILED(res))
	{ 
		NonFatal_Error(res, __LINE__, __FILE__);
		return;
	}

	// X Mesh loading succeeded !!! 

	//Incrament themesh count
	Num_Meshes++;
	*/
}
void Mesh_Controller_Class::Render( DWORD id, D3DXVECTOR3* Pos )
{
	D3D_Mesh* pMeshPtr = pMeshes;

	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );//directx culls counter-clockwise

	D3DXMATRIXA16 Trans, World;
	D3DXMatrixIdentity( &World );
	g_D3D_Struct->g_pD3DDevice->SetTransform( D3DTS_WORLD, &World );
	D3DXMatrixTranslation( &Trans, Pos->x, Pos->y, Pos->z );
	D3DXMatrixMultiply( &World, &Trans, &World );
	g_D3D_Struct->g_pD3DDevice->SetTransform( D3DTS_WORLD, &World );

	while( pMeshPtr!=0 )
	{
		if(pMeshPtr->ID == id )
		{
			pMeshPtr->CheckVisible( pFrustum, Pos );

			pMeshPtr->pBoundCube->_DrawThis();

			if(pMeshPtr->bCanDraw)
				pMeshPtr->Render( pMeshPtr->pFrameRoot );
			return;
		}
		pMeshPtr = pMeshPtr->pNext;
	}

}
/*-----------------------------------------------------
			Render the mesh transformed
			with the specified matrix.
-----------------------------------------------------*/
void Mesh_Controller_Class::Render( DWORD id, D3DXMATRIX* Mat )
{
	D3D_Mesh* pMeshPtr = pMeshes;

	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );//directx culls counter-clockwise
	
	//Reset the World matrix.
	D3DXMATRIXA16 World;
	D3DXMatrixIdentity( &World );
	g_D3D_Struct->g_pD3DDevice->SetTransform( D3DTS_WORLD, &World );
	D3DXMatrixMultiply( Mat, &World, Mat);//-----------------------------------------HACK - might be wrong order of operations.
	g_D3D_Struct->g_pD3DDevice->SetTransform( D3DTS_WORLD, Mat );

	while( pMeshPtr!=0 )
	{
		if(pMeshPtr->ID == id )
		{

			//Checks if the mesh is visible, also updates the bounding volume to the mesh position
			pMeshPtr->CheckVisible( pFrustum, Mat );

			pMeshPtr->pBoundCube->_DrawThis();

			if(pMeshPtr->bCanDraw)
				pMeshPtr->Render( pMeshPtr->pFrameRoot );
			return;
		}
		pMeshPtr = pMeshPtr->pNext;
	}

}
void Mesh_Controller_Class::Free()
{
	D3D_Mesh* pMeshPtr=pMeshes;
	D3D_Mesh* pTemp=0;;
	
	while(pMeshPtr!=0)
	{
		pTemp=pMeshPtr;
		pMeshPtr=pMeshPtr->pNext;
		pTemp->Free();
		delete pTemp;
	}
	/* -- OLD array mode
	for( int i=0; i<Max_Meshes; ++i )
	{
		if(pMeshes[i]) pMeshes[i]->Free();
	}
	*/
}	

void Mesh_Controller_Class::Remove_Ref( DWORD id )
{
	D3D_Mesh* pMeshPtr = pMeshes;
	D3D_Mesh* pPrev;	//To patch up the list

	while(pMeshPtr!=0)
	{
		if(pMeshPtr->ID==id)
		{
			pMeshPtr->Ref_Count--;
			if(pMeshPtr->Ref_Count<=0)
			{
				pPrev->pNext = pMeshPtr->pNext;	//Patch up the list
				//if(pMeshPtr->FileLocation) delete [] pMeshPtr->FileLocation; //mabe later
				pMeshPtr->Free();	//Free the resource.
				delete pMeshPtr;
				
				pMeshPtr=0;
			}
			return;
		}
		pPrev = pMeshPtr;
		pMeshPtr = pMeshPtr->pNext;
	}

}