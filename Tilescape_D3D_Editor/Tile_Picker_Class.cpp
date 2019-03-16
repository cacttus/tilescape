#include "Tile_Picker_Class.h"


Tile_Picker_Class::Tile_Picker_Class(){;}

void Tile_Picker_Class::Init( Tile_Controller_Class* ptc ){

	memset(this, 0, sizeof(Tile_Picker_Class));

	int Num = 8; //max 8 vertices
	int Size = Num * sizeof(Vertex);

	// - Might not need this variable.
	pTileController = ptc;
	// - Careful here, you may have not instantiated tilecontroller yet!

	HRESULT hr;
	hr = g_D3D_Struct->g_pD3DDevice->CreateVertexBuffer( Size, //size of buffer
		0, 
		FVF_DESCRIPTOR, 
		D3DPOOL_DEFAULT, 
		&D3DLineBuffer, //the vertex buffer (d3d)
		NULL );
	if(FAILED(hr)) Fatal_Error("COULD NOT CREATE TILE BUFFER");


	LineBuffer = new Vertex[Num];
	pHitStruct = new Hit_Struct();

	b_VertexMode = true;

	VOID* lpVoid;
	D3DLineBuffer->Lock( 0, 0, &lpVoid, 0 );		
	memcpy( lpVoid, LineBuffer, Size );
	D3DLineBuffer->Unlock();	

}
/*------------------------------------------------
		Update the tile picker.
		Based on a top left vertex
		and the number of tiles 
		selected to the right of it
------------------------------------------------*/
void Tile_Picker_Class::Update()
{
	if(!pHitStruct->pMesh) return;

	HRESULT hr;
	Vertex* pVerts;

	if( b_VertexMode )
	{
		hr = D3DLineBuffer->Lock( 0, 0, (void**)&pVerts, D3DLOCK_DISCARD ); ErrorCheck(hr, __LINE__, __FILE__ );
		
		if( pHitStruct->Tri==0 ) //Triangle 0 was hit
		{

			if(pHitStruct->UV>0.5f)
			{ 
				*(pVerts+0) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3)); 
				*(pVerts+1) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0));

				*(pVerts+2) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0));
				*(pVerts+3) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1));
				pHitStruct->TriVert = 0;
				LastVertSelected = 0;
				PrimCount=2;
			}
			else if(pHitStruct->U>0.5f)
			{ 
				*(pVerts+0) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0)); 
				*(pVerts+1) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1));

				*(pVerts+2) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1));
				*(pVerts+3) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2));
								pHitStruct->TriVert = 1;
				LastVertSelected = 3;
				PrimCount=2;
			}
			else if(pHitStruct->V>0.5f)
			{ 
				*(pVerts+0) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1)); 
				*(pVerts+1) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2));

				*(pVerts+2) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2));
				*(pVerts+3) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3));	
								pHitStruct->TriVert = 2;
				LastVertSelected = 2;
				PrimCount=2;
			}
			else 
			{ 
				*(pVerts+0) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0)); 
				*(pVerts+1) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1));

				*(pVerts+2) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1));
				*(pVerts+3) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2));

				*(pVerts+4) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2)); 
				*(pVerts+5) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3));

				*(pVerts+6) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3));
				*(pVerts+7) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0));
								pHitStruct->TriVert = 4;

				LastVertSelected = 4;

				PrimCount=4;
			}

		}
		else //Triangle 1 was hit
		{
			if(pHitStruct->UV>0.5f)//user can move vertex 2
			{ 
				*(pVerts+0) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3)); 
				*(pVerts+1) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0));

				*(pVerts+2) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0));
				*(pVerts+3) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1));	
								pHitStruct->TriVert = 0;
				LastVertSelected = 0;
				PrimCount=2;
			}
			else if(pHitStruct->U>0.5f)//user can move vertex 3
			{ 
				*(pVerts+0) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1)); 
				*(pVerts+1) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2));

				*(pVerts+2) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2));
				*(pVerts+3) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3));	
								pHitStruct->TriVert = 2;
				LastVertSelected = 1;
				PrimCount=2;
			}
			else if(pHitStruct->V>0.5f)//user can move vertex 1
			{ 
				*(pVerts+0) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2)); 
				*(pVerts+1) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3));

				*(pVerts+2) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3));
				*(pVerts+3) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0));
								pHitStruct->TriVert = 3;
				LastVertSelected = 2;
				PrimCount=2;
			}
			else 
			{ 
				*(pVerts+0) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0)); 
				*(pVerts+1) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1));

				*(pVerts+2) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+1));
				*(pVerts+3) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2));

				*(pVerts+4) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+2)); 
				*(pVerts+5) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3));

				*(pVerts+6) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+3));
				*(pVerts+7) = *(pHitStruct->pMesh->pVertices + (pHitStruct->Offset+0));
					pHitStruct->TriVert = 4;
				LastVertSelected = 4;
				PrimCount=4;
			}
		}

		hr = D3DLineBuffer->Unlock(); ErrorCheck(hr, __LINE__, __FILE__ );
		

	}
	else if( b_MultiTileMode ) // Select a range of tiles
	{

	}
	

}

void Tile_Picker_Class::Render()
{
 
	g_D3D_Struct->g_pD3DDevice->SetTexture( 0, 0 );
	g_D3D_Struct->g_pD3DDevice->SetStreamSource( 0, D3DLineBuffer, 0, sizeof(Vertex) ); 
	g_D3D_Struct->g_pD3DDevice->DrawPrimitive( D3DPT_LINELIST, 0, PrimCount);
	
	
}
void Tile_Picker_Class::Free()
{
	if(pHitStruct)
	{
		delete pHitStruct;
		pHitStruct=0;
	}

	if(D3DLineBuffer)
	{
		D3DLineBuffer->Release();
		D3DLineBuffer=0;
	}

	delete LineBuffer;
}

