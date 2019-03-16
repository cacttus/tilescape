/*

	Paths that overlay tiles.
	Controls the reinstantiation, 
	sets the texture, and sets
	the UV co-ordinates on the
	respective Tile_Struct polygons.

	The path texture should be a 256x256 texture
	seperated into 64x64 paths with one blank 
	path at the beginning. It should be in
	the order of the above.

	Derek Page Sept 6, 2007

---------------------------------*/

#ifndef __PATH__CLASS__H__
#define __PATH__CLASS__H__

#include "Tile_Picker_Class.h"


/*

	left, up, right, down (order)
	- the ones specify WHAT SIDE THERE ARE TILES (left cap would have a tile to the left)
	0=	0000		no path
	8=	1000		left cap
	4=	0100		top cap
	2=	0010		right cap
	1=	0001		bottom cap

	12=	1100		left top
	6=	0110		top right
	3=	0011		right bottom
	9=	1001		bottom left

	10=	1010		left right (horizontal)
	5=	0101		top bottom (vertical)
	14=	1110		left top right
	7=	0111		top right bottom
	11=	1011		right bottom left
	13=	1101		bottom left top
	15=	1111		all

*/

//Definitions for the tiles
#define _LEFT_CAP			0x08
#define _TOP_CAP			0x04
#define _RIGHT_CAP			0x02
#define _BOTTOM_CAP			0x01

#define _LEFT_TOP			( 0x08 | 0x04 )
#define _TOP_RIGHT			( 0x04 | 0x02 )
#define _RIGHT_BOTTOM		( 0x02 | 0x01 )
#define _BOTTOM_LEFT		( 0x01 | 0x08 )

#define _LEFT_RIGHT			( 0x08 | 0x02 )
#define _TOP_BOTTOM			( 0x04 | 0x01 )

#define _LEFT_TOP_RIGHT		( ( 0x08 | 0x04 ) | 0x02 )
#define _TOP_RIGHT_BOTTOM	( ( 0x04 | 0x02 ) | 0x01 )
#define _RIGHT_BOTTOM_LEFT	( ( 0x02 | 0x01 ) | 0x08 )
#define _BOTTOM_LEFT_TOP	( ( 0x01 | 0x08 ) | 0x04 )
#define _ALL				( ( ( 0x08 | 0x04 ) | 0x02 ) | 0x01 )

class Path_Class
{
	char*		Path_Grid;			//A grid of path states, per tile 0 to 12 states

	//int*		Path_Grid_Type;		//An array of types of paths (like dirt, snow etc )(just 0 for now )

	int			Grid_Len;			//Size of the array Path_Grid
	int			Axis_Size;			//Size of a single axis
		
	bool		b_Path_Edit_Mode;	//If the editor is in path edit mode
	bool		LBUTTON_DOWN;		//if the left mouse button is down
	bool		LBUTTON_HOLDING;	//if the left mouse button is being held

	//Tile_Control_Struct*	pTileController;	//Pointer to the tile controller
	IDirect3DTexture9*		pTexture;	//The the texture to load


	void Place_Path_Tile( );		//Places a path tile, converging the surrounding tiles
	void Check_Tile( int TileNum, int MainTileInd  );			//Checks a tile's surrounding tiles and changes it's texture accordingly

public:
	void Set_Texture( DWORD Stage, int TileNum );	//Sets the texture stage is the texture stage( usually 1, and 0 is for the base texture )

	//void Init( int size, char* TexLocation, Tile_Control_Struct* tcs  );		//Initialize the class specifying the x,y size of the field (only one axis, it is square) and the texture location
	void Set_Path( int TileNum );	//Update and check
	void Free();

	Path_Class();
};


#endif