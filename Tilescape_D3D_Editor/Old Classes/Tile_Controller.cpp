#include "Tile_Controller.h"
#include "Tile_Struct.h"
#include "Base_Struct.h"

/*

	Tile Controller functions

----------------------------*/
/*
	PickTile:
	w,h : screen width and height
	returns:
	The return contents are placed into the PickedTile and PickedVert variables
	LOWORD: number of vertex( Top_Border_Verts)selected(0 to 3) or -1 for none
	HIWORD: tile number selected -1 for none
-----------------------------------------------------------------------------*/
DWORD Tile_Control_Struct::PickTile(int w, int h)//Seems to work, but new code can be sneaky...
{
	D3DMATRIX proj;
	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_PROJECTION, &proj );

	//Code below generously donated from toymaker.info
	///----
	POINT mouse;
	GetCursorPos(&mouse);
	D3DXVECTOR3 v;
	v.x = ( ( ( 2.0f * mouse.x ) / w ) - 1 ) / proj._11;
	v.y = -( ( ( 2.0f * mouse.y ) / h ) - 1 ) / proj._22;
	v.z = 1.0f;

	D3DXMATRIX m, matView;
	D3DXVECTOR3 rayOrigin,rayDir;
	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_VIEW, &matView);
	D3DXMatrixInverse( &m, NULL, &matView );

	// Transform the screen space pick ray into 3D space
	rayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	rayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	rayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
	rayOrigin.x = m._41;
	rayOrigin.y = m._42;
	rayOrigin.z = m._43;
	////----
	//Loop through tiles on screen
	D3DXVECTOR3 tmp[2][3];
	float U,V,UV,dist, Mdist=100000.0f,MU,MV,MUV;//dist=temp Mdist= min distance(closest triangle)
	int ID=-1;	//ID of the tile (return)
	bool Tri0=1;//which triangle was hit 0 or 1
	//--------------------------New - instead, loop through all tiles, get the one with the least distance.
	for( int i=0; i<pTiles->Num; ++i )
	{
		for( int p=0; p<2; ++p)
		{
				tmp[p][0].x = pTiles->pTiles[i]->pPolys[p]->p1.x;//Top_Border_Verts[s].x;
				tmp[p][0].y = pTiles->pTiles[i]->pPolys[p]->p1.y;//Top_Border_Verts[s].y;
				tmp[p][0].z = pTiles->pTiles[i]->pPolys[p]->p1.z;//Top_Border_Verts[s].z;
				tmp[p][1].x = pTiles->pTiles[i]->pPolys[p]->p2.x;//Top_Border_Verts[s].x;
				tmp[p][1].y = pTiles->pTiles[i]->pPolys[p]->p2.y;//Top_Border_Verts[s].y;
				tmp[p][1].z = pTiles->pTiles[i]->pPolys[p]->p2.z;//Top_Border_Verts[s].z;
				tmp[p][2].x = pTiles->pTiles[i]->pPolys[p]->p3.x;//Top_Border_Verts[s].x;
				tmp[p][2].y = pTiles->pTiles[i]->pPolys[p]->p3.y;//Top_Border_Verts[s].y;
				tmp[p][2].z = pTiles->pTiles[i]->pPolys[p]->p3.z;//Top_Border_Verts[s].z;
		}
		if( D3DXIntersectTri( &tmp[0][0], &tmp[0][1], &tmp[0][2], &rayOrigin, &rayDir, &U, &V, &dist ) )
		{
			UV = 1-(U+V);
			if( dist < Mdist )
			{
				ID = pTiles->pTiles[i]->ID; 
				MU = U; MV = V; MUV = UV; 
				Mdist = dist;
				Tri0=1;
			}
		}
		if( D3DXIntersectTri( &tmp[1][0], &tmp[1][1], &tmp[1][2], &rayOrigin, &rayDir, &U, &V, &dist ) )
		{
			UV = 1-(U+V);
			if( dist < Mdist )
			{
				ID = pTiles->pTiles[i]->ID; 
				MU = U; MV = V; MUV = UV;  
				Mdist = dist; 
				Tri0=0; 
			}
		}
	}
	if(!(ID==-1))//a tile was hit
	{	
		if(Tri0)//it's tri[0]
		{
			if(MUV>0.7f){ PickedTile=ID; PickedVert=0; return MAKELONG(0,ID); }//MAKEWORD(a,b); //user can move vertex 0
			else if(MU>0.7f){ PickedTile=ID; PickedVert=1; return MAKELONG(1,ID); }//user can move vertex 1
			else if(MV>0.7f){ PickedTile=ID; PickedVert=3; return MAKELONG(3,ID); }//user can move vertex 3
			else{ PickedTile=ID; PickedVert=-1; return MAKELONG(-1,ID); }//user can move vertex 3
		}
		else//it's tri[1]
		{
			//UV = 1-(U+V);//////////////------------------------------PERFECT VVVVVVVV
			//Triangle 2 was hit!
			if(MUV>0.7f){ PickedTile=ID; PickedVert=2; return MAKELONG(2,ID); }//user can move vertex 2 -- SEEMS TO WORK
			else if(MU>0.7f){ PickedTile=ID; PickedVert=3; return MAKELONG(3,ID); }//user can move vertex 3
			else if(MV>0.7f){ PickedTile=ID; PickedVert=1; return MAKELONG(1,ID); }//user can move vertex 1
			else{ PickedTile=ID; PickedVert=-1; return MAKELONG(-1,ID); }//user can move vertex 3;
		}
		
	}
	return 0;
}

/*
	Procedural Land:
	-Creates a flat
	tiled landscape
---------------------------*/
void Tile_Control_Struct::Procedural_Land(int num_tiles)
{
	Base_Height = 1.0f;
	Tile_Size = 3.0f;
	Axis_Tiles = num_tiles;
	World_Size = Axis_Tiles * Tile_Size;
	Num_Tiles = (int)(Axis_Tiles * Axis_Tiles);
	HeightUpperLimit=90.0f;			//Set the height limits for the land
	HeightLowerLimit=0.0f;

	Tiles = new Tile_Struct[Num_Tiles];

	TileBuffer = new Buffer_Class();
	BaseBuffer = new Buffer_Class();
	TileBuffer->Init( Num_Tiles*2, TILETEX_GRASS, 0 );
	BaseBuffer->Init( Num_Tiles*8, BASETEX_ROCK, D3DUSAGE_DYNAMIC );
	BaseBuffer->Num_Polygons=BaseBuffer->Num_Verts=0;
	//BaseBuffer->D3DTexture.Load( "Texture\\128x32_Rock.bmp", g_D3D_Struct->g_pD3DDevice );

	int Cur_Poly=0;	//Current Polygon
	int Cur_Tile=0;	//temp current tile
	int tID=0;
	D3DXVECTOR3 norm;//outputted normal

	//Create a default flat landscape
	for( int i=(int)-(Axis_Tiles/2); i<(int)(Axis_Tiles/2); ++i)//x axis
	{
		for( int j=(int)-(Axis_Tiles/2); j<(int)(Axis_Tiles/2); ++j)//z axis
		{
			//Assign polygons in the buffer
			//then make pointers to them, contained in the tile structure
			//Assume this uses the first tile top texture
			TileBuffer->Polygons[Cur_Poly].p1.x = (float)i*Tile_Size;
			TileBuffer->Polygons[Cur_Poly].p1.y = 0.0f;
			TileBuffer->Polygons[Cur_Poly].p1.z = (float)(j*Tile_Size);
			TileBuffer->Polygons[Cur_Poly].p1.u = 0.0f;
			TileBuffer->Polygons[Cur_Poly].p1.v = 0.0f;

			TileBuffer->Polygons[Cur_Poly].p2.x = (float)(i*Tile_Size)+Tile_Size;
			TileBuffer->Polygons[Cur_Poly].p2.y = 0.0f;
			TileBuffer->Polygons[Cur_Poly].p2.z = (float)(j*Tile_Size);
			TileBuffer->Polygons[Cur_Poly].p2.u = 0.49f;
			TileBuffer->Polygons[Cur_Poly].p2.v = 0.0f;

			TileBuffer->Polygons[Cur_Poly].p3.x = (float)i*Tile_Size;
			TileBuffer->Polygons[Cur_Poly].p3.y = 0.0f;
			TileBuffer->Polygons[Cur_Poly].p3.z = (float)(j*Tile_Size)+Tile_Size;
			TileBuffer->Polygons[Cur_Poly].p3.u = 0.0f;
			TileBuffer->Polygons[Cur_Poly].p3.v = 0.49f;

			//Second polygon
			TileBuffer->Polygons[Cur_Poly+1].p1.x = (float)(i*Tile_Size)+Tile_Size;
			TileBuffer->Polygons[Cur_Poly+1].p1.y = 0.0f;
			TileBuffer->Polygons[Cur_Poly+1].p1.z = (float)j*Tile_Size+Tile_Size;
			TileBuffer->Polygons[Cur_Poly+1].p1.u = 0.49f;
			TileBuffer->Polygons[Cur_Poly+1].p1.v = 0.49f;

			TileBuffer->Polygons[Cur_Poly+1].p2 = TileBuffer->Polygons[Cur_Poly].p3;

			TileBuffer->Polygons[Cur_Poly+1].p3 = TileBuffer->Polygons[Cur_Poly].p2;

			//UVW coords

			/* Tile Info */

			Tiles[Cur_Tile].pPolys[0] = &TileBuffer->Polygons[Cur_Poly];
			Tiles[Cur_Tile].pPolys[1] = &TileBuffer->Polygons[Cur_Poly+1];

			Tiles[Cur_Tile].pTileController = this;
			Tiles[Cur_Tile].ID = tID;
			tID++;

			Tiles[Cur_Tile].Top_Border_Verts[0] = &TileBuffer->Polygons[Cur_Poly].p1;//Note: 3,2,1,0 rewound the verts opposite
			Tiles[Cur_Tile].Top_Border_Verts[1] = &TileBuffer->Polygons[Cur_Poly].p2;
			Tiles[Cur_Tile].Top_Border_Verts[2] = &TileBuffer->Polygons[Cur_Poly+1].p1;
			Tiles[Cur_Tile].Top_Border_Verts[3] = &TileBuffer->Polygons[Cur_Poly].p3;

			for( int i=0; i<4;++i)
			Tiles[Cur_Tile].Vert_Height[i]=0.0f;

			Cur_Tile++;
			Cur_Poly+=2;
		}
	}

	TileBuffer->FillBuffer();
	pTiles = new Tile_Buffer_Class();
	pTiles->Init(this);


	//Loop again, this time setting the Shared Verts
	//--- Any post tile-creation processes should be here ---
	int tm=0, tp=0,m=0, p=0;
	Vertex* tv = new Vertex();
	tv->x = tv->y = tv->z = tv->u = tv->v = -100000.0f;//Failsafe vertex
	for( int i=0; i<(int)(Num_Tiles); ++i )
	{
		Tiles[i].Init();
		m = i-1;
		p = i+1;
		tm = i-Axis_Tiles;
		tp = i+Axis_Tiles;

		if( m >= 0 )
		{			
			Tiles[i].pSharedVerts[6] = Tiles[m].Top_Border_Verts[2]; 
			Tiles[i].pSharedVerts[7] = Tiles[m].Top_Border_Verts[1]; 
			Tiles[i].pBorderTiles[3] = &Tiles[m];
		}
		else
		{
			Tiles[i].pSharedVerts[6] = tv;
			Tiles[i].pSharedVerts[7] = tv;
			Tiles[i].pBorderTiles[3] = 0;
		}

		if( p < Num_Tiles )
		{
			Tiles[i].pSharedVerts[3] = Tiles[p].Top_Border_Verts[3]; 
			Tiles[i].pSharedVerts[2] = Tiles[p].Top_Border_Verts[0];
			Tiles[i].pBorderTiles[1] = &Tiles[p];
		}
		else
		{
			Tiles[i].pSharedVerts[3] = tv;
			Tiles[i].pSharedVerts[2] = tv; 
			Tiles[i].pBorderTiles[1] = 0;
		}

		if( tp < Num_Tiles )
		{
			Tiles[i].pSharedVerts[0] = Tiles[tp].Top_Border_Verts[3];
			Tiles[i].pSharedVerts[1] = Tiles[tp].Top_Border_Verts[2];
			Tiles[i].pBorderTiles[0] = &Tiles[tp];
		}
		else
		{
			Tiles[i].pSharedVerts[0] = tv;
			Tiles[i].pSharedVerts[1] = tv;
			Tiles[i].pBorderTiles[0] = 0;
		}

		if( tm >=0 )
		{
			Tiles[i].pSharedVerts[4] = Tiles[tm].Top_Border_Verts[1]; 
			Tiles[i].pSharedVerts[5] = Tiles[tm].Top_Border_Verts[0];
			Tiles[i].pBorderTiles[2] = &Tiles[tm];
		}
		else
		{
			Tiles[i].pSharedVerts[4] = tv;
			Tiles[i].pSharedVerts[5] = tv;
			Tiles[i].pBorderTiles[2] = 0;
		}
	}


}

/*
	Changes the texture of all the tiles
*/
void Tile_Control_Struct::Change_All_Tiles(int TexInd)			//change all tile textures
{
	for( int i=0; i<Num_Tiles; ++i )
	{
		Tiles[i].Change_Tex(TexInd);
	}
	this->TileBuffer->FillBuffer();
}

/*
	Changes the base textures

--------------------------------------------*/
void Tile_Control_Struct::Change_All_Bases(int TexInd)			//change all tile textures
{
/*
	for( int i=0; i<Num_Tiles; ++i )
	{
		if(Tiles[i].pBase!=0)
		Tiles[i].pBase->Change_Tex(TexInd);
	}
	this->TileBuffer->FillBuffer();
	*/
	;
}
/*
	Raises a certain number of tiles
	Radius: number to raise from the center
	SelID: the id of the center (selected) tile
*/
void Tile_Control_Struct::Raise_Batch_Tiles(int Radius, int SelID)
{
	bool RaiseLowestFirst=true;//if the lowest verts should be raised first

	if(Radius > Axis_Tiles) return;

	int st	= SelID-((Axis_Tiles*Radius) + Axis_Tiles-Radius);	//start tile id
	int Row = (Radius*2)+1	;	//# tiles in a row
	int N	= Row*Row;			//Total tiles to raise
  	int Ind=st;					//The current Tile ID

	if(RaiseLowestFirst)
	{
		int*	Lows = new int[N*4];
		memset(Lows,0,sizeof(int)*N*4);
		int		LowInd=0;
		float   tHeight = 100000.0f;

		for( int i=0; i<N; ++i )
		{
			if(i%Row==0)
			{
				Ind+=(Axis_Tiles-Row+1); //go to the next row
			}
			else Ind++;

			if( (Ind>0) && 
				(Ind<(Num_Tiles+1)) ){
				if(&Tiles[Ind]!=0) 
				{		
					if((int)Tiles[Ind].Get_Lowest_Height()<(int)tHeight)//This is lower than any, reset the buffer
					{
						LowInd=0;
						Lows[LowInd] = Tiles[Ind].ID;
						tHeight = Tiles[Ind].Get_Lowest_Height();
						LowInd++;
					}
					else if((int)Tiles[Ind].Get_Lowest_Height()==(int)tHeight)
					{
						Lows[LowInd] = Tiles[Ind].ID;
						LowInd++;
					}
				}
			}
					
		}//End of the first for

		for( int i=0; i<LowInd; ++i )
		{
   			Tiles[Lows[i]].Raise_Vertex(0,0,1);
		}


		delete Lows;

	}
	else
	{
		for( int i=0; i<N; ++i )
		{
			if(i%Row==0)
			{
				Ind+=(Axis_Tiles-Row+1); //go to the next row
			}
			else Ind++;

			if( (Ind>0) && 
				(Ind<(Num_Tiles+1)) )
				if(&Tiles[Ind]!=0) 
					Tiles[Ind].Raise_Vertex(65535,0,0);
		}
	}
}
/*
	Lowers a certain number of tiles
	Radius: number to raise from the center
	SelID: the id of the center (selected) tile
*/
void Tile_Control_Struct::Lower_Batch_Tiles(int Radius, int SelID)
{
	bool LowerHighestFirst=true;//if the lowest verts should be raised first

	if(Radius > Axis_Tiles) return;

	int st	= SelID-((Axis_Tiles*Radius)+Axis_Tiles-Radius);	//start tile id
	int Row = (Radius*2)+1	;	//# tiles in a row
	int N	= Row*Row;			//Total tiles to raise
  	int Ind=st;					//The current Tile ID

	if(LowerHighestFirst)
	{
		int*	Highs = new int[N*4];
		memset(Highs,0,sizeof(int)*N*4);
		int		HighInd=0;
		float   tHeight = -1000.0f;

		for( int i=0; i<N; ++i )
		{
			if(i%Row==0)
			{
				Ind+=(Axis_Tiles-Row+1); //go to the next row
			}
			else Ind++;

			if( (Ind>0) && 
				(Ind<(Num_Tiles+1)) ){
				if(&Tiles[Ind]!=0) 
				{		
					if((int)Tiles[Ind].Get_Highest_Height()>(int)tHeight)//This is lower than any, reset the buffer
					{
						HighInd=0;
						Highs[HighInd] = Tiles[Ind].ID;
						tHeight = Tiles[Ind].Get_Highest_Height();
						HighInd++;
					}
					else if((int)Tiles[Ind].Get_Highest_Height()==(int)tHeight)
					{
						Highs[HighInd] = Tiles[Ind].ID;
						HighInd++;
					}
				}
			}
					
		}//End of the first for

		for( int i=0; i<HighInd; ++i )
		{
   			Tiles[Highs[i]].Lower_Vertex(0,0,2);
		}


		delete Highs;

	}
	else
	{
		for( int i=0; i<N; ++i )
		{
			if(i%Row==0)
			{
				Ind+=(Axis_Tiles-Row+1); //go to the next row
			}
			else Ind++;

			if( (Ind>0) && 
				(Ind<(Num_Tiles+1)) )
				if(&Tiles[Ind]!=0) 
					Tiles[Ind].Raise_Vertex(65535,0,0);
		}
	}
}
/*
	Raise tiles in a hill fashion
	Radius:		//not needed??!
	Height:		Steepness of the slope.
	SelID:		The Tile.ID of the selected tile (the top of the hill)
*/
void Tile_Control_Struct::Raise_Hill_Tiles(int Radius, int Height, int SelID)
{
	//if center is not coplanar make it coplanar
	//Raise the center - no matter what
	//
	
	float Highest=0.0f;
	Tiles[SelID].Raise_Vertex(0,0,-1); //raise the center tile 
	for( int i=0; i<4; ++i )
	{
		if(Tiles[SelID].Top_Border_Verts[i]->y>Highest) Highest = Tiles[SelID].Top_Border_Verts[i]->y;
	}
	//Only raise vertices lower than the parent vertice
	int Level = 1; //the level being raised
	int CornerID = SelID - (Axis_Tiles*Level) - Level;//Id of the tile to the corner of the main
	int Width = Level*2; //+1 but we are going to start changing at level+1
	bool Defect = 0; //if there is a defect in the current level, finish the level and exit this function else go to the next level
	float Level_Height = Highest; //height of the current level



	int i=0;

	//Traverse each side:: +1 then +Axis_Tiles then -1 then -Axis_Tiles
	while(!Defect)
	{
		int tmpID=CornerID+1;
		for( i=0; i<Width; ++i )//top side 0,1 - switched 1,2
		{
			if((tmpID>=0)&&(tmpID<Num_Tiles))
			{
				if(!(i==Width-1))//side tile
				{
						if(Tiles[tmpID].Top_Border_Verts[1]->y< Level_Height)
						{
							Tiles[tmpID].Raise_Vertex(1,0,0);
							Defect=1;
						}
						if(Tiles[tmpID].Top_Border_Verts[2]->y<Level_Height)
						{
							Tiles[tmpID].Raise_Vertex(2,0,0);
							Defect=1;
						}
				}
				else //Corner tile
				{
						if(Tiles[tmpID].Top_Border_Verts[1]->y< Level_Height)
						{
							Tiles[tmpID].Raise_Vertex(1,0,0);
							Defect=1;
						}
				}
			}
			tmpID++;
		}
		tmpID--;
		tmpID+=Axis_Tiles;
		for( i=0; i<Width; ++i )//right side 3,0 - switched 0,1
		{
			if((tmpID>=0)&&(tmpID<Num_Tiles))
			{
				if(!(i==Width-1))//side tile
				{
   					if(Tiles[tmpID].Top_Border_Verts[0]->y< Level_Height)
					{
						Tiles[tmpID].Raise_Vertex(0,0,0);
						Defect=1;
					}
					if(Tiles[tmpID].Top_Border_Verts[1]->y<Level_Height)
					{
						Tiles[tmpID].Raise_Vertex(1,0,0);
						Defect=1;
					}
				}
				else //Corner tile
				{
					if(Tiles[tmpID].Top_Border_Verts[0]->y< Level_Height)
					{
						Tiles[tmpID].Raise_Vertex(0,0,0);
						Defect=1;
					}
				}
			}
			tmpID+=Axis_Tiles;
		}
		tmpID-=Axis_Tiles;
		tmpID--;
		for( i=0; i<Width; ++i )//bottom side 2,3 - switched 3,0
		{
			if((tmpID>=0)&&(tmpID<Num_Tiles))
			{
				if(!(i==Width-1))//side tile
				{
   					if(Tiles[tmpID].Top_Border_Verts[3]->y< Level_Height)
					{
						Tiles[tmpID].Raise_Vertex(3,0,0);
						Defect=1;
					}
					if(Tiles[tmpID].Top_Border_Verts[0]->y<Level_Height)
					{
						Tiles[tmpID].Raise_Vertex(0,0,0);
						Defect=1;
					}
				}
				else //Corner tile
				{
					if(Tiles[tmpID].Top_Border_Verts[3]->y< Level_Height)
					{
						Tiles[tmpID].Raise_Vertex(3,0,0);
						Defect=1;
					}
				}
			}
			tmpID--;
		}
		tmpID++;
		tmpID-=Axis_Tiles;
		for( i=0; i<Width; ++i )//left side 1,2 - changed 2,3
		{
			if((tmpID>=0)&&(tmpID<Num_Tiles))
			{
				if(!(i==Width-1))//side tile
				{
   					if(Tiles[tmpID].Top_Border_Verts[2]->y< Level_Height)
					{
						Tiles[tmpID].Raise_Vertex(2,0,0);
						Defect=1;
					}
					if(Tiles[tmpID].Top_Border_Verts[3]->y<Level_Height)
					{
						Tiles[tmpID].Raise_Vertex(3,0,0);
						Defect=1;
					}
				}
				else //Corner tile
				{
					if(Tiles[tmpID].Top_Border_Verts[2]->y< Level_Height)
					{
						Tiles[tmpID].Raise_Vertex(2,0,0);
						Defect=1;
					}
				}
			}
			tmpID-=Axis_Tiles;
		}

		Level++; //next level
		CornerID = SelID - (Axis_Tiles*Level) - Level;//reset the corner
		Width = Level*2;
	}
}

/*
	Lower tiles in a hill fashion
	Radius:		//not needed??!
	Height:		Steepness of the slope.
	SelID:		The Tile.ID of the selected tile (the top of the hill)
*/
void Tile_Control_Struct::Lower_Hill_Tiles(int Radius, int Height, int SelID)
{
	//if center is not coplanar make it coplanar
	//Raise the center - no matter what
	//
	
	float Lowest=-1000.0f;
	Tiles[SelID].Lower_Vertex(0,0,-1); //raise the center tile 
	for( int i=0; i<4; ++i )
	{
		if(Tiles[SelID].Top_Border_Verts[i]->y < Lowest) Lowest = Tiles[SelID].Top_Border_Verts[i]->y;
	}
	//Only raise vertices lower than the parent vertice
	int Level = 1; //the level being raised
	int CornerID = SelID - (Axis_Tiles*Level) - Level;//Id of the tile to the corner of the main
	int Width = Level*2; //+1 but we are going to start changing at level+1
	bool Defect = 0; //if there is a defect in the current level, finish the level and exit this function else go to the next level
	float Level_Height = Lowest; //height of the current level



	int i=0;

	//Traverse each side:: +1 then +Axis_Tiles then -1 then -Axis_Tiles
	while(!Defect)
	{
		int tmpID=CornerID+1;
		for( i=0; i<Width; ++i )//top side 0,1 - switched 1,2
		{
			if((tmpID>=0)&&(tmpID<Num_Tiles))
			{
				if(!(i==Width-1))//side tile
				{
						if(Tiles[tmpID].Top_Border_Verts[1]->y > Level_Height)
						{
							Tiles[tmpID].Lower_Vertex(1,0,0);
							Defect=1;
						}
						if(Tiles[tmpID].Top_Border_Verts[2]->y > Level_Height)
						{
							Tiles[tmpID].Lower_Vertex(2,0,0);
							Defect=1;
						}
				}
				else //Corner tile
				{
						if(Tiles[tmpID].Top_Border_Verts[1]->y > Level_Height)
						{
							Tiles[tmpID].Lower_Vertex(1,0,0);
							Defect=1;
						}
				}
			}
			tmpID++;
		}
		tmpID--;
		tmpID+=Axis_Tiles;
		for( i=0; i<Width; ++i )//right side 3,0 - switched 0,1
		{
			if((tmpID>=0)&&(tmpID<Num_Tiles))
			{
				if(!(i==Width-1))//side tile
				{
   					if(Tiles[tmpID].Top_Border_Verts[0]->y > Level_Height)
					{
						Tiles[tmpID].Lower_Vertex(0,0,0);
						Defect=1;
					}
					if(Tiles[tmpID].Top_Border_Verts[1]->y > Level_Height)
					{
						Tiles[tmpID].Lower_Vertex(1,0,0);
						Defect=1;
					}
				}
				else //Corner tile
				{
					if(Tiles[tmpID].Top_Border_Verts[0]->y > Level_Height)
					{
						Tiles[tmpID].Lower_Vertex(0,0,0);
						Defect=1;
					}
				}
			}
			tmpID+=Axis_Tiles;
		}
		tmpID-=Axis_Tiles;
		tmpID--;
		for( i=0; i<Width; ++i )//bottom side 2,3 - switched 3,0
		{
			if((tmpID>=0)&&(tmpID<Num_Tiles))
			{
				if(!(i==Width-1))//side tile
				{
   					if(Tiles[tmpID].Top_Border_Verts[3]->y > Level_Height)
					{
						Tiles[tmpID].Lower_Vertex(3,0,0);
						Defect=1;
					}
					if(Tiles[tmpID].Top_Border_Verts[0]->y > Level_Height)
					{
						Tiles[tmpID].Lower_Vertex(0,0,0);
						Defect=1;
					}
				}
				else //Corner tile
				{
					if(Tiles[tmpID].Top_Border_Verts[3]->y > Level_Height)
					{
						Tiles[tmpID].Lower_Vertex(3,0,0);
						Defect=1;
					}
				}
			}
			tmpID--;
		}
		tmpID++;
		tmpID-=Axis_Tiles;
		for( i=0; i<Width; ++i )//left side 1,2 - changed 2,3
		{
			if((tmpID>=0)&&(tmpID<Num_Tiles))
			{
				if(!(i==Width-1))//side tile
				{
   					if(Tiles[tmpID].Top_Border_Verts[2]->y > Level_Height)
					{
						Tiles[tmpID].Lower_Vertex(2,0,0);
						Defect=1;
					}
					if(Tiles[tmpID].Top_Border_Verts[3]->y > Level_Height)
					{
						Tiles[tmpID].Lower_Vertex(3,0,0);
						Defect=1;
					}
				}
				else //Corner tile
				{
					if(Tiles[tmpID].Top_Border_Verts[2]->y > Level_Height)
					{
						Tiles[tmpID].Lower_Vertex(2,0,0);
						Defect=1;
					}
				}
			}
			tmpID-=Axis_Tiles;
		}

		Level++; //next level
		CornerID = SelID - (Axis_Tiles*Level) - Level;//reset the corner
		Width = Level*2;
	}
}


void Tile_Control_Struct::Raise_All_Tiles()
{
	for( int i=0; i<Num_Tiles; ++i )
	{
		Tiles[i].Raise_Vertex( 0, 0, -1 );
	}
}
void Tile_Control_Struct::Lower_All_Tiles()
{
	for( int i=0; i<Num_Tiles; ++i )
	{
		Tiles[i].Lower_Vertex( 0, 0, -1 );
	}
}