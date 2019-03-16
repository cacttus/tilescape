
#include "Tile_Controller_Class.h"
#include "Filedef.h"


//Raises the top left vertex of the specified mesh
//at the specified offset
void Tile_Controller_Class::RaiseVert_TL( Mesh_Class* pm, DWORD offset )
{
	pm->pVertices[offset+0].y += (float)Base_Height;
	pm->pVertices[offset+5].y += (float)Base_Height;
	pm->pVertices[offset+16].y += (float)Base_Height;

	pm->pVertices[offset+5].v -= 1.0f;
	pm->pVertices[offset+16].v -= 1.0f;
}
void Tile_Controller_Class::RaiseVert_BL( Mesh_Class* pm, DWORD offset )//-- note switched TR with BL
{
	pm->pVertices[offset+1].y += (float)Base_Height;
	pm->pVertices[offset+4].y += (float)Base_Height;
	pm->pVertices[offset+9].y += (float)Base_Height;

	pm->pVertices[offset+4].v -= 1.0f;
	pm->pVertices[offset+9].v -= 1.0f;
}
void Tile_Controller_Class::RaiseVert_TR( Mesh_Class* pm, DWORD offset )
{
	pm->pVertices[offset+3].y += (float)Base_Height;
	pm->pVertices[offset+12].y += (float)Base_Height;
	pm->pVertices[offset+17].y += (float)Base_Height;

	pm->pVertices[offset+12].v -= 1.0f;
	pm->pVertices[offset+17].v -= 1.0f;
}
void Tile_Controller_Class::RaiseVert_BR( Mesh_Class* pm, DWORD offset )
{
	pm->pVertices[offset+2].y += (float)Base_Height;
	pm->pVertices[offset+8].y += (float)Base_Height;
	pm->pVertices[offset+13].y += (float)Base_Height;

	pm->pVertices[offset+8].v -= 1.0f;
	pm->pVertices[offset+13].v -= 1.0f;
}
/*--------------------------------------------
	Culls the base polygons in mesh pm
	at offset offset.
	the offset should be relative to the
	plane that the bases reside on, e.g.
	specify the top left corner vertex, of the
	two planes to cull.
	plane is which side to cull( in vertices )
--------------------------------------------*/
void Tile_Controller_Class::CullBases( Mesh_Class *pm, DWORD offset, DWORD plane )
{
	if( !( (plane==4) || (plane==8) || (plane==12) || (plane==16) ) )
		Fatal_Error( "Plane Overflow" );
	//Assuming that the mesh class has re-arranged
	//the attributes so that the bases are at the
	//end of the attribute buffer, in order. (um..)
	//offset is in vertices, this needs to get into base-faces.
	offset /=4; //get into square space (that is 2 polys per square and 4 verts per square)
	offset -= (offset/5); //get rid of tiles and into square-base space (5 squares per tile)
	//in polygon space
	DWORD TEST = ((SubAxis_Tiles*SubAxis_Tiles)*2) +// jump past the tiles
		    (offset*2); //back into polygon space and add.
		/*((SubAxis_Tiles*SubAxis_Tiles)*2)+  //jump forward in the buffer, past the tiles
			( (offset - ((offset/20)*2))//get rid of tiles
			/3 //get back into tile space (/8)??? /16????
			) ;*/
	

	if( (TEST >= pm->Num_Faces) || (TEST+1 >= pm->Num_Faces) ) 
		Fatal_Error( "The number of faces was exceeded" );
	
	pm->pCanDrawPoly[TEST] = FALSE;
	pm->pCanDrawPoly[TEST+1] = FALSE;

}
/*------------------------------------
	Checks to see if the bases' polys
	are actually visible.
------------------------------------*/
void Tile_Controller_Class::CheckBaseCull( Mesh_Class* pm, DWORD offset, DWORD v0, DWORD dv0, DWORD v1, DWORD dv1, WORD side )
{
	INT off = 0;

		 if(side==0) off = (SubAxis_Tiles*20)* -1;//compiler complained at me
	else if(side==1) off = 20;
	else if(side==2) off = (SubAxis_Tiles*20);
	else if(side==3) off = -20;
	else return;
	if( offset+off+19 > (pm->Num_Faces*20) ) 
		Fatal_Error( "Over flow check u r c o d e" );
	else if( offset+off+19 <0 ) 
		Fatal_Error( "UndeRFLow CheCkUrCodE" );
	if(pm->pVertices[offset+v0].y >= pm->pVertices[(offset+off)+dv0].y )
	{
		if(pm->pVertices[offset+v1].y > pm->pVertices[(offset+off)+dv1].y )
		{
			//Cull the other tile's bases
			CullBases( pm, (offset+off), dv1 );//cull the bases of the other one
		}
		else if((int)pm->pVertices[offset+v1].y == (int)pm->pVertices[(offset+off)+dv1].y )
		{
			//They are equal, Cull both bases
			CullBases( pm, offset, v0 );
			CullBases( pm, (offset+off), dv1 );
		}
	}
	else if(pm->pVertices[offset+v0].y < pm->pVertices[(offset+off)+dv0].y )
	{
		if(pm->pVertices[offset+v1].y < pm->pVertices[(offset+off)+dv1].y )
		{
			//Cull this tile's bases
			CullBases( pm, offset, v0 );
		}
	}
}
/*--------------------------------------------
	Checks to see if the bases are visible
	parms: a 
	mesh (pm) which contains the bases
	offset to a tile in vertices (offset)
	This function sets the boolean buffer
	to the appropriate value depending on
	wether the bases are visible or not.
--------------------------------------------*/
void Tile_Controller_Class::CheckBasesVisible( Mesh_Class *pm, DWORD offset )
{
	//I just took this code from the other one
	if(offset>pm->Num_Vertices) Fatal_Error( "WHOA! bad offset!" );
	UINT	TileID = offset / 20;
	DWORD	PassFlags = 0;
	if( TileID < SubAxis_Tiles )							PassFlags |= FLAG_T_T;	// - tile is at top of mesh
	if( (FLOAT) ((TileID+1)%(SubAxis_Tiles)) == 0.0f )		PassFlags |= FLAG_T_R;	// - tile is to right of mesh
	if( (FLOAT) (TileID%(SubAxis_Tiles)) == 0.0f )			PassFlags |= FLAG_T_L;	// - tile is to left of mesh
	if( TileID >= ((SQR(SubAxis_Tiles))-SubAxis_Tiles) )	PassFlags |= FLAG_T_B;	// - tile is at bottom of mesh
	
	if( !(PassFlags & FLAG_T_T ) ) //if this tile is not at the top
	{
		//check it and the tile it faces
		//check 0&1 vs 5&4
		CheckBaseCull( pm, offset, 4, 13, 5, 12, 0 );

	}
	if( !(PassFlags & FLAG_T_R ) ) //if this tile is not at the r
	{
		//check it and the tile it faces
		//check 1&2 vs 9&8
		CheckBaseCull( pm, offset, 8, 17, 9, 16, 1 );
	}
	if( !(PassFlags & FLAG_T_B ) ) //if this tile is not at the b
	{
		//check it and the tile it faces
		//check 3&2 vs 12&13
		CheckBaseCull( pm, offset, 12, 5, 13, 4, 2 );
	}
	if( !(PassFlags & FLAG_T_L ) ) //if this tile is not at the l
	{
		//check it and the tile it faces
		//check 0&3 vs 16&17
		CheckBaseCull( pm, offset, 16, 9, 17, 8, 3 );
	}
}
/*---------------------------------
	RaiseHillTile()
		Raises a tile's vertices
		as if they were a hill.
		The vertexes of all the
		surrounding tiles are
		also raised in unison.
---------------------------------*/
void Tile_Controller_Class::RaiseHillTile( Hit_Struct* phs )
{

	UINT TileID = phs->Offset / 20;
	DWORD PassFlags = 0;
	Mesh_Class*		pBorderMesh=0;

	UINT Axis_Meshes = Axis_Tiles/SubAxis_Tiles;

	if( phs->pMesh->Index==0 ){ PassFlags |= FLAG_TBM; PassFlags|= FLAG_LBM; }
	else if( phs->pMesh->Index==SQR(Axis_Meshes)-1){ PassFlags |= FLAG_RBM; PassFlags |= FLAG_BBM; }

	else
	{
		//First check if this mesh is an edge mesh
		if( phs->pMesh->Index < Axis_Meshes ) // - Mesh is Top border
			PassFlags |= FLAG_TBM;

		if( (FLOAT)(phs->pMesh->Index%(Axis_Meshes)) ) // - Mesh is right border
			PassFlags |= FLAG_RBM;		
		
		if( !(FLOAT)( phs->pMesh->Index % Axis_Meshes) ) // - Mesh is to the left
			PassFlags |= FLAG_LBM;

		if( phs->pMesh->Index >= (Num_Meshes-Axis_Meshes) ) // - Mesh is at the bottom
			PassFlags |= FLAG_BBM;

	}

	if( TileID < SubAxis_Tiles )							PassFlags |= FLAG_T_T;	// - tile is at top of mesh
	if( (FLOAT) ((TileID+1)%(SubAxis_Tiles)) == 0.0f )		PassFlags |= FLAG_T_R;	// - tile is to right of mesh
	if( (FLOAT) (TileID%(SubAxis_Tiles)) == 0.0f )			PassFlags |= FLAG_T_L;	// - tile is to left of mesh
	if( TileID >= ((SQR(SubAxis_Tiles))-SubAxis_Tiles) )	PassFlags |= FLAG_T_B;	// - tile is at bottom of mesh


	//Top left vertex
	if( (phs->TriVert == 0)||(phs->TriVert==4) )
	{
		//---------------Raise the actual vertex
		RaiseVert_TL( phs->pMesh, phs->Offset );
		//The definite pseudocode
			//see if tile is bordering this mesh
				//see if there is a mesh above this one
			//else
			//the tile does not affect other meshes

		//-------------------------------Handle Top tile
		if( PassFlags & FLAG_T_T )
		{
			if( !(PassFlags & FLAG_TBM) )
			{
				//there is a mesh above calculate!
				pBorderMesh = pMeshes[phs->pMesh->Index - (Axis_Meshes)]; //the top mesh (above this one)
				RaiseVert_BL( pBorderMesh, ((SQR(SubAxis_Tiles))*20)-((SubAxis_Tiles*20)-phs->Offset) ); //Raise the vert
			}
		}
		else
		{
			RaiseVert_BL( phs->pMesh, phs->Offset-(SubAxis_Tiles*20) ); //Just raise vert of tile above
		}
		if(pBorderMesh)pBorderMesh->FillVertexBuffer();//probably better to put it inside the if() and use offset fill
		pBorderMesh=0;

		//----------------------------Handle Left Tile
		if( PassFlags & FLAG_T_L )
		{
			if( !(PassFlags & FLAG_LBM) )//if this is not a left border mesh
			{
				pBorderMesh = pMeshes[phs->pMesh->Index-1]; //mesh to the left of this
				RaiseVert_TR( pBorderMesh, phs->Offset+(SubAxis_Tiles*20)-20);
			}
		}
		else
		{
			RaiseVert_TR( phs->pMesh, (phs->Offset-20) ); //Just raise vert of tile above
		}
		if(pBorderMesh)pBorderMesh->FillVertexBuffer();//probably better to put it inside the if() and use offset fill
		pBorderMesh=0;

		//--------------------------------------------Handle Top Left Corner Tile		
			if( (PassFlags & FLAG_T_T) )//The tile resides at the top
			{
				//check if the tile also resides at the left
				if( (PassFlags & FLAG_T_L ) )
				{
					//if so , process the top/left mesh
					if(   !(PassFlags & FLAG_LBM) && !(PassFlags & FLAG_TBM) )//if the mesh is not at the top left
					{
						pBorderMesh = pMeshes[phs->pMesh->Index - Axis_Meshes - 1];
						RaiseVert_BR( pBorderMesh, (SQR(SubAxis_Tiles)*20)-20 );
					}

				}
				else
				{
					if( !(PassFlags & FLAG_TBM)  ) // if the mesh is not a top border mesh
					{
						//process the top mesh
						pBorderMesh = pMeshes[phs->pMesh->Index - Axis_Meshes];
						RaiseVert_BR( pBorderMesh, (SQR(SubAxis_Tiles)*20) - ((SubAxis_Tiles*20)-phs->Offset) - 20 );
					}
				}
			}
			else
			{
				if( PassFlags & FLAG_T_L ) //the tile is at the left of the mesh
				{
					if(  !(PassFlags & FLAG_LBM) ) // if the mesh is not a left border mesh
					{
						//process the left mesh
						pBorderMesh = pMeshes[phs->pMesh->Index-1];
						RaiseVert_BR( pBorderMesh, phs->Offset-20 );
					}
				}
				else
				{
					//just raise it it is in the same mesh
					pBorderMesh = 0;
					RaiseVert_BR( phs->pMesh, phs->Offset-(SubAxis_Tiles*20)-20 );
				}
			}

		if(pBorderMesh) pBorderMesh->FillVertexBuffer();

	}//if the trivert is==0
	if((phs->TriVert==3)||(phs->TriVert==4))//it's the top right (vert)
	{
		//The same tile
		RaiseVert_TR( phs->pMesh, phs->Offset );

		//-----------------------------------------------------------Handle Top tile
		if( PassFlags & FLAG_T_T )//if this tile is a top tile
		{
			if( !(PassFlags & FLAG_TBM) ) //if it's not a top border mesh
			{
				//there is a mesh above calculate!
				pBorderMesh = pMeshes[phs->pMesh->Index - (Axis_Meshes)]; //the top mesh (above this one)
				RaiseVert_BR( pBorderMesh, ((SQR(SubAxis_Tiles))*20)-((SubAxis_Tiles*20)-phs->Offset) ); //Raise the vert
			}
		}
		else
		{
			RaiseVert_BR( phs->pMesh, phs->Offset-(SubAxis_Tiles*20) ); //Just raise vert of tile above
		}
		if(pBorderMesh)pBorderMesh->FillVertexBuffer();//probably better to put it inside the if() and use offset fill
		pBorderMesh=0;

		//---------------------------------------------------------Handle TopRight Middle Corner Tile		
			if( (PassFlags & FLAG_T_T) )//The tile resides at the top
			{
				//check if the tile also resides at the right
				if( (PassFlags & FLAG_T_R ) )
				{
					//if so , process the top/left mesh
					if(   !(PassFlags & FLAG_RBM) && !(PassFlags & FLAG_TBM) )//if the mesh is not at the top right
					{
						pBorderMesh = pMeshes[phs->pMesh->Index - Axis_Meshes + 1];
						RaiseVert_BL( pBorderMesh, (SQR(SubAxis_Tiles)*20)-(SubAxis_Tiles*20) );
					}

				}
				else
				{
					if( !(PassFlags & FLAG_TBM)  ) // if the mesh is not a top border mesh
					{
						//process the top mesh
						pBorderMesh = pMeshes[phs->pMesh->Index - Axis_Meshes];
						RaiseVert_BL( pBorderMesh, (SQR(SubAxis_Tiles)*20) - ((SubAxis_Tiles*20)-phs->Offset) + 20 );
					}
				}
			}
			else
			{
				if( PassFlags & FLAG_T_R ) //the tile is at the right of the mesh
				{
					if(  !(PassFlags & FLAG_RBM) ) // if the mesh is not a riht border mesh
					{
						//process the right mesh
						pBorderMesh = pMeshes[phs->pMesh->Index+1];
						RaiseVert_BL( pBorderMesh, phs->Offset-((SubAxis_Tiles*20)*2)+20 );
					}
				}
				else
				{
					//just raise it it is in the same mesh
					pBorderMesh = 0;
					RaiseVert_BL( phs->pMesh, phs->Offset-(SubAxis_Tiles*20)+20 );
				}
			}

		if(pBorderMesh) pBorderMesh->FillVertexBuffer();
		
		//------------------------------------------------------------Right tile
		if( PassFlags & FLAG_T_R )//if the tile is to the right of the mesh
		{
			if( !(PassFlags & FLAG_RBM) )//if this is not a right border mesh
			{
				pBorderMesh = pMeshes[phs->pMesh->Index+1]; //mesh to the left of this
				RaiseVert_TL( pBorderMesh, phs->Offset-(SubAxis_Tiles*20)+20);
			}
		}
		else
		{
			RaiseVert_TL( phs->pMesh, (phs->Offset+20) ); //Just raise vert of tile above
		}
		if(pBorderMesh)pBorderMesh->FillVertexBuffer();//probably better to put it inside the if() and use offset fill
		pBorderMesh=0;


	}
	if( (phs->TriVert==1)||(phs->TriVert==4) ) //it's the bottom left
	{
		// Handle the same
		RaiseVert_BL( phs->pMesh, phs->Offset );

		//----------------------------Handle Left Tile
		if( PassFlags & FLAG_T_L )
		{
			if( !(PassFlags & FLAG_LBM) )//if this is not a left border mesh
			{
				pBorderMesh = pMeshes[phs->pMesh->Index-1]; //mesh to the left of this
				RaiseVert_BR( pBorderMesh, phs->Offset+(SubAxis_Tiles*20)-20);
			}
		}
		else
		{
			RaiseVert_BR( phs->pMesh, (phs->Offset-20) ); //Just raise vert of tile above
		}
		if(pBorderMesh)pBorderMesh->FillVertexBuffer();//probably better to put it inside the if() and use offset fill
		pBorderMesh=0;


		// Handle the bottom left
		//---------------------------------------------------------Handle bottom left	
		
			if( (PassFlags & FLAG_T_B) )//The tile resides at the bottom
			{
				//check if the tile also resides at the left
				if( (PassFlags & FLAG_T_L ) )
				{
					//if so , process the bottom left mesh
					if(   !(PassFlags & FLAG_LBM) && !(PassFlags & FLAG_BBM) )//if the mesh is not at the bottom left
					{
						pBorderMesh = pMeshes[phs->pMesh->Index + Axis_Meshes - 1];
						RaiseVert_TR( pBorderMesh, (SubAxis_Tiles*20) - 20 );
					}

				}
				else
				{
					if( !(PassFlags & FLAG_BBM)  ) // if the mesh is not a Bottom border mesh
					{
						//process the bttom mesh
						pBorderMesh = pMeshes[phs->pMesh->Index + Axis_Meshes];
						RaiseVert_TR( pBorderMesh, ((SubAxis_Tiles*20) - (((SQR(SubAxis_Tiles))*20)-(phs->Offset)))-20 );//might be right
					}
				}
			}
			else
			{
				if( PassFlags & FLAG_T_L ) //the tile is at the left of the mesh
				{
					if(  !(PassFlags & FLAG_LBM) ) // if the mesh is not a left border mesh
					{
						//process the left mesh
						pBorderMesh = pMeshes[phs->pMesh->Index-1];
						RaiseVert_TR( pBorderMesh, phs->Offset+((SubAxis_Tiles*20)*2)-20 );
					}
				}
				else
				{
					//just raise it it is in the same mesh
					pBorderMesh = 0;
					RaiseVert_TR( phs->pMesh, phs->Offset+(SubAxis_Tiles*20)-20 );
				}
			}

		if(pBorderMesh) pBorderMesh->FillVertexBuffer();

		//-----------------------------------------------------------Handle Bottom tile
		if( PassFlags & FLAG_T_B )//if this tile is a bottom tile
		{
			if( !(PassFlags & FLAG_BBM) ) //if it's not a top border mesh
			{
				//there is a mesh above calculate!
				pBorderMesh = pMeshes[phs->pMesh->Index + (Axis_Meshes)]; //the top mesh (above this one)
				RaiseVert_TL( pBorderMesh, (SubAxis_Tiles*20) - (((SQR(SubAxis_Tiles))*20)-(phs->Offset)) ); //Raise the vert
			}
		}
		else
		{
			RaiseVert_TL( phs->pMesh, phs->Offset+(SubAxis_Tiles*20) ); //Just raise vert of tile below
		}
		if(pBorderMesh)pBorderMesh->FillVertexBuffer();//probably better to put it inside the if() and use offset fill
		pBorderMesh=0;


	}
	if( (phs->TriVert==2)||(phs->TriVert==4) ) //it's the bottom right
	{
		RaiseVert_BR( phs->pMesh, phs->Offset );
		//-----------------------------------------------------------Handle Bottom tile
		if( PassFlags & FLAG_T_B )//if this tile is a bottom tile
		{
			if( !(PassFlags & FLAG_BBM) ) //if it's not a bottom border mesh
			{
				//there is a mesh above calculate!
				pBorderMesh = pMeshes[phs->pMesh->Index + (Axis_Meshes)]; //the top mesh (above this one)
				int TEST = (SubAxis_Tiles*20) - (((SQR(SubAxis_Tiles))*20)-(phs->Offset));
				RaiseVert_TR( pBorderMesh, TEST ); //Raise the vert
			}
		}
		else
		{
			RaiseVert_TR( phs->pMesh, phs->Offset+(SubAxis_Tiles*20) ); //Just raise vert of tile above
		}
		if(pBorderMesh)pBorderMesh->FillVertexBuffer();//probably better to put it inside the if() and use offset fill
		pBorderMesh=0;

		//--------------------------------------------------------------Handle the Bottom right
			if( (PassFlags & FLAG_T_B) )//The tile resides at the bottom
			{
				//check if the tile also resides at the right
				if( (PassFlags & FLAG_T_R ) )
				{
					//if so , process the bottom left mesh
					if(   !(PassFlags & FLAG_RBM) && !(PassFlags & FLAG_BBM) )//if the mesh is not at the bottom left
					{
						pBorderMesh = pMeshes[phs->pMesh->Index + Axis_Meshes + 1];
						RaiseVert_TL( pBorderMesh, 0 );
					}

				}
				else
				{
					if( !(PassFlags & FLAG_BBM)  ) // if the mesh is not a Bottom border mesh
					{
						//process the bttom mesh
						pBorderMesh = pMeshes[phs->pMesh->Index + Axis_Meshes];
						RaiseVert_TL( pBorderMesh, ((SubAxis_Tiles*20) - ((SQR(SubAxis_Tiles)*20)-(phs->Offset))) + 20 );//again (?) might be right
					}
				}
			}
			else
			{
				if( PassFlags & FLAG_T_R ) //the tile is at the left of the mesh
				{
					if(  !(PassFlags & FLAG_RBM) ) // if the mesh is not a left border mesh
					{
						//process the left mesh
						pBorderMesh = pMeshes[phs->pMesh->Index+1];
						RaiseVert_TL( pBorderMesh, phs->Offset+20 );
					}
				}
				else
				{
					//just raise it it is in the same mesh
					pBorderMesh = 0;
					RaiseVert_TL( phs->pMesh, phs->Offset+(SubAxis_Tiles*20)+20 );
				}
			}

		if(pBorderMesh) pBorderMesh->FillVertexBuffer();
		// ---------------------------------------------------------------------Handle the right
		if( PassFlags & FLAG_T_R )
		{
			if( !(PassFlags & FLAG_RBM) )//if this is not a left border mesh
			{
				pBorderMesh = pMeshes[phs->pMesh->Index+1]; //mesh to the left of this
				RaiseVert_BL( pBorderMesh, phs->Offset-(SubAxis_Tiles*20)+20);
			}
		}
		else
		{
			RaiseVert_BL( phs->pMesh, (phs->Offset+20) ); //Just raise vert of tile above
		}
		if(pBorderMesh)pBorderMesh->FillVertexBuffer();//probably better to put it inside the if() and use offset fill
		pBorderMesh=0;

	}
	
	//If going to keep the hill fn the way it is
	// then this is not needed, but if going to
	// use the raise lowest vert first, then it is.
	CheckBasesVisible( phs->pMesh, phs->Offset );

	phs->pMesh->FillVertexBuffer();
}

/*----------------------------------------------------------------------
	RaiseSingleVert()
		Raises a single vertex, not dependant on the tile.
		Takes the amount to raise "Amt" and a pointer to the hit str-
		ucture that should have stored the last user hit.
		//Amt should not be used, it is the base height//--HACK HACK HACK
----------------------------------------------------------------------*/
void Tile_Controller_Class::RaiseSingleTile( Hit_Struct* phs )
{
	//int LocalSpace = phs->Offset + phs->TriVert; //% 20; // - Gets the local space of the vertices from 0 to 19.
	//int Rel1, Rel2, Rel3, Rel4; // - 4 Relative vertices to raise

		if( phs->TriVert == 0  )
		{ 
			RaiseVert_TL( phs->pMesh, phs->Offset );
		}

		else if( phs->TriVert == 1  )
		{ 
			RaiseVert_TR( phs->pMesh, phs->Offset );
		}

		else if( phs->TriVert == 2  )
		{	
			RaiseVert_BR( phs->pMesh, phs->Offset );
		}

		else if( phs->TriVert == 3  )
		{	
			RaiseVert_BL( phs->pMesh, phs->Offset );
		}

		else if( phs->TriVert== 4 )
		{
			RaiseVert_TL( phs->pMesh, phs->Offset );
			RaiseVert_TR( phs->pMesh, phs->Offset );
			RaiseVert_BR( phs->pMesh, phs->Offset );
			RaiseVert_BL( phs->pMesh, phs->Offset );

		}
	CheckBasesVisible( phs->pMesh, phs->Offset );
	phs->pMesh->FillVertexBuffer();
}
/*----------------------------------------------------------------------
	LowerSingleTile()
		Raises a single vertex, not dependant on the tile.
		Takes the amount to raise "Amt" and a pointer to the hit str-
		ucture that should have stored the last user hit.
		//Amt should not be used, it is the base height//--HACK HACK HACK
----------------------------------------------------------------------*/
void Tile_Controller_Class::LowerSingleTile( Hit_Struct* phs )
{
	//int LocalSpace = phs->Offset + phs->TriVert; //% 20; // - Gets the local space of the vertices from 0 to 19.
	//int Rel1, Rel2, Rel3, Rel4; // - 4 Relative vertices to raise

		if( phs->TriVert == 0  )
		{ 
			phs->pMesh->pVertices[phs->Offset+0].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+5].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+16].y -= (float)Base_Height;
		}

		else if( phs->TriVert == 1  )
		{ 
			phs->pMesh->pVertices[phs->Offset+1].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+4].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+9].y -= (float)Base_Height;
		}

		else if( phs->TriVert == 2  )
		{	
			phs->pMesh->pVertices[phs->Offset+2].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+8].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+13].y -= (float)Base_Height;
		}

		else if( phs->TriVert == 3  )
		{	
			phs->pMesh->pVertices[phs->Offset+3].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+12].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+17].y -= (float)Base_Height;
		}

		else if( phs->TriVert== 4 )
		{
			phs->pMesh->pVertices[phs->Offset+0].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+1].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+2].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+3].y -= (float)Base_Height;

			phs->pMesh->pVertices[phs->Offset+5].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+16].y -= (float)Base_Height;

			phs->pMesh->pVertices[phs->Offset+4].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+9].y -= (float)Base_Height;

			phs->pMesh->pVertices[phs->Offset+8].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+13].y -= (float)Base_Height;

			phs->pMesh->pVertices[phs->Offset+12].y -= (float)Base_Height;
			phs->pMesh->pVertices[phs->Offset+17].y -= (float)Base_Height;

		}

	phs->pMesh->FillVertexBuffer();
}
/*---------------------------------------
		  Instantiate the mesh
		  Num_Axis_Tiles must be divisible
		  by 16;
----------------------------------------*/
void Tile_Controller_Class::Init2( int Num_Axis_Tiles, Texture_Controller_Class* ptc, Frustum_Class* pfr )
{
	SubAxis_Tiles = 16;
	if(Num_Axis_Tiles%SubAxis_Tiles!=0.0f) return;

	pTC = ptc;
	pFR = pfr;

	//Here - Load the (default) textures for the tiles
	ptc->Add_Texture( TILETEX_GRASS, TEXID_TILE_GRASS );

	ptc->Add_Texture( BASETEX_ROCK, TEXID_BASE_ROCK );

	Axis_Tiles = Num_Axis_Tiles;

	Num_Tiles = Axis_Tiles * Axis_Tiles;

	Tile_Width =	5.00;		
	Base_Height =	2.50;//--HACK

	pTiles = new Tile_Class[Num_Tiles];

	Generate( 0 );

}

/*-------------------------------------------
			Generate the mesh.
-------------------------------------------*/
void Tile_Controller_Class::Generate( int Axis )
{

	DWORD i=0, j=0, k=0, l=0, m, 
		Cur_Mesh=0, Cur_Tile=0, Cur_Vert=0, Cur_Index=0;
	DOUBLE v_pw = 0.0, v_ph=0.0; //added to vert x and vert z

	HRESULT hr;

	WORD*		p_i=0;	// pointer to indices
	DWORD*		p_a=0;	// pointer to attribs
	Vertex*		p_v=0;  // pointer to vertices

	DWORD		Attrib_Ind=0;	//Index of the current attribute

	Num_Meshes = Num_Tiles/(SubAxis_Tiles*SubAxis_Tiles);
	pMeshes = new Mesh_Class*[Num_Meshes];
	
	/* Simpler code declarations */
	Vertex* pVM = 0;		//For simpler code - Pointer to the vertices in the current mesh
	float cW = 0.0f;		//For simpler code - Current Point x calculation
	float cH = 0.0f;		//For simpler code - Current Point Z calculation

	//Instantiate the meshes and create the axis tiles
	for( i=0; i<(Axis_Tiles/SubAxis_Tiles); ++i )
	{
		for( j=0; j<(Axis_Tiles/SubAxis_Tiles); ++j )
		{

			Cur_Mesh = ((Axis_Tiles/SubAxis_Tiles)*j) + i;//-- note switched j and i to change the indexes (willitwork?)

			pMeshes[Cur_Mesh] = new Mesh_Class();
			
			pMeshes[Cur_Mesh]->Index = Cur_Mesh;
			pMeshes[Cur_Mesh]->pTC = pTC;

			if(Cur_Mesh>=Num_Meshes) Fatal_Error( "Mesh buffer overflow" );

			pMeshes[Cur_Mesh]->Init(	( (SubAxis_Tiles*SubAxis_Tiles)*20) ,		//verts
										( (SubAxis_Tiles*SubAxis_Tiles)*10) );	//faces

			pVM = pMeshes[Cur_Mesh]->pVertices;

			//Lock the vertex and index buffers
			hr = pMeshes[Cur_Mesh]->pVertexBuffer->Lock( 0,0, (void**)&p_v, D3DLOCK_DISCARD ); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );
	
			for( k=0; k<SubAxis_Tiles; ++k )
			{
				for( l=0; l<SubAxis_Tiles; ++l )
				{
					cW = (float)v_pw + (Tile_Width*k);
					cH = (float)v_ph + (Tile_Width*l);

					//Create the vertices and copy them to the mesh

					/* The 4 Tile vertices */
					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing

					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing

					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing

					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing

					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;


					/* The 4 Base Faces (top, bottom) */
					//--1
					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = (float)(-Base_Height); 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = (float)(-Base_Height); 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					//--2
					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = (float)(-Base_Height); 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = (float)(-Base_Height); 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					//--3
					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW + Tile_Width;
					pVM[Cur_Vert].y = (float)(-Base_Height); 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = (float)(-Base_Height); 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					//--4
					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = 0.0f; 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 0.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = (float)(-Base_Height); 
					pVM[Cur_Vert].z = cH + Tile_Width;
						pVM[Cur_Vert].u = 1.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					pVM[Cur_Vert].x = cW;
					pVM[Cur_Vert].y = (float)(-Base_Height); 
					pVM[Cur_Vert].z = cH;
						pVM[Cur_Vert].u = 0.0f;//for testing
						pVM[Cur_Vert].v = 1.0f;//for testing
					*p_v=pVM[Cur_Vert]; p_v++; Cur_Vert++;

					
					//if(Cur_Vert>=pMeshes[Cur_Mesh]->Num_Vertices) Fatal_Error( "Buf Overflow in mesh make vertices function" );

				}
			}
			hr = pMeshes[Cur_Mesh]->pVertexBuffer->Unlock(); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );


			//Set the attribute pointer
			p_a = pMeshes[Cur_Mesh]->pAttributes; 

			/* Set up Indices and Tiles */
			hr = pMeshes[Cur_Mesh]->pIndexBuffer->Lock( 0, 0, (void**)&p_i, D3DLOCK_DISCARD ); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );
			for(k=0;k<SubAxis_Tiles;++k)
			{
				for(l=0;l<SubAxis_Tiles;++l)
				{
					
					//Create the tile
					pTiles[Cur_Tile].Gen_20( Cur_Tile, Cur_Index );
					
					if(Cur_Tile>=Num_Tiles) Fatal_Error( " Buffer Overflow in Generate fn(). " );

					//Copy the Indices to the mesh
					for( m=0; m<2; ++m )
					{
					*p_i = pTiles[Cur_Tile].TopPolys[m].p1; p_i++; 
					*p_i = pTiles[Cur_Tile].TopPolys[m].p2; p_i++;   
					*p_i = pTiles[Cur_Tile].TopPolys[m].p3; p_i++; 
					*(p_a+Attrib_Ind) = pTiles[Cur_Tile].TopPolys[m].dwAttribGroup; Attrib_Ind++;
					}

					for( m=0; m<8; ++m )
					{
						*p_i = pTiles[Cur_Tile].BasePolys[m].p1; p_i++;  
						*p_i = pTiles[Cur_Tile].BasePolys[m].p2; p_i++;  
						*p_i = pTiles[Cur_Tile].BasePolys[m].p3; p_i++; 
						*(p_a+Attrib_Ind)  = pTiles[Cur_Tile].BasePolys[m].dwAttribGroup;
						if(Attrib_Ind>=pMeshes[Cur_Mesh]->nAttributes) Fatal_Error( "Attribute buffer Overflow!");
						Attrib_Ind++;
					}

					Cur_Tile++;
				}
			}
			hr = pMeshes[Cur_Mesh]->pIndexBuffer->Unlock(); if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );


			pMeshes[Cur_Mesh]->pFrustum = pFR;

			//Make the attributes
			pMeshes[Cur_Mesh]->CreateAttributes(); // - ha
/*
			for( int i=0 ;i< SQR(SubAxis_Tiles)-1; ++i )
			{
				CheckBasesVisible( pMeshes[Cur_Mesh], i*20 );
			}*/

			//Reset
			p_i = 0;
			p_v = 0;
			p_a = 0;
			Attrib_Ind = 0;
			Cur_Mesh++;
			Cur_Vert = 0; 
			Cur_Index = 0;

			v_pw+=(SubAxis_Tiles*Tile_Width); 
		}//for j<SubAxis_Tiles meshes
		v_pw=0.0;
		v_ph+=(16*Tile_Width);
	}//for i<SubAxis_Tiles meshes

}
/*-------------------------------------------
	Check_MeshTilesVisible()
		Checks if the mesh's tiles
		are visible and adds them to the render
		buffer.
-------------------------------------------*/
void Tile_Controller_Class::Check_MeshTilesVisible( Mesh_Class* pm )
{
/*	int Tiles = pm->Num_Vertices/20;
	for( UINT i=0; i<Tiles; ++i )
	{
		
		if( pFR->_Point( pm->pVertices[(Tiles*20)+1] ) ) pm->AddTilePolys();
		else if( pFR->_Point( pm->pVertices[(Tiles*20)+1] ) );
	}*/

}
void Tile_Controller_Class::MeshVisibility()
{
	/*int i;
	for( i=0; i<Num_Meshes; ++i )
	{
		//first see if the whole mesh is visible
		pMeshes[i]->Vis_Quick();

		if( pMeshes[i]->bCanDraw )
			Check_MeshTilesVisible( pMeshes[i] );
	}*/

}
/*-------------------------------------------
			Render the mesh.
-------------------------------------------*/
void Tile_Controller_Class::Render( )
{

/**/

	UINT i;
	//Clear the world matrxix.
	D3DXMATRIXA16 World;
	D3DXMatrixIdentity(&World);
	g_D3D_Struct->g_pD3DDevice->SetTransform( D3DTS_WORLD, &World );

	for( i=0; i<Num_Meshes; ++i )
	{
		//first see if the whole mesh is visible
		pMeshes[i]->Vis_Quick();

		if( pMeshes[i]->bCanDraw )
		{
			//per-tile determination
			//Eight dot products vs. Ten polygons ? don't know 
			//check for individual polys outside?
			pMeshes[i]->Render( TEXID_TILE_GRASS );
			pMeshes[i]->Render( TEXID_BASE_ROCK );
		}

	}
/**/
}
/*-------------------------------------------
			Pick Tile()
		Takes the cursor position as input and 
		returns: 
		 - An offset(in vertexes) into the returned "Mesh_Class".
		 - The mesh that contains the tile.
		 - The tile number ( 0 or 1 ).
-------------------------------------------*/
void Tile_Controller_Class::PickTile( POINT* pCursorPos, Hit_Struct* &pHitStruct )
{

	Vertex*			pVerts;
	DWORD			i,j,vOffset;
	D3DXMATRIX		proj, invView, matView;
	D3DXVECTOR3		cv;
	D3DXVECTOR3		rayOrigin, rayDir, tmp[2][3];
	float			U, V, UV, dist, Mdist = 100000.0f;		//dist=temp Mdist= min distance(closest triangle)
	bool			Tri0=1;		// - which triangle was hit 0 or 1

	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_PROJECTION, &proj );

	cv.x = ( ( ( 2.0f * pCursorPos->x ) / g_D3D_Struct->ScreenWidth ) - 1 ) / proj._11;
	cv.y = -( ( ( 2.0f * pCursorPos->y ) / g_D3D_Struct->ScreenHeight ) - 1 ) / proj._22;
	cv.z = 1.0f;

	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_VIEW, &matView);

	D3DXMatrixInverse( &invView, NULL, &matView );
/* 
	11 21 31 41
	12 22 32 42
	13 23 33 43 
	14 24 34 44
*/
	// Transform the screen space pick ray into 3D space
	rayDir.x = cv.x*invView._11 + cv.y*invView._21 + cv.z*invView._31;
	rayDir.y = cv.x*invView._12 + cv.y*invView._22 + cv.z*invView._32;
	rayDir.z = cv.x*invView._13 + cv.y*invView._23 + cv.z*invView._33;
	rayOrigin.x = invView._41;
	rayOrigin.y = invView._42;
	rayOrigin.z = invView._43;

	//loop through visible meshes
	for( i=0; i<Num_Meshes; ++i )
	{
		if( pMeshes[i] )
		{
			if( pMeshes[i]->bCanDraw )
			{		
				// - Reset
				pVerts = pMeshes[i]->pVertices;
				vOffset= 0;	
				for( j=0; j<pMeshes[i]->Num_Vertices; j+=20 )
				{
					// Obtain Triangle 1
					tmp[0][0].x = (pVerts  )->x; tmp[0][0].y = (pVerts  )->y; tmp[0][0].z = (pVerts  )->z; 
					tmp[0][1].x = (pVerts+2)->x; tmp[0][1].y = (pVerts+2)->y; tmp[0][1].z = (pVerts+2)->z; 
					tmp[0][2].x = (pVerts+3)->x; tmp[0][2].y = (pVerts+3)->y; tmp[0][2].z = (pVerts+3)->z; 

					//Obtain triangle 2
					tmp[1][0].x = (pVerts  )->x; tmp[1][0].y = (pVerts  )->y; tmp[1][0].z = (pVerts  )->z; 
					tmp[1][1].x = (pVerts+1)->x; tmp[1][1].y = (pVerts+1)->y; tmp[1][1].z = (pVerts+1)->z; 
					tmp[1][2].x = (pVerts+2)->x; tmp[1][2].y = (pVerts+2)->y; tmp[1][2].z = (pVerts+2)->z; 
					
					//Check for intersections
					if( D3DXIntersectTri( &tmp[0][0], &tmp[0][1], &tmp[0][2], &rayOrigin, &rayDir, &U, &V, &dist ) )
					{
						UV = 1-(U+V);	//calculate the third weight value
						if( dist < Mdist )//Make sure that it isn't selecting void space which would return #NAN, #INF, or something very large.  This also tells if the tile in question is closer than any other tile that was hit.
						{
							//ID = pTiles->pTiles[i]->ID; 
							//MU = U; MV = V; MUV = UV; 
							Mdist = dist;			// - Set Mdist to dist, so only the closest tile to the screen will be selected.			
							pHitStruct->Offset = vOffset;		// - Set the returned offset.
							pHitStruct->pMesh = pMeshes[i];
							pHitStruct->Tri = 1;				// - This finally tells which triangle was hit.
							pHitStruct->U = U;
							pHitStruct->V = V;
							pHitStruct->UV = UV;
						}
					}

					if( D3DXIntersectTri( &tmp[1][0], &tmp[1][1], &tmp[1][2], &rayOrigin, &rayDir, &U, &V, &dist ) )
					{
						UV = 1-(U+V);
						if( dist < Mdist )
						{
							//ID = pTiles->pTiles[i]->ID; 
							//MU = U; MV = V; MUV = UV;  
							Mdist = dist; 
							pHitStruct->Offset = vOffset;
							pHitStruct->pMesh = pMeshes[i];
							pHitStruct->Tri = 0;
							pHitStruct->U = U;
							pHitStruct->V = V;
							pHitStruct->UV = UV;
						}
					}

					//Incrament by 20, which will go to the next tile in the buffer
					pVerts+=20;		
					vOffset+=20;
				}
		
			}
		}
	}
}

/*-------------------------------------------
			Free Resources
-------------------------------------------*/
void Tile_Controller_Class::Free()
{
	if(pMeshes)
	{
		for(UINT i=0; i<Num_Meshes; ++i)
		{
			if( pMeshes[i] )
				pMeshes[i]->Free();
		}

		delete [] pMeshes;
	}
	if(pTiles)
	{
		delete [] pTiles;
	}
}	