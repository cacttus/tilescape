/*
	Class to hold buffer data
	on a per-material basis.
	Derek Page
	August 5, 2007
-----------------------------------*/
#ifndef __BUFFER__CLASS__H__
#define __BUFFER__CLASS__H__

#include "InitD3D.h"
#include "Texture_Class.h"
#include "Vertex.h"
#include "polygon.h"

class Buffer_Class{
	void CreateBuffer( int size, DWORD flags );

public:

	int Num_Verts;			//Number of vertices in buffer
	int Num_Polygons;		//Number of polygons in buffer
	int Active_Polygons;	//Number of active polygons in buffer
	int Cur_Poly;

	IDirect3DVertexBuffer9	*D3DBuffer;		//Vertex buffer to hold data for this type of material
	Texture_Class			D3DTexture;		//The texture
	Polygon					* Polygons;		//Vertices within the buffer
	bool					* bCan_Draw;	//If the respective polygon should be drawn

	bool FLAG_DYNAMIC;		//If the buffer is a dynamic vb

	/* Functions */
	void AddPoly( Polygon* Poly );

	void Init( int Num_Polys, char* Tex, DWORD Flags );

	void FillBuffer_Conditional();//Slower than FillBuffer, but checks if polygons are used
	void FillBuffer();
	void FillBuffer( VOID* data );

	void Set_Draw_Flags();	//Sets all the polygon flags to 1 (Draw)
	void Clear_Draw_Flags(); //Sets all the polygon flags to 0 (Don't Draw )
	void Check_Draw_Flags();
	
	void DrawBuffer(void);
	void Free();

	Buffer_Class(){ memset(this,0,sizeof(Buffer_Class)); }
};

#endif