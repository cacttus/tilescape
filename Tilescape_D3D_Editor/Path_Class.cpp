#include "Path_Class.h"

//int PathTool_SelTile=0;		//global pathtool variable


/*
	Initialize the class, 
	size:			size of one side of the map.
	TexLocation:	file location of the path texture.
	tcs:			a pointer to the global tile controlller.

void Path_Class::Init( int size, char* TexLocation, Tile_Control_Struct* tcs   )
{
	Path_Grid = new char[size*size];
	memset(Path_Grid, -1, sizeof(char)*(size*size));
	Grid_Len = size*size;
	Axis_Size = size;

	pTileController = tcs;
	if(!pTileController) Fatal_Error( "Failed to instantiate the path class!!! (init function)" );

	HRESULT hr = D3DXCreateTextureFromFile( g_D3D_Struct->g_pD3DDevice, TexLocation, &pTexture );
	if(FAILED(hr)) Fatal_Error( hr, __LINE__, __FILE__ );
}

//This is called in the Set_Path function, not to be used directly.
//MainTile specifies if the tile being checked is the main one, or a border one
//MainTileInd is the index of the tile if MainTile is true, else set it to -1
void Path_Class::Check_Tile( int TileNum, int MainTileInd )
{
		int top=-1,right=-1,bottom=-1,left=-1;//instantiate to "out of bounds"

		//get the surrounding tiles

		if(TileNum-Axis_Size>=0)		if( (MainTileInd!= TileNum-Axis_Size) )	top = Path_Grid[TileNum-Axis_Size];
		if(TileNum+1<Grid_Len)			if( (MainTileInd!= TileNum+1) )			right = Path_Grid[TileNum+1];
		if(TileNum+Axis_Size<Grid_Len)	if( (MainTileInd!= TileNum+Axis_Size) )	bottom = Path_Grid[TileNum+Axis_Size];
		if(TileNum-1>=0)				if( (MainTileInd!= TileNum-1) )			left = Path_Grid[TileNum-1];

		//Determine the new path schem
		char Schem=0;		// see the notes above

		if( left!=-1   ) Schem |= 0x08;
		if( top!=-1   ) Schem |= 0x04;
		if( right!=-1 ) Schem |= 0x02;
		if( bottom!=-1 ) Schem |= 0x01;

		Path_Grid[TileNum] = Schem;

		//Set the UVW coords
		//.2500 u and .2500 v for each
		float u=0.0f, v=0.0f;

		if(	(Schem == _BOTTOM_CAP) || (Schem==_BOTTOM_LEFT) || (Schem==_TOP_RIGHT_BOTTOM) ) u=0.000f;
		else if( (Schem == _LEFT_CAP) || (Schem==_LEFT_TOP) || (Schem==_LEFT_RIGHT) || (Schem==_RIGHT_BOTTOM_LEFT) ) u = 0.2500f;
		else if( (Schem == _TOP_CAP ) || (Schem==_TOP_RIGHT) || (Schem==_TOP_BOTTOM) || (Schem==_BOTTOM_LEFT_TOP ) ) u = 0.5000f;
		else if( (Schem == _RIGHT_CAP) || (Schem==_RIGHT_BOTTOM) || (Schem==_LEFT_TOP_RIGHT) || (Schem==_ALL) ) u = 0.7500f;
		
		if( (Schem == _LEFT_CAP ) || (Schem==_TOP_CAP) || (Schem==_RIGHT_CAP) ) v=0.000;
		else if( (Schem == _BOTTOM_CAP ) || (Schem==_LEFT_TOP) || (Schem==_TOP_RIGHT) || (Schem==_RIGHT_BOTTOM) ) v=0.2500f;
		else if( (Schem == _BOTTOM_LEFT) || (Schem==_LEFT_RIGHT) || (Schem==_TOP_BOTTOM) || (Schem==_LEFT_TOP_RIGHT ) ) v=0.5000f;
		else if( (Schem == _TOP_RIGHT_BOTTOM ) || (Schem==_RIGHT_BOTTOM_LEFT ) || (Schem==_BOTTOM_LEFT_TOP ) || (Schem==_ALL ) ) v=0.7500f;
		
		pTileController->Tiles[TileNum].pPolys[0]->p1.u2 = (float)u;
		pTileController->Tiles[TileNum].pPolys[0]->p3.u2 = (float)u;
		pTileController->Tiles[TileNum].pPolys[1]->p2.u2 = (float)u;

		pTileController->Tiles[TileNum].pPolys[1]->p1.u2 = (float)(u + 0.2500);
		pTileController->Tiles[TileNum].pPolys[1]->p3.u2 = (float)(u + 0.2500);
		pTileController->Tiles[TileNum].pPolys[0]->p2.u2 = (float)(u + 0.2500);

		pTileController->Tiles[TileNum].pPolys[1]->p1.v2 = (float)v;
		pTileController->Tiles[TileNum].pPolys[1]->p2.v2 = (float)v;
		pTileController->Tiles[TileNum].pPolys[0]->p3.v2 = (float)v;

		pTileController->Tiles[TileNum].pPolys[0]->p1.v2 = (float)(v + 0.2500);
		pTileController->Tiles[TileNum].pPolys[0]->p2.v2 = (float)(v + 0.2500);
		pTileController->Tiles[TileNum].pPolys[1]->p3.v2 = (float)(v + 0.2500);

}

/*
	TileNum: the integer id of the tile as in the tile_control_struct	
	This is the actual path placement function.

void Path_Class::Set_Path( int TileNum )
{
	/* Update the input 
	if( Keydown(VK_LBUTTON) )
	{
		if(LBUTTON_DOWN==true)
			LBUTTON_HOLDING=true;
		else
			LBUTTON_DOWN = true;
	}
	else
	{
		LBUTTON_DOWN = LBUTTON_HOLDING = false;
	}

	if( (TileNum<0) || (TileNum>Grid_Len) ) return; //overflow/underflow check

	//Determine if there is already a path there
	if(Path_Grid[TileNum]!=-1) return;


	/* Initiated when the user first presses the lmb 
	if( LBUTTON_DOWN && !LBUTTON_HOLDING )
	{
		//Update current tile
		Check_Tile( TileNum, -1  );

		//Get the surrounding tiles
		int top=0,right=0,bottom=0,left=0;//instantiate to "out of bounds"

		if(TileNum-Axis_Size>=0)	
			if(Path_Grid[TileNum-Axis_Size] )		
				top = pTileController->Tiles[TileNum-Axis_Size].ID;
		if(TileNum+1<Grid_Len)	
			if(Path_Grid[TileNum+1] )					
				right = pTileController->Tiles[TileNum+1].ID;
		if(TileNum+Axis_Size<Grid_Len)	
			if(Path_Grid[TileNum+Axis_Size] )	
				bottom = pTileController->Tiles[TileNum+Axis_Size].ID;
		if(TileNum-1>=0)	
			if(Path_Grid[TileNum-1] )						
				left = pTileController->Tiles[TileNum-1].ID;

		//Update surrounding tiles
		if( left ) Check_Tile( left, TileNum );
		if( top ) Check_Tile( top, TileNum   );
		if( right ) Check_Tile( right, TileNum  );
		if( bottom ) Check_Tile( bottom,  TileNum   );

	}
}
/*
	Sets the texture before rendering the tile
	stage is the texture stage( usually 1, and 0 is for the base texture )
	//
	//--Hacked!!
	//

void Path_Class::Set_Texture( DWORD Stage, int TileNum )	
{
	//only set it if there is a path
	//if( Path_Grid[TileNum] )
	{
		HRESULT hr = g_D3D_Struct->g_pD3DDevice->SetTexture( Stage, pTexture ); 
		if(FAILED(hr))
		{
			Fatal_Error(hr, __LINE__, __FILE__ );
		}
	}
	//else
	{
		//There is no path, disable texture mapping for this tile.
	//	g_D3D_Struct->g_pD3DDevice->SetTextureStageState( Stage, D3DTSS_COLOROP, D3DTOP_DISABLE );
	}	

}
*/
void Path_Class::Free()
{
	if(pTexture)
	{
		pTexture->Release();
		pTexture = 0;
	}
	if( Path_Grid ) delete [] Path_Grid;
}

Path_Class::Path_Class(){ memset(this,0,sizeof(Path_Class) ); }
