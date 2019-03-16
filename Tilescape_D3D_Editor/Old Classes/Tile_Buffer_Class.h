#ifndef __TILE__BUFFER__CLASS__H__
#define __TILE__BUFFER__CLASS__H__

struct Tile_Control_Struct;
struct Tile_Struct;

#include "Tile_Struct.h"
#include "Tile_Controller.h"


/*

	Tile buffer class
	holds a bunch of pointers to the tiles that 
	are located on the screen (visible)

------------------------------------------------*/
class Tile_Buffer_Class
{
public:
	Tile_Struct** pTiles;	//array of Pointers to visible tiles
	int Num;				//number of tile(pointers) in the buffer after calling CheckAllTilesVisible
	void Init(Tile_Control_Struct* tcs);
	void CheckAllTilesVisible(int w, int h, Tile_Control_Struct* tcs); //check what tiles are visible and update the pTiles variable

};

#endif