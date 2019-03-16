/*


	Tile Engine
	Derek Page
	Aug 02, 2007	11:10p

-----------------------------------------*/

/* Return a D3DXVECTOR3 normal based on two points
D3DXVECTOR3 Calc_Normal(D3DXVECTOR3* p1, D3DXVECTOR3* p2 )//returns a normalized normal based on two corners of a plane
{
	D3DXVECTOR3 Ret;
	D3DXVec3Cross(&Ret, p1, p2 );
	D3DXVec3Normalize( &Ret, &Ret );
	return Ret;
}*/
#ifndef __TILE__STRUCT__H__
#define __TILE__STRUCT__H__


#include "InitD3D.h"
#include "Polygon.h"
#include "Buffer_Class.h"
#include "Side_Struct.h"
#include "Filedef.h"
#include "Base_Struct.h"
#include "Tile_Buffer_Class.h"

struct Tile_Struct;
struct Base_Struct;
struct Tile_Control_Struct;
class Tile_Buffer_Class;

struct UV
{
	float u,v;
	UV(){memset(this,0,sizeof(UV));}
};

/*

	Tile Structure

-----------------------------*/
struct Tile_Struct
{	
	void			Move_Tile_Up(int VertNum, bool Hill);
	void			Move_Tile_Down( int VertNum, bool Hill );
	int				Raise_Vertex( int num, bool Hill, int Options );	//num: vertex number Hill:if hill Options: 1=raise lowest 2=highest -1=raise all
	int				Lower_Vertex( int num, bool Hill, int Options  );
	void			Update_BaseVerts();
	bool			Top_Mode;			//Topology of top triangles
	int				Top_Tex;			//The ground type of the tile.
	int				Base_Tex;			//The vertical face type of the tile.
	int				Material;			//material group
	int				Base_Pol_IDs[8];	//Id's of the base polygons(their offset in the buffer)

public:	

	Tile_Control_Struct* pTileController;	//pointer to the tile info structrure

	int ID;									//Tile id assigned upon room creation
	float Vert_Height[4];					//Height of each vertice
	float Top_Border_Height[4];
	float pSharedHeight[4];

	Base_Struct*	pBase; 					//pointer to base below/above tile
	Polygon*		pPolys[2];				//the Buffer->Vertices of the tile's triangles (the actual Buffer->Vertices in the vertex buffer)
	Vertex*			Top_Border_Verts[4];	//Corners at the top of the tile must be a pointer (points to the vertex buffer)d
	Vertex*			Bottom_Border_Verts[4];	//Corners at the base of the tile must be a pointer (points to the vertex buffer)
	Vertex*			pSharedVerts[8];		//Vertices shared by the corners of other tiles ( the shared Top_Border_Verts )
	Base_Struct*	pVertexBases[4];		//Base pointers for per vertices
	Tile_Struct*	pBorderTiles[4];		//tiles bordering this tile

	void Init();

	void CheckBaseVisible(){;}		//the base is visible if the top or bottom of the tile is visible
	void Tri_Flip();				//Flip the order of the top two triangles, to change the topology of the tile
	
	void Change_Tex(int Num);		//Updates the texture, which is assumed to be a single map, 0=tl tex 1=tr 2=bl 3=br
	void Change_Base_Tex(int Num);	//Change the base-face texture

	void Omit_Base_Check();			//Check if the polygons of the base are visible, if not, they are culled out
	
	float Get_Lowest_Height();		//Returns the lowest y value of the border vertices
	float Get_Highest_Height();		//Returns the highest y value of the border vertices

	Tile_Struct();
	~Tile_Struct(){;}

};

#endif