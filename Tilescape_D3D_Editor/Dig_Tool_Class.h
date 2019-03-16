#ifndef __DIG__TOOL__CLASS__H__
#define __DIG__TOOL__CLASS__H__
#include "InitD3D.h"
#include "Tile_Picker_Class.h"


//The global states of the menu


extern int DigToolState;//check the commdef.h file
extern int DigTool_RaiseLandRadius;

class Dig_Tool_Class
{
	
	bool KEYDOWN;

	int LBUTTONDOWN;
	//short	PickedVert;
	//short	PickedTile;
	int MouseYForMoving;//=0;
	bool LiftHill;//=0;
	bool FlipTris;//=0; //if right mouse button is down before left and in edit mode, then when left is clicked, flip the tris.
	bool DrawGrid;//=1; //if the grid should be drawn
	bool DGKD;//=0;	//the drawgrid key is down
	bool b_Terrain_Edit_Mode;//=1; //if the editor is in terrain mode
	POINT g_Lastp;
	//Tile_Struct Picked_Tile;
	bool CONTROL_DOWN, ALT_DOWN;//= 0;
	bool F_DOWN, B_DOWN;
	bool bMinus_Tool; //if using the minus tool
	bool bBatch_Tiles; //if Batch Tiles is on
	bool b_F1DOWN, b_F2DOWN, b_F3DOWN;
	unsigned int CUR_BASE_TEX_ID, CUR_TILE_TEX_ID;

public:
	//void Update(POINT* g_CursorPos, Tile_Control_Struct* g_TileInfo, 
	//	Tile_Picker_Class* TilePicker, int g_Screen_Width, int g_Screen_Height);
	Dig_Tool_Class(){memset(this,0,sizeof(Dig_Tool_Class));}
};
#endif