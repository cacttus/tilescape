#include "Tile_Buffer_Class.h"
/*

	Tile buffer functions

-----------------------------*/
void Tile_Buffer_Class::CheckAllTilesVisible(int w, int h, Tile_Control_Struct* tcs)
{
	D3DXMATRIX proj, view, world;
	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_PROJECTION, &proj );
	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_VIEW, &view );
	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_WORLD, &world );
	
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = w;
	viewport.Height = h;
	viewport.MinZ=1.0f;//----------------------------------------------------------CHANGE L8ER
	viewport.MaxZ = 500.0f;
	
	D3DXVECTOR3 Temp;
	D3DXVECTOR3 In;
	Num=0;
	//Loop through all tiles n' check which are visible
	for( int i=0; i<tcs->Num_Tiles; ++i )
	{
		for( int j=0; j<4; ++j )
		{
			In.x = tcs->Tiles[i].Top_Border_Verts[j]->x;
			In.y = tcs->Tiles[i].Top_Border_Verts[j]->y;
			In.z = tcs->Tiles[i].Top_Border_Verts[j]->z;
			D3DXVec3Project(&Temp, &In, &viewport, 
			&proj, &view, &world);
			if((int)Temp.x>0 && (int)Temp.y>0 && (int)Temp.x<w && (int)Temp.y<h)//Tile is inside
			{
				pTiles[Num] = &tcs->Tiles[i];
				Num++;
				j=4;//exit the loop
			}
		}

	}

}
void Tile_Buffer_Class::Init(Tile_Control_Struct* tcs ){//called in the Procedural_Land function
		pTiles = new Tile_Struct*[tcs->Num_Tiles]; 
		memset(pTiles, 0, 4*tcs->Num_Tiles);
	}