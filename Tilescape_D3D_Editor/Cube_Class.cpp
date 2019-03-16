#include "Cube_Class.h"

/*--------------------------------------
	Calculates the box given a set
	of vertices.
--------------------------------------*/
void Cube_Class::_Calc( Vertex* verts, DWORD numverts )
{
	DWORD i;
	D3DXVECTOR3 min = D3DXVECTOR3( 1000000.0f, 1000000.0f, 1000000.0f );
	D3DXVECTOR3 max = D3DXVECTOR3( -1000000.0f, -1000000.0f, -1000000.0f );
	DWORD TEST;
	for( i=0; i<numverts-6; ++i )
	{
		min.x = min(verts[i].x,min.x);
		min.y = min(verts[i].y,min.y);
		min.z = min(verts[i].z,min.z);

		max.x = max(verts[i].x,max.x);
		max.y = max(verts[i].y,max.y);
		max.z = max(verts[i].z,max.z);
		if(i<-1000) TEST=i;
	}

	Min = min;
	Max = max;

	//See header, or documentation for point reference.
	P[0] = Max;
	P[1] = Max; P[1].x = Min.x;
	P[2] = Min; P[2].y = Max.y;
	P[3] = Max; P[3].z = Min.z;

	P[4] = Max; P[4].y = Min.y;
	P[5] = Min; P[5].z = Max.z;
	P[6] = Min;
	P[7] = Min; P[7].x = Max.z;

	//Set the initial contents of v
		for( int i=0; i<8; ++i )V[i]=P[i];
}
/*--------------------------------------
	Calculates the cube accumulatively
	based on the provided d3dxmesh.
	This should be used for a mesh
	that has a hierarchy, or multiple
	parts.
	- appmatrix: the matrix to apply
		to the points, this is usually
		a part of the allochierarchy class.
--------------------------------------*/
void Cube_Class::_ReCalc( LPD3DXMESH pMesh, D3DXMATRIX* appmatrix )
{
	
	if(!pMesh) return;

	HRESULT hr;
	DWORD i;

	Vertex* pVerts;
	Vertex	Temp;

	hr = pMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&pVerts );
	ErrorCheck(hr, __LINE__, __FILE__ );

	for( i=0; i < pMesh->GetNumVertices(); ++i )
	{
		Temp = *pVerts;

		//if there is a matrix to apply, then apply it
		if(appmatrix)
		{
			D3DXVECTOR3 v;
			v.x = Temp.x; v.y = Temp.y; v.z = Temp.z;
			D3DXVec3TransformCoord( &v, &v, appmatrix );	
			Temp.x = v.x; Temp.y = v.y; Temp.z = v.z;
		}
		//failsafe for loadmeshhierarchyfromx error
		if( ((!(Temp.x<-1000000))&&(!(Temp.y<-1000000))&&(!(Temp.z<-1000000))) && 
			((!(Temp.x>1000000))&&(!(Temp.y>1000000))&&(!(Temp.z>1000000)))
			)
		{
			Min.x = min( Temp.x, Min.x );
			Min.y = min( Temp.y, Min.y );
			Min.z = min( Temp.z, Min.z );

			Max.x = max( Temp.x, Max.x );
			Max.y = max( Temp.y, Max.y );
			Max.z = max( Temp.z, Max.z );
		}

		pVerts++;
	}

	hr = pMesh->UnlockVertexBuffer();
	ErrorCheck(hr, __LINE__, __FILE__);

	P[0] = Max;
	P[1] = Max; P[1].x = Min.x;
	P[2] = Min; P[2].y = Max.y;
	P[3] = Max; P[3].z = Min.z;

	P[4] = Max; P[4].y = Min.y;
	P[5] = Min; P[5].z = Max.z;
	P[6] = Min;
	P[7] = Min; P[7].x = Max.z;

	//Set the initial contents of v
	for( int i=0; i<8; ++i )V[i]=P[i];

}
/*--------------------------------------
	Rotates the cube along
	an axis
--------------------------------------*/
void Cube_Class::_Rotate( float degrees )
{
	;	
}
/*-------------------------------------------------
		--- Update() --- 
		Should be called once per frame per object.
		Updates the bound box based on the location 
		of the object.

		The first fn() updates based on a 
		transformation matrix; could be the
		world matrix.

		The second fn() updates based on a single 
		point of reference.
-------------------------------------------------*/
void Cube_Class::_Update( D3DXMATRIX* pmat )
{
	if( !pmat ) return;

	for( int i=0 ;i<8; ++i )
	{
		D3DXVec3TransformCoord( &V[i], &P[i], pmat );
	}
}
void Cube_Class::_Update( D3DXVECTOR3* ppos )
{
	if(!ppos) return;

	for( int i=0 ;i<8; ++i )
	{
		V[i].x = P[i].x + ppos->x;
		V[i].y = P[i].y + ppos->y;
		V[i].z = P[i].z + ppos->z;
	}
}
/*--------------------------------------
	Check if the box is visible.
	Just checks to see if the box
	is completely behind any
	of the frustum planes.
--------------------------------------*/
BOOL Cube_Class::_Visible( Frustum_Class * pFrustum )
{
	bool Visible;

	//Check to see if the box is completely behind a plane
	for( int i=0; i<6; ++i )
	{
		Visible=false;
		for( int j=0; j<8; ++j )
		{
			if( D3DXPlaneDotCoord( &pFrustum->Planes[i], &V[j] ) > 0 )//a positive result yeilds a point in front of the plane
			{
				//One or more points is in front of the plane.
				Visible=true; 
				j=8; 
			}
		}
		//The cube is completely behind one of the planes,
		// return true
		if(Visible==false) 
			return false;
	}
	return true;

}
/*--------------------------------------
	Slow cube drawing operation.
	Useful for debugging.
--------------------------------------*/
void Cube_Class::_DrawThis()
{

	float pointSize = 6.0f;
	g_D3D_Struct->g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));
	g_D3D_Struct->g_pD3DDevice->DrawPrimitiveUP( D3DPT_POINTLIST, 8, &P, sizeof(D3DXVECTOR3) );

}