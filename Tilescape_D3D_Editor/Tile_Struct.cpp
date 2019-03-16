#include "Tile_Struct.h"
#include "Base_Struct.h"
#include "Tile_Controller.h"

/*

	Tile_Struct Functions

--------------------------------*/
//Create the base pointer
void Tile_Struct::Init()
{

	int Side_Vert = 0;	//the vertex to the left of the one in question
	int Cur_Vert = 0;	//Side vert is to the left of Cur_Vert
	int Cur_Side = 0;	//the current side, relative to the vertex

	Polygon*	tmpPolys = pTileController->BaseBuffer->Polygons;	//for simpler code
	int*		nPolys = &pTileController->BaseBuffer->Num_Polygons;	//for simpler code
	int*		nVerts = &pTileController->BaseBuffer->Num_Verts;		//for simpler code
	
	float pu = (float)0.25f * Base_Tex;//stored in the tile structure

		pBase=new Base_Struct();//ONE BASE BECAUSE I CHANGED THE FORMAT	
		int cur_id=0;	
		for( int i=0; i<4; ++i )//for each side 
		{
			Side_Vert = (i==0 ? 3 : i-1);	//the vertex to the left of Cur_Vert
			Cur_Vert = i;					//Cur_Vert is the vertex on the right-top when viewing a side
			tmpPolys[*nPolys].p1 = *Top_Border_Verts[Cur_Vert];
			tmpPolys[*nPolys].p2 = *Top_Border_Verts[Side_Vert];
			tmpPolys[*nPolys].p3 = *Top_Border_Verts[Cur_Vert];
			tmpPolys[*nPolys].p3.y =0.0f;//-= pTileController->Base_Height;
			pBase->Sides[Cur_Vert].tri[0] = &tmpPolys[*nPolys];

			tmpPolys[*nPolys].p1.u = (float)(pu+0.249f);		tmpPolys[*nPolys].p1.v = 1.0f;
			tmpPolys[*nPolys].p2.u = pu+0.0f;					tmpPolys[*nPolys].p2.v = 1.0f;
			tmpPolys[*nPolys].p3.u = (float)(pu+0.249f);		tmpPolys[*nPolys].p3.v = 0.0f;
/*
			in.x = tmpPolys[*nPolys].p1.x;
			in.y = tmpPolys[*nPolys].p1.y;
			in.z = tmpPolys[*nPolys].p1.z;
			D3DXVec3Normalize( &norm, &in );
			tmpPolys[*nPolys].p1.nx = norm.x;
			tmpPolys[*nPolys].p1.ny = norm.y;
			tmpPolys[*nPolys].p1.nz = norm.z;
*/
			//store the buffer index of the polygon in the tile_struct
			Base_Pol_IDs[cur_id] = (*nPolys);
			cur_id++;

			(*nPolys)++;

			tmpPolys[*nPolys].p1 = *Top_Border_Verts[Side_Vert];
			tmpPolys[*nPolys].p2 = *Top_Border_Verts[Cur_Vert];
			tmpPolys[*nPolys].p3 = *Top_Border_Verts[Side_Vert];
			tmpPolys[*nPolys].p1.y =0.0f;//-= pTileController->Base_Height;
			tmpPolys[*nPolys].p2.y =0.0f;//-= pTileController->Base_Height;
			pBase->Sides[Cur_Vert].tri[1] = &tmpPolys[*nPolys];

			tmpPolys[*nPolys].p1.u = pu+0.249f;		tmpPolys[*nPolys].p1.v = 0.0f;
			tmpPolys[*nPolys].p2.u = pu+0.0f;		tmpPolys[*nPolys].p2.v = 1.0f;
			tmpPolys[*nPolys].p3.u = pu+0.249f;		tmpPolys[*nPolys].p3.v = 0.0f;
			
			//store the buffer index of the polygon in the tile_struct
			Base_Pol_IDs[cur_id] = (*nPolys);
			cur_id++;

			(*nPolys)++;
			(*nVerts)+=6;

/*			//don't know if i need these
			Bottom_Border_Verts[0] = &pBase->Sides[0].tri[0]->p2;
			Bottom_Border_Verts[1] = &pBase->Sides[0].tri[1]->p2;
			Bottom_Border_Verts[2] = &pBase->Sides[2].tri[0]->p2;
			Bottom_Border_Verts[3] = &pBase->Sides[2].tri[1]->p2;*/

		}
}
Tile_Struct::Tile_Struct()
{
	memset(this,0,sizeof(Tile_Struct) );
}


/*
	Raise_Vertex
	VertNum = vertex number to raise (0 to 3) or -1 for all
	Hill = wether to raise on a hill or not
	returns: 0=invalid input 1=too high 2=too low
	Options: -Omits the VertNum variable
			0  = Nothing 
			1  = Raise Lowest vert first
			2  = Raise Highest first
			-1 = Raise all verts
---------------------------------*/
int Tile_Struct::Raise_Vertex( int VertNum, bool Hill, int Options )
{
	int test = ID;
	if(ID<=0)return 0;//Quick and painless

	if(VertNum<0)return 0;						//Failure
	if(VertNum>pTileController->Num_Tiles) return 0;

	if(Options==-1)
	{
		Raise_Vertex(0,Hill,0);
		Raise_Vertex(1,Hill,0);
		Raise_Vertex(2,Hill,0);
		Raise_Vertex(3,Hill,0);
	}
	else if(Options==1) //raise the lowest vert out of all the tile's verts
	{
		float LL=100000.0f;
		for( int i=0; i<4; ++i )
		{
			if(Top_Border_Verts[i]->y<LL)
			{
				LL=Top_Border_Verts[i]->y;
			}
		}
		for( int i=0; i<4; ++i)
		{
			if( (int)Top_Border_Verts[i]->y == (int)LL)
				Raise_Vertex(i,0,0);
		}
	}
	else//raise single vertex
	{
		if( (Top_Border_Verts[VertNum]->y+pTileController->Base_Height) > pTileController->HeightUpperLimit ) return 1;
	
	
		Move_Tile_Up(VertNum, Hill);
		Update_BaseVerts();
		Omit_Base_Check();
	}
	//update the base structure of the bordering tiles
	for( int i=0; i<4; ++i ) 
		if(pBorderTiles[i]!=0)
			if(pBorderTiles[i]->pBase!=0)
				pBorderTiles[i]->Omit_Base_Check();

	pTileController->BaseBuffer->FillBuffer_Conditional();//might wanna take out for later
	return 1;

}
/*
	Lower_Vertex
	VertNum = vertex number to raise (0 to 3) or -1 for all
	Hill = wether to raise on a hill or not
	returns: 0=invalid input 1=too high 2=too low
	Options: -Omits the VertNum variable
			0  = Nothing 
			1  = Raise Lowest vert first
			2  = Raise Highest first
			-1 = Raise all verts
---------------------------------*/
int Tile_Struct::Lower_Vertex(int VertNum, bool Hill, int Options )
{
	int test = ID;

	//Test for height
	if(VertNum<0)return 0;//Failure

	if(Options==-1)
	{
		Lower_Vertex(0,Hill,0);
		Lower_Vertex(1,Hill,0);
		Lower_Vertex(2,Hill,0);
		Lower_Vertex(3,Hill,0);
	}
	else if(Options==2) //raise the lowest vert out of all the tile's verts
	{
		float LL=-1000.0f;
		for( int i=0; i<4; ++i )
		{
			if(Top_Border_Verts[i]->y>LL)
			{
				LL=Top_Border_Verts[i]->y;
			}
		}
		for( int i=0; i<4; ++i)
		{
			if( (int)Top_Border_Verts[i]->y == (int)LL)
				Lower_Vertex(i,0,0);
		}
	}
	else
	{
		if( (Top_Border_Verts[VertNum]->y-pTileController->Base_Height) < pTileController->HeightLowerLimit )return 2;
	
	
		Move_Tile_Down(VertNum, Hill);
		Update_BaseVerts();
		Omit_Base_Check();
	}
	//update the base structure of the bordering tiles
	for( int i=0; i<4; ++i ) 
		if(pBorderTiles[i]!=0)
			if(pBorderTiles[i]->pBase!=0)
				pBorderTiles[i]->Omit_Base_Check();


	pTileController->BaseBuffer->FillBuffer_Conditional();
	return 1;
}

/*
	Moves a tile up *works*
----------------------------*///changed
void Tile_Struct::Move_Tile_Up(int VertNum, bool Hill)
{
	if(VertNum==0)pPolys[0]->p1.y += pTileController->Base_Height;
	if(VertNum==1)pPolys[0]->p2.y += pTileController->Base_Height;	
	if(VertNum==1)pPolys[1]->p3.y += pTileController->Base_Height;
	if(VertNum==2)pPolys[1]->p1.y += pTileController->Base_Height;
	if(VertNum==3)pPolys[0]->p3.y += pTileController->Base_Height;
	if(VertNum==3)pPolys[1]->p2.y += pTileController->Base_Height;

	//Calculate the normal
/*	pPolys[0]->p1.Normal = pPolys[0]->p2.Normal = pPolys[0]->p3.Normal =
	Calc_Normal( &D3DXVECTOR3(pPolys[0]->p1.x, pPolys[0]->p1.y, pPolys[0]->p1.z), &D3DXVECTOR3(pPolys[0]->p2.x, pPolys[0]->p2.y, pPolys[0]->p2.z) );
	pPolys[1]->p1.Normal = pPolys[1]->p2.Normal = pPolys[1]->p3.Normal =
	Calc_Normal( &D3DXVECTOR3(pPolys[1]->p1.x, pPolys[1]->p1.y, pPolys[1]->p1.z), &D3DXVECTOR3(pPolys[1]->p2.x, pPolys[1]->p2.y, pPolys[1]->p2.z) );
*/
	pTileController->TileBuffer->FillBuffer();
}
/*
	Moves the actual tile face down
	VertNum= vertice to move
	Hill = if surrounding vertices are affected
________________________________________________________*/
void Tile_Struct::Move_Tile_Down( int VertNum, bool Hill )
{

	if(VertNum==0)pPolys[0]->p1.y -= pTileController->Base_Height;
	if(VertNum==1)pPolys[0]->p2.y -= pTileController->Base_Height;			
	if(VertNum==3)pPolys[0]->p3.y -= pTileController->Base_Height;
	if(VertNum==2)pPolys[1]->p1.y -= pTileController->Base_Height;
	if(VertNum==3)pPolys[1]->p2.y -= pTileController->Base_Height;			
	if(VertNum==1)pPolys[1]->p3.y -= pTileController->Base_Height;

	/*pPolys[0]->p1.Normal = pPolys[0]->p2.Normal = pPolys[0]->p3.Normal =
	Calc_Normal( &D3DXVECTOR3(pPolys[0]->p1.x, pPolys[0]->p1.y, pPolys[0]->p1.z), &D3DXVECTOR3(pPolys[0]->p2.x, pPolys[0]->p2.y, pPolys[0]->p2.z) );
	pPolys[1]->p1.Normal = pPolys[1]->p2.Normal = pPolys[1]->p3.Normal =
	Calc_Normal( &D3DXVECTOR3(pPolys[1]->p1.x, pPolys[1]->p1.y, pPolys[1]->p1.z), &D3DXVECTOR3(pPolys[1]->p2.x, pPolys[1]->p2.y, pPolys[1]->p2.z) );
*/
	pTileController->TileBuffer->FillBuffer();
}
/*

	Update the top vertices of the base so they match
	the corner vertices of the tile.

--------------------------------------------------*/
void Tile_Struct::Update_BaseVerts()
{

	int Side_Vert = 0;	//the vertex to the left of the one in question
	int Cur_Vert = 0;	//Side vert is to the left of Cur_Vert
	int Cur_Side = 0;	//the current side, relative to the vertex

	Polygon*	tmpPolys = pTileController->BaseBuffer->Polygons;	//for simpler code
	int*		nPolys = &pTileController->BaseBuffer->Num_Polygons;	//for simpler code
	int*		nVerts = &pTileController->BaseBuffer->Num_Verts;		//for simpler code
	
	float pu = (float)0.25f * Base_Tex;//stored in the tile structure

	//if there is no base create a new base 
	if(pBase==0)
	{

	}
	else
	{

		for( int i=0; i<4; ++i )//for each side
		{
			Side_Vert = (i==0 ? 3 : i-1);	//the vertex to the left of the one in question
			Cur_Vert = i;	

			int id0 = (i*2);
			int id1 = (i*2)+1;//01 23 45 67

			pBase->Sides[Cur_Vert].tri[0]->p1 = *Top_Border_Verts[Cur_Vert];
			pBase->Sides[Cur_Vert].tri[0]->p2 = *Top_Border_Verts[Side_Vert];
			pBase->Sides[Cur_Vert].tri[0]->p3 = *Top_Border_Verts[Cur_Vert];
			pBase->Sides[Cur_Vert].tri[0]->p3.y = 0.0f;//this->pBorderTiles[i]->Top_Border_Verts[;

			pBase->Sides[Cur_Vert].tri[1]->p1 = *Top_Border_Verts[Side_Vert];
			pBase->Sides[Cur_Vert].tri[1]->p2 = *Top_Border_Verts[Cur_Vert];
			pBase->Sides[Cur_Vert].tri[1]->p3 = *Top_Border_Verts[Side_Vert];
			pBase->Sides[Cur_Vert].tri[1]->p1.y = 0.0f;
			pBase->Sides[Cur_Vert].tri[1]->p2.y = 0.0f;

			pBase->Sides[Cur_Vert].tri[0]->p1.u = pu + 0.2490f; pBase->Sides[Cur_Vert].tri[0]->p1.v = (Top_Border_Verts[Cur_Vert]->y/pTileController->Base_Height);//1.0f;
			pBase->Sides[Cur_Vert].tri[0]->p2.u = pu + 0.00f;	pBase->Sides[Cur_Vert].tri[0]->p2.v = (Top_Border_Verts[Side_Vert]->y/pTileController->Base_Height);//1.0f;
			pBase->Sides[Cur_Vert].tri[0]->p3.u = pu + 0.2490f; pBase->Sides[Cur_Vert].tri[0]->p3.v = 0.0f;
			
			pBase->Sides[Cur_Vert].tri[1]->p1.u = pu + 0.000f;	pBase->Sides[Cur_Vert].tri[1]->p1.v = 0.0f;
			pBase->Sides[Cur_Vert].tri[1]->p2.u = pu + 0.2490f;  pBase->Sides[Cur_Vert].tri[1]->p2.v = 0.0f;
			pBase->Sides[Cur_Vert].tri[1]->p3.u = pu + 0.00f;	pBase->Sides[Cur_Vert].tri[1]->p3.v = (Top_Border_Verts[Side_Vert]->y/pTileController->Base_Height);//1.0f;
		

		}//for( i=0, i<4...
	}//if(pbase==0) else...
}
/*
	Flip two triangles (a square) 90d ccw
	should be called before moving the Buffer->Vertices
	of the triangle edges swaps the Buffer->Vertices
----------------------------------------------------*/
void Tile_Struct::Tri_Flip()
{
	
	Vertex Temp, Temp2;

	//First check what mode we're in
	if(Top_Mode==0)
	{
		Temp = pPolys[0]->p1;
		Temp2 = pPolys[1]->p1;

		pPolys[0]->p1 = pPolys[0]->p2;
		pPolys[0]->p2 = pPolys[1]->p1; //(Polys[1]->p1)
		pPolys[0]->p3 =  Temp; //(Polys[0]->p1)
		
		pPolys[1]->p1 = pPolys[1]->p2;
		pPolys[1]->p2 = Temp;
		pPolys[1]->p3 = Temp2;//(t1.b)

	//really shouldn't because of pSharedVerts
		Top_Border_Verts[0] = &pPolys[0]->p1;
		Top_Border_Verts[1] = &pPolys[0]->p2;
		Top_Border_Verts[2] = &pPolys[1]->p1;
		Top_Border_Verts[3] = &pPolys[0]->p3;

		Top_Mode=1;
	}
	else
	{
		Vertex Temp = pPolys[0]->p1;

		pPolys[0]->p2 = pPolys[0]->p1;
		pPolys[0]->p1 = pPolys[0]->p3;
		pPolys[0]->p3 = pPolys[1]->p1;
		
		pPolys[1]->p2 = pPolys[1]->p1;
		pPolys[1]->p1 = pPolys[1]->p3;
		pPolys[1]->p3 = Temp;

		Top_Border_Verts[0] = &pPolys[0]->p3; 
		Top_Border_Verts[1] = &pPolys[0]->p1;
		Top_Border_Verts[2] = &pPolys[0]->p2;
		Top_Border_Verts[3] = &pPolys[1]->p1;

		Top_Mode=0;
	}
	Update_BaseVerts();

}

void Tile_Struct::Change_Tex( int Num )
{
	if(Num<0||Num>3)	return;		//ID out of range
	if(Top_Tex==Num)	return;		//Texture is already assigned

	float pu=0.0f, pv=0.0f;

	Top_Tex = Num;

	if(Num==1||Num==3)
		pu = 0.5f;
	if(Num==2||Num==3)
		pv = 0.5f;
	pPolys[0]->p1.u = pu;			pPolys[0]->p1.v = pv;
	pPolys[0]->p2.u = 0.49f + pu;	pPolys[0]->p2.v = pv;
	pPolys[0]->p3.u = pu;			pPolys[0]->p3.v = 0.49f + pv;

	pPolys[1]->p1.u = 0.49f + pu;	pPolys[1]->p1.v = 0.49f + pv;
	pPolys[1]->p2.u = pu;			pPolys[1]->p2.v = 0.49f + pv;
	pPolys[1]->p3.u = 0.49f + pu;	pPolys[1]->p3.v = pv;
}
/*
	Change the texture of the base
	Num : Texture ID, contained on the base texture map
*/
void Tile_Struct::Change_Base_Tex( int Num )
{
	if(pBase==0) return;		//base doesn't exist
	if(Base_Tex==Num) return;	//Texture already applied

	if(Num<0||Num>3)return;
	float pu=0.0f, pv=0.0f;

	Base_Tex = Num;

	pu = 0.249f*Num;

	for( int i=0; i<4; ++i )
	{
		Polygon* t0 = pBase->Sides[i].tri[0]; //for neater code
		Polygon* t1 = pBase->Sides[i].tri[1]; //for neater code

		t0->p1.u = pu;			t0->p1.v = 0.0f;
		t0->p2.u = pu+0.249f;	t0->p2.v = 0.0f;
		t0->p3.u = pu;			t0->p3.v = 1.0f;

		t1->p1.u = pu+0.249f;	t1->p1.v = 1.0f;
		t1->p2.u = pu;			t1->p2.v = 1.0f;
		t1->p3.u = pu+0.249f;	t1->p3.v = 0.0f;

	}
}
/*

	Omit_Base_Check
	- checks if base polygons are not visible
	- Clears the flag in the BaseBuffer if not
	Note: base sides turned out to be OPPOSITE
		than what is displayed in the documentation.
	Note(2): Compared to the documentation the top border 
			verts are rotated once to the right and
			the polygons are rotated twice to the right 
			(i.e. polys 0,1 become 4,5) (verts 3,2 become 0,3).
-----------------------------------------*/
void Tile_Struct::Omit_Base_Check()
{
	bool* bDraw = pTileController->BaseBuffer->bCan_Draw; //for simpler code

	//Bottom side is not visible
	if(pBorderTiles[0]!=0)//make sure the pointer exists
	{
		if( (pBorderTiles[0]->Top_Border_Verts[0]->y >= Top_Border_Verts[1]->y) && 
			(pBorderTiles[0]->Top_Border_Verts[3]->y >= Top_Border_Verts[2]->y) )//32, 01 - original
		{//0,1 - original
			bDraw[Base_Pol_IDs[4]]=FALSE;
			bDraw[Base_Pol_IDs[5]]=FALSE;
		}
		else
		{
			bDraw[Base_Pol_IDs[4]]=TRUE;
			bDraw[Base_Pol_IDs[5]]=TRUE;
		}
	}
	//Right side is not visible
	if(pBorderTiles[1]!=0)
	{
		if( (pBorderTiles[1]->Top_Border_Verts[1]->y >= Top_Border_Verts[2]->y) && 
			(pBorderTiles[1]->Top_Border_Verts[0]->y >= Top_Border_Verts[3]->y) )// 03, 12 - original
		{//2,3 - original
			bDraw[Base_Pol_IDs[6]]=FALSE;
			bDraw[Base_Pol_IDs[7]]=FALSE;
		}
		else
		{
			bDraw[Base_Pol_IDs[6]]=TRUE;
			bDraw[Base_Pol_IDs[7]]=TRUE;
		}
	}
	//Top side is not visible
	if(pBorderTiles[2]!=0)
	{
		if( (pBorderTiles[2]->Top_Border_Verts[2]->y >= Top_Border_Verts[3]->y) && 
			(pBorderTiles[2]->Top_Border_Verts[1]->y >= Top_Border_Verts[0]->y) )//10, 32 - original
		{//4,5 - original
			bDraw[Base_Pol_IDs[0]]=FALSE;
			bDraw[Base_Pol_IDs[1]]=FALSE;
		}
		else
		{
			bDraw[Base_Pol_IDs[0]]=TRUE;
			bDraw[Base_Pol_IDs[1]]=TRUE;
		}
	}
	//Left side is not visible
	if(pBorderTiles[3]!=0)
	{
		if( (pBorderTiles[3]->Top_Border_Verts[3]->y >= Top_Border_Verts[0]->y) && 
			(pBorderTiles[3]->Top_Border_Verts[2]->y >= Top_Border_Verts[1]->y) )// 21, 30 -original
		{//6,7 - original
			bDraw[Base_Pol_IDs[2]]=FALSE;
			bDraw[Base_Pol_IDs[3]]=FALSE;
		}
		else
		{
			bDraw[Base_Pol_IDs[2]]=TRUE;
			bDraw[Base_Pol_IDs[3]]=TRUE;
		}
	}

}	

float Tile_Struct::Get_Lowest_Height()
{	if(Top_Border_Verts==0) return 0.0f;

	return min(
			min(Top_Border_Verts[0]->y,Top_Border_Verts[1]->y),
			min(Top_Border_Verts[2]->y,Top_Border_Verts[3]->y)
			);
}

float Tile_Struct::Get_Highest_Height()
{
	if(Top_Border_Verts==0) return 0.0f;

	return max(
			max(Top_Border_Verts[0]->y,Top_Border_Verts[1]->y),
			max(Top_Border_Verts[2]->y,Top_Border_Verts[3]->y)
			);
}