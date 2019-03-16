#include "Mesh_Class.h"
#include "Texture_Controller.h"
void Mesh_Class::Init( DWORD nVerts, DWORD nFaces )
{
	HRESULT hr;

	Num_Faces =		nFaces;
	Num_Vertices =	nVerts;
	nAttributes =	nFaces;

	pAttributes = new DWORD[Num_Faces];
	memset( pAttributes, 0, sizeof(DWORD) * Num_Faces );

	pVertices = new Vertex[Num_Vertices];
	memset( pVertices, 0, sizeof(Vertex) * Num_Vertices );

	pCanDrawPoly = new BOOL[Num_Faces];
	memset( pCanDrawPoly, 1, sizeof(bool) * Num_Faces );


	hr = g_D3D_Struct->g_pD3DDevice->CreateVertexBuffer ( 
		sizeof(Vertex)*Num_Vertices,
		D3DUSAGE_DYNAMIC, 
		FVF_DESCRIPTOR,
		D3DPOOL_DEFAULT, 
		&pVertexBuffer,
		NULL );
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );
	
	//The buffer used to store index data
	hr = g_D3D_Struct->g_pD3DDevice->CreateIndexBuffer ( 
		(sizeof(WORD) * 3) * Num_Faces,
		0, //D3DUSAGE_DYNAMIC
		D3DFMT_INDEX16, 
		D3DPOOL_DEFAULT, 
		&pIndexBuffer, 
		NULL );
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );

	//The buffer filled each time visibility changes
	hr = g_D3D_Struct->g_pD3DDevice->CreateIndexBuffer ( 
		(sizeof(WORD) * 3) * Num_Faces,
		D3DUSAGE_DYNAMIC, 
		D3DFMT_INDEX16, 
		D3DPOOL_DEFAULT, 
		&pRenderBuffer,
		NULL );
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );

}
/*
	Creates an attribute table key based
	on the currently allocated attributes.
	e.g. for the tile game the key may look like
	+0x00000000: [0,256]		; tiles
	+0x00000004: [25,1024]		; bases
*/
void Mesh_Class::MakeAttribKey( )
{
	if( !pAttributes ) return;

	DWORD i;
	nAttribSets=GetNumAttribs();	//Number of attribute types

	pAttribKey = new DWORD[nAttribSets*2];	//2 dwords for each set, 1for the index, and one for the number of attribs of that index.

	//Obviously similar code to GetNumAttribs(),
	//this just loops through all the attributes
	//and finds new attribs that it has not seen.
	DWORD CurKeyInd = 0;	//current index of the key
	DWORD dwNextInd = 0;	// the next index to search for
	DWORD dwCurInd = 0;		// current index(starts at 0)
	bool DoOver = TRUE;		// if not done finding all the attribs
	bool Found = FALSE;		// if found a new index

	while(DoOver)
	{
		Found = FALSE;
		DoOver=FALSE;
		dwCurInd = dwNextInd;
		dwNextInd=0;
		for( i=0; i<nAttributes; ++i )
		{
			if( dwNextInd != 0 )
			{
				if( (pAttributes[i]>dwCurInd) && (pAttributes[i]<dwNextInd) )
				{
					dwNextInd=pAttributes[i];
					DoOver=true;
				}
			}
			else//this is the first iteration
			{
				if(pAttributes[i]>dwCurInd)
				{
					dwNextInd=pAttributes[i];
					DoOver=true;
				}
			}
			if( (pAttributes[i]==dwCurInd) && (!Found) )
			{ 
				pAttribKey[CurKeyInd] = pAttributes[i];
				pAttribKey[CurKeyInd+1] = GetNumAttribsByType( pAttributes[i] );
				if(CurKeyInd>=(nAttribSets*2)) Fatal_Error( "Buffer Overrun in MakeKey()" );
				CurKeyInd+=2; //incrament two places
				Found=TRUE; 
			}
		}
	}
}
/*------------------------------------------
	Just returns the number of attributes 
	in the attribute buffer 'pAttributes'.
------------------------------------------*/
DWORD Mesh_Class::GetNumAttribs()
{
	DWORD dwNextInd = 0;	// - the next index to search for
	DWORD dwNumAttribs=0;	// - The number of indices
	DWORD dwCurInd = 0;		// - current index(starts at 0)

	bool DoOver = TRUE;		// - if not done finding all the attribs
	bool Found = FALSE;		// - if found a new index

	while(DoOver)
	{
		Found = FALSE;
		DoOver=FALSE;
		dwCurInd = dwNextInd;
		dwNextInd=0;
		for( DWORD i=0; i<nAttributes; ++i )
		{
			if( dwNextInd != 0 )//this is not the first iteration
			{
				if( (pAttributes[i]>dwCurInd) && (pAttributes[i]<dwNextInd) )
				{
					//Found a new attribute ( a number higher than the current one looking for and less than the one gonna look for)
					dwNextInd=pAttributes[i];
					DoOver=true;
				}
			}
			else//this is the first iteration, NextInd has not been set yet, so the above won't work
			{
				if(pAttributes[i]>dwCurInd)
				{
					dwNextInd=pAttributes[i];
					DoOver=true;
				}
			}

			if( (pAttributes[i]==dwCurInd) && (!Found) )
			{ 
				dwNumAttribs++; 
				Found=TRUE; //Found this attribute, but must keep searching through for smaller next attributes.
			}

		}
	}
	return dwNumAttribs;
}
DWORD Mesh_Class::GetNumAttribsByType( DWORD attrib )
{
	if(!pAttributes) return 0;
	
	DWORD i;
	int Num=0;
	
	for( i = 0; i < nAttributes; ++i )
	{
		if(pAttributes[i]==attrib) Num++;	
	}
	
	return Num;

}
/*---------------------------------------------
	Shifts the buffer elemenst, instead
	of swapping them, which the old ver-
	sion did.  This keeps the buffer
	array ordered, and contiguous.
	buf is the attribute buffer
	lenleft is the length left in the buffer
	attrib1 is the start attribute
	attrib2 is the attribute to move
----------------------------------------------*/
void Mesh_Class::ShiftBuf( DWORD* &a1, DWORD* &a2, Tri_16* t1, Tri_16* t2)
{
	DWORD *olda1 = a1, *olda2 = a2;
	Tri_16 *oldt1=t1, *oldt2=t2;

	DWORD aT=0, aT2=0;	// The attribute being held temporarily
	Tri_16 tT, tT2;
	
	//DWORD* Hole=0;	// position in the buffer, The hole that was created when the switching attribute, took over the other attribute's position
	
	aT = *a1;	 //the temp attrib
	*a1 = *a2; //switch the two

	tT = *t1;
	*t1 =*t2;
	
	if(a2<a1) Fatal_Error( " WTF, a2<a1!!! " );

	DWORD len = (DWORD)(a2 - a1);// / sizeof(DWORD);
/*
	DWORD len2=0;
	DWORD*tmp = a1;
	while(tmp!=a2)
	{
		tmp++; len2++;
	}
*/
	a1++; //go past the switched attribute
	t1++;
	//shift the buffer
	for( DWORD i=0; i<len; ++i )
	{
		aT2 = *a1;
		*a1 = aT;
		aT = aT2;
		a1++;

		tT2 = *t1;
		*t1 = tT;
		tT = tT2;
		t1++;
	}
	a1 = olda1; a2 = olda2;
	t1 = oldt1; t2 = oldt2;

}

/*---------------------------------
	Sorts all the attributes
	in the attribute table
	based on a key value.
	The new version keeps the
	order of the attributes
	contiguous.
----------------------------------*/
void Mesh_Class::SortAttributes()
{
	if( !nAttribSets ) return;
	if( !pAttributes ) return;

	//New - don't create the seperate attribute table, it is just redundant.

	/* Loop through all the attributes flipping them
		and their correspinding indicies */
	DWORD	i=0 ,j=0, k=0;

	DWORD*	pAttribPtr;				//points to the end of each set while performing a linear search through the attributes.
	DWORD*	pSrchPtr;
	DWORD	pDTEMP;

	Tri_16* pTriPtr;				//Triangles are switched in correlation to the indices pAttribPtr := pTriPtr
	Tri_16* pT2;
	Tri_16  pTTEMP;

	void*	pVoid;

	HRESULT hr = pIndexBuffer->Lock( 0, 0, &pVoid, D3DLOCK_DISCARD ); if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );
	{

		//Next loop through all the attribute types and rearrange the index buffer
		for( i=0; i<1; i++ )
		{
			//Set the attrib pointer to point to the attributes
			pAttribPtr = pAttributes;
			pTriPtr = (Tri_16*)pVoid;

			//find the starting point
			for( j=0; j<nAttributes; ++j )
			{ 
				if(*pAttribPtr == pAttribKey[i*2])
				{
					j=nAttributes; 
				}
				else
				{ 
					pAttribPtr++; 
					pTriPtr++; 
				} 
			}

			UINT COUNTER1 = 0;
			UINT COUNTER2 = 0;

			//Loop through all attributes in the attribute table and rearrange inconsistencies.
			for( j=0; j<nAttributes-1; ++j )
			{

				if( *pAttribPtr != pAttribKey[i*2] )	//If the array is not contiguous, then switch the attributes.
				{
					

					COUNTER2=0;
					pSrchPtr =	pAttribPtr;				//Start from where pAttribPtr left off.
					pT2 =		pTriPtr;

					for( k=j; k< nAttributes-1; ++k )	//Run through the rest of the buffer.
					{
						//if( (pSrchPtr - pAttributes) > nAttributes) //out of bounds!!!!!
						COUNTER2++;
						
						//As you can see I had a real error here, fixed though -- check documentation.
						if( (COUNTER2+COUNTER1) >= nAttributes) 
							Fatal_Error( "BUFFER OVERRUN!!!" );
						
						if( (unsigned)(pSrchPtr - pAttributes) > (nAttributes*sizeof(DWORD))) 
							Fatal_Error( "BUFFER OVERRUN(2)!!!" );

						if(*pSrchPtr == pAttribKey[i*2])	//Look for the current item that is is reinstating it's contiguity.
						{
							ShiftBuf( pAttribPtr, pSrchPtr, pTriPtr, pT2 );

							/*
							//Switch the values
							pDTEMP =		*pSrchPtr;
							*pSrchPtr =		*pAttribPtr;
							*pAttribPtr =	pDTEMP;

							pTTEMP =	*pT2;
							*pT2 =		*pTriPtr;
							*pTriPtr =	pTTEMP;
							*/
							k = nAttributes-1;//exit the loop
							//Reset the pointers
						}
						pSrchPtr++;
						pT2++;
						
					}

				}//if pAttribPtr != pAttribKey[i*2]

				pAttribPtr++;
				pTriPtr++;
				COUNTER1++;
			}//for j < nAttributes
			//Reset the pointers
		}//for i < nAttribSets
	}//pIndexbuffer->Lock()
	hr = pIndexBuffer->Unlock(); if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );

}
/*----------------------------------
	CreateAttributes()
		Does all the secondary
		data creation routines.
----------------------------------*/
void Mesh_Class::CreateAttributes()
{
	MakeAttribKey();
	SortAttributes();

	BoundCube._Calc( pVertices, Num_Vertices );
}
void Mesh_Class::ResetDrawBuffer()
{
	memset( pCanDrawPoly, 1, Num_Faces*sizeof(BOOL) );
}

/*--------------------------------------
	FillRenderBuffer -
		Fills the rendering buffer, which
		is a seperate index buffer, with
		parts of the index buffer for
		rendering.  The index buffer
		is never actually rendered.
--------------------------------------*/
void Mesh_Class::FillRenderBuffer( DWORD AttribID )
{
	HRESULT hr;
	DWORD i;
	
	WORD* pInd;
	WORD* pRen;

	DWORD off=0;
	DWORD siz=0;

	for( i=0; i<nAttribSets; ++i )
	{
		if(pAttribKey[i*2]==AttribID)
		{
			Buf_Faces = pAttribKey[(i*2)+1];
			i=nAttribSets;
		}
		else off+=pAttribKey[(i*2)+1];
	}


	off = off*(sizeof(WORD)*3);
	siz = Buf_Faces*(sizeof(WORD)*3);

	hr = pIndexBuffer->Lock( off, siz, (void**)&pInd, D3DLOCK_READONLY); if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );
	hr = pRenderBuffer->Lock( 0, 0, (void**)&pRen, D3DLOCK_DISCARD ); if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );
	
	memcpy( pRen, pInd, siz );

	pRenderBuffer->Unlock();
	pIndexBuffer->Unlock();


}
/*--------------------------------------
	CND_FillRenderBuffer - 
		Fills the rendering buffer, which
		is a seperate index buffer, with
		parts of the index buffer for
		rendering.  The index buffer
		is never actually rendered.
		This is conditionally if the
		polygon is visible.
		This is slow, should just use
		post-processing.
--------------------------------------*/
void Mesh_Class::CND_FillRenderBuffer( DWORD AttribID )
{
	HRESULT hr;
	DWORD i;
	
	WORD* pInd;
	WORD* pRen;

	DWORD off=0;	//offset into the index buffer
	DWORD siz=0;	//size of the index buffer to lock

	for( i=0; i<nAttribSets; ++i )
	{
		if(pAttribKey[i*2]==AttribID)
		{
			Buf_Faces = pAttribKey[(i*2)+1];
			i=nAttribSets;
		}
		else off+=pAttribKey[(i*2)+1];
	}
	//Buf faces equals the number of faces of this attribute

	DWORD off2 = off; // offset to start at, in faces.
	DWORD siz2 = Buf_Faces; // size to go through, in faces.

	off = off*(sizeof(WORD)*3);
	siz = Buf_Faces*(sizeof(WORD)*3);

	hr = pIndexBuffer->Lock( off, siz, (void**)&pInd, D3DLOCK_READONLY ); if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );
	hr = pRenderBuffer->Lock( 0, 0, (void**)&pRen, D3DLOCK_DISCARD ); if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );
	
	DWORD cur=0;//Must start at 0, the index buffer is set to the offset
	for( DWORD i=off2; i<(off2+siz2); ++i )
	{
		if(pCanDrawPoly[i])
		{
			memcpy( pRen, pInd+cur, sizeof(WORD)*3 );
			pRen+=3;
		}
		else
		{
			//hey look it seems to work in oneway or another
			int TEST=0; 
			TEST++;
		}
		cur+=3;
	}

	pRenderBuffer->Unlock();
	pIndexBuffer->Unlock();


}
/*----------------------------------
	AddTilePolys
		- add the polygons of
		a tile at the specified
		offset in vertices;
		- only adds the top polys
----------------------------------*/
void Mesh_Class::AddTilePolys( DWORD TileOffset )
{
	


}
/*------------------------------
	FillVertexBuffer -
		This function simply
		re-fills the vertex buffer
		of the mesh. It should
		be called whenever the
		mesh's vertices have been
		changed.
-------------------------------*/
void Mesh_Class::FillVertexBuffer()
{
	HRESULT hr;
	Vertex* p_v;
	hr = pVertexBuffer->Lock( 0, 0, (void**)&p_v, D3DLOCK_DISCARD ); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );
	memcpy( p_v, pVertices, Num_Vertices*sizeof(Vertex));
	hr = pVertexBuffer->Unlock(); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );
}
/*-------------------------------
	UpdateSingle -
		Only updates a single
		vertex in the mesh.
		offeset - offset into
		the pVertices array (in vertices)
-------------------------------*/
void Mesh_Class::UpdateSingle( DWORD offset )
{
	HRESULT hr;
	Vertex* p_v;
	offset *= sizeof(Vertex);
	hr = pVertexBuffer->Lock( offset, (UINT)sizeof(Vertex), (void**)&p_v, 0 ); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );
	memcpy( p_v, pVertices+offset, sizeof(Vertex) );
	hr = pVertexBuffer->Unlock(); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );
}
/*-------------------------------
	UpdateRange -
		Updates a range of
		vertexes in the mesh.
		offeset - offset into
		the pVertices array (in vertices).
		range - range (in vertices) to
		update.
-------------------------------*/
void Mesh_Class::UpdateRange( DWORD offset, DWORD range )
{
	HRESULT hr;
	Vertex* p_v;
	offset*=sizeof(Vertex);
	range*=sizeof(Vertex);
	hr = pVertexBuffer->Lock( offset, range, (void**)&p_v, 0 ); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );
	memcpy( p_v, pVertices+offset, range );
	hr = pVertexBuffer->Unlock(); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );
}
/*-----------------------------------------
		Quick visibility determination
		just checks to see if the mesh's
		bound cube is visible.
-----------------------------------------*/
void Mesh_Class::Vis_Quick()
{
	if( BoundCube._Visible( pFrustum ) )
		bCanDraw=true;
	else
		bCanDraw=false;
}
/*-----------------------------------------
		Per-Poly visibility determination.
		This level can reduce the
		over-all amount of polygons
		that the mesh will draw.
-----------------------------------------*/
void Mesh_Class::Vis_PerPoly()
{

}
/*-------------------------------------------
		Main Rendering function;
		does visibility determination
		and fills the rendering
		buffer.  Then this function
		will draw the pieces of
		the primitive that are visible
		to with the index buffer.
-------------------------------------------*/
void Mesh_Class::Render( DWORD AttribID )
{
	//called by the encapsulating structure
	//Vis_Quick();
	if(!bCanDraw) return; // - Easy out.
	//Vis_PerPoly();

	//ResetDrawBuffer();
	CND_FillRenderBuffer( AttribID );

	//FillRenderBuffer( AttribID );

	pTC->Set_Texture( AttribID );

	HRESULT hr;

	hr = g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW ); 
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__);
	
	hr = g_D3D_Struct->g_pD3DDevice->SetFVF( FVF_DESCRIPTOR ); 
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__);
	
	hr = g_D3D_Struct->g_pD3DDevice->SetStreamSource( 0, pVertexBuffer, 0, sizeof(Vertex) ); 
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__);


	/* Draw Indices (regular) */
 	hr = g_D3D_Struct->g_pD3DDevice->SetIndices( pRenderBuffer ); 
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__);

	//Note to self: to render a line list (or just lines) select Num_Faces*3-1
	hr = g_D3D_Struct->g_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, Num_Vertices, 0, Buf_Faces ); 
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__);
	
	/* TEST INDICES 
	if(!Draw_Verts)
	{
	hr = g_D3D_Struct->g_pD3DDevice->SetIndices( pIndexBuffer ); 
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__);

	hr = g_D3D_Struct->g_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, Num_Vertices, 0, Num_Faces);
	}
	/* TEST VERTICES 
	else
	{
	float pointSize = 3.0f;
	g_D3D_Struct->g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));
	hr = g_D3D_Struct->g_pD3DDevice->DrawPrimitive( D3DPT_POINTLIST, 0, Num_Vertices); 
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__);

	}
	/**/
	/* TEST VERTICES ##FUCKS UP!!##
	//hr = g_D3D_Struct->g_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, Num_Vertices); 
	//if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__);

	/**/

}
/*----------------------------------
		Free Resources
-----------------------------------*/
void Mesh_Class::Free()
{
	if( pCanDrawPoly )
	{
		delete [] pCanDrawPoly;
	}
	if( pVertices )
	{
		delete [] pVertices;
	}
	if( pAttribKey)
	{
		delete [] pAttribKey;
		pAttribKey=0;
	}
	if(pAttributes)
	{
		delete [] pAttributes;
		pAttributes=0;
	}
	if(pRenderBuffer)
	{
		pRenderBuffer->Release();
		pRenderBuffer = 0;
	}
	if(pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer=0;
	}
	if(pIndexBuffer)
	{
		pIndexBuffer->Release();
		pIndexBuffer=0;
	}
}
Mesh_Class::Mesh_Class(){ memset(this,0,sizeof(Mesh_Class)); }