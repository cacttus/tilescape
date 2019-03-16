#include "Buffer_Class.h"
void Buffer_Class::CreateBuffer( int size, DWORD flags )
{
	HRESULT hr;
	//Fill the D3D buffer
	if( flags & D3DUSAGE_DYNAMIC )
	{
		FLAG_DYNAMIC = true;
		hr = g_D3D_Struct->g_pD3DDevice->CreateVertexBuffer( size, //size of buffer
			flags, 
			FVF_DESCRIPTOR, 
			D3DPOOL_DEFAULT, 
			&D3DBuffer, //the vertex buffer (d3d)
			NULL );
	}
	else
	{
		FLAG_DYNAMIC = false;
		hr = g_D3D_Struct->g_pD3DDevice->CreateVertexBuffer( size, //size of buffer
			flags,
			FVF_DESCRIPTOR, 
			D3DPOOL_MANAGED, 
			&D3DBuffer, //the vertex buffer (d3d)
			NULL );
	}
	if(hr!=D3D_OK)  Fatal_Error("Failed to create vertex buffer");
}

void Buffer_Class::AddPoly( Polygon* Poly )
{
}
void Buffer_Class::Init( int Num_Polys, char* Tex, DWORD Flags )
{
	Num_Polygons = (int)Num_Polys;
	Num_Verts = (int)Num_Polys*3;
	Active_Polygons = Num_Polygons;

	Polygons = new Polygon[	Num_Polygons ];
	memset(Polygons,0,sizeof(Polygon)*Num_Polygons);
	bCan_Draw = new bool[ Num_Polygons ];

	CreateBuffer( Num_Polygons*sizeof(Polygon), Flags );
	Set_Draw_Flags();

	D3DTexture.Load( Tex, g_D3D_Struct->g_pD3DDevice );
}
void Buffer_Class::FillBuffer_Conditional()//Slower than FillBuffer, but checks if polygons are used
{
	int cur=0;
	int n=0;
	Polygon* temp;
	for(int i=0; i<Num_Polygons; ++i )
		if(bCan_Draw[i])n++;
	temp = new Polygon[n];
	for( int i=0; i<Num_Polygons; ++i )
		if(bCan_Draw[i]){
			memcpy( (temp+cur), &Polygons[i], 3*sizeof(Vertex));
			cur++;
		}
	//Fill the buffer
	if(FLAG_DYNAMIC)
	{
		VOID* lpVoid;
		D3DBuffer->Lock( 0, 0, &lpVoid, D3DLOCK_DISCARD );
		memcpy( lpVoid, temp, cur*3*sizeof(Vertex) );
		D3DBuffer->Unlock();	
	}
	else
	{
		VOID* lpVoid;
		D3DBuffer->Lock( 0, 0, &lpVoid, 0 );		
		memcpy( lpVoid, temp, cur*3*sizeof(Vertex) );
		D3DBuffer->Unlock();	
	}
	delete [] temp;
}
void Buffer_Class::FillBuffer()
{

	if(FLAG_DYNAMIC)
	{
		VOID* lpVoid;
		D3DBuffer->Lock( 0, 0, &lpVoid, D3DLOCK_DISCARD );
		memcpy( lpVoid, Polygons, Num_Polygons*3*sizeof(Vertex) );
		D3DBuffer->Unlock();	
	}
	else
	{
		VOID* lpVoid;
		D3DBuffer->Lock( 0, 0, &lpVoid, 0 );		
		memcpy( lpVoid, Polygons, Num_Polygons*3*sizeof(Vertex) );
		D3DBuffer->Unlock();	
	}
}	
void Buffer_Class::FillBuffer( VOID* data )
{
	VOID* lpVoid;
	D3DBuffer->Lock( 0, 0, &lpVoid, 0 );		
	memcpy( lpVoid, data, Num_Polygons*3*sizeof(Vertex) );
	D3DBuffer->Unlock();	
}
void Buffer_Class::Set_Draw_Flags()	//Sets all the polygon flags to 1 (Draw)
{
	memset(bCan_Draw,1,sizeof(bool)*Num_Polygons);

}
void Buffer_Class::Clear_Draw_Flags() //Sets all the polygon flags to 0 (Don't Draw )
{
	memset(bCan_Draw,0,sizeof(bool)*Num_Polygons);
}
void Buffer_Class::Check_Draw_Flags()
{
	Active_Polygons=0;
	for( int i=0; i<Num_Polygons; ++i )
	{
		if( bCan_Draw[i]==1 )
		Active_Polygons++;
	}
}
void Buffer_Class::DrawBuffer(void)
{
	;
}
void Buffer_Class::Free()
{
	D3DTexture.Free(); 
	D3DBuffer->Release(); 
	delete [] Polygons;
	delete [] bCan_Draw;
}