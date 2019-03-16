#include "Dig_Tool_Class.h"
#include "Commdef.h"
#include "Button_Functions.h"

int DigToolState=0;//check the commdef.h file
int DigTool_RaiseLandRadius=1;
/*
void Dig_Tool_Class::Update(POINT* g_CursorPos, Tile_Control_Struct* g_TileInfo, 
							Tile_Picker_Class* TilePicker, int g_Screen_Width, int g_Screen_Height)
{
	if( g_User_State==GLOBAL_DIGTOOL )
	{
		if(Keydown(VK_LBUTTON))
		{
				if(LBUTTONDOWN==0)
				{
					if(FlipTris==0)
					{		
						MouseYForMoving = g_CursorPos->y;
						LBUTTONDOWN=1;
					}
					else
					{
						g_TileInfo->Tiles[g_TileInfo->PickedTile].Tri_Flip();
						LBUTTONDOWN=2;
					}
				}
				else//lbutton was already initialized 
				{
					if(Keydown(VK_CONTROL)) LiftHill=1; else LiftHill=0;
					TilePicker->Update(g_TileInfo->PickedVert, &g_TileInfo->Tiles[g_TileInfo->PickedTile] );
					// Hill
					if( DigToolState ==DIGTOOL_RAISEHILL)
					{
						if(MouseYForMoving-g_CursorPos->y>10)
						{
							g_TileInfo->Raise_Hill_Tiles(2, 3, g_TileInfo->Tiles[g_TileInfo->PickedTile].ID);
							MouseYForMoving=g_CursorPos->y;
						}
						else if(MouseYForMoving-g_CursorPos->y<-10)
						{
							g_TileInfo->Lower_Hill_Tiles(2, 3, g_TileInfo->Tiles[g_TileInfo->PickedTile].ID);
							MouseYForMoving=g_CursorPos->y;
						}
					}
					else if(DigToolState==DIGTOOL_RAISELAND)//----Per vertex, and single tile operations----
					{
						if(DigTool_RaiseLandRadius>1)
						{
							if(MouseYForMoving-g_CursorPos->y>10)
							{
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Change_Tex( CUR_TILE_TEX_ID );
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Change_Base_Tex( CUR_BASE_TEX_ID );//change the texture (if needed)

								g_TileInfo->Raise_Batch_Tiles(DigTool_RaiseLandRadius, g_TileInfo->Tiles[g_TileInfo->PickedTile].ID);

								MouseYForMoving=g_CursorPos->y;
							}
							else if(MouseYForMoving-g_CursorPos->y<-10)
							{
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Change_Tex( CUR_TILE_TEX_ID );
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Change_Base_Tex( CUR_BASE_TEX_ID );

								g_TileInfo->Lower_Batch_Tiles(DigTool_RaiseLandRadius, g_TileInfo->Tiles[g_TileInfo->PickedTile].ID);
								MouseYForMoving=g_CursorPos->y;
							}
						}
						else
						{
							if(MouseYForMoving-g_CursorPos->y>10)
							{
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Change_Tex( CUR_TILE_TEX_ID );
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Change_Base_Tex( CUR_BASE_TEX_ID );//change the texture (if needed)

								if(g_TileInfo->PickedVert==-1)
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Raise_Vertex(0,LiftHill,-1);
								else
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Raise_Vertex(g_TileInfo->PickedVert,LiftHill,0);
								MouseYForMoving=g_CursorPos->y;
							}
							else if(MouseYForMoving-g_CursorPos->y<-10)
							{
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Change_Tex( CUR_TILE_TEX_ID );
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Change_Base_Tex( CUR_BASE_TEX_ID );

								if(g_TileInfo->PickedVert==-1)
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Lower_Vertex(0,LiftHill,-1);
								else
								g_TileInfo->Tiles[g_TileInfo->PickedTile].Lower_Vertex(g_TileInfo->PickedVert,LiftHill,0);
								MouseYForMoving=g_CursorPos->y;
							}
						}//if digtool_raiselandrdius <= 1
					}
					else if(DigToolState==DIGTOOL_RAISEALL)
					{
						if(MouseYForMoving-g_CursorPos->y>10)
						{
							g_TileInfo->Raise_All_Tiles();
							MouseYForMoving=g_CursorPos->y;
						}
						else if(MouseYForMoving-g_CursorPos->y<-10)
						{
							g_TileInfo->Lower_All_Tiles();
							MouseYForMoving=g_CursorPos->y;
						}
					}
					
				}
			}//if keydown(vk_lbutton
			else
			{
				LBUTTONDOWN=0;

	
			}
		
		if(Keydown(VK_F1)&&DGKD==0){ DGKD=1; if(DrawGrid)DrawGrid=0; else DrawGrid=1; }
		else DGKD=0;

		if(Keydown(VK_RBUTTON)) FlipTris=1; else FlipTris=0;
	} //if (digtoolstate)

}*/