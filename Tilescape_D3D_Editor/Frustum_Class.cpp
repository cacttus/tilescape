#include "Frustum_Class.h"

void Frustum_Class::Update()
{
	D3DXMATRIX view, proj, vpm;
	int i;

	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_VIEW, &view );
	g_D3D_Struct->g_pD3DDevice->GetTransform( D3DTS_PROJECTION, &proj );

	D3DXMatrixMultiply( &vpm, &view, &proj );

	//The widely used "get frustum from matrix" routine.
	//I can't seem to find the original abstract on this.
	Planes[0].a = vpm._14 + vpm._13;
	Planes[0].b = vpm._24 + vpm._23;
	Planes[0].c = vpm._34 + vpm._33;
	Planes[0].d = vpm._44 + vpm._43;

	Planes[1].a = vpm._14 - vpm._13;
	Planes[1].b = vpm._24 - vpm._23;
	Planes[1].c = vpm._34 - vpm._33;
	Planes[1].d = vpm._44 - vpm._43;

	Planes[2].a = vpm._14 + vpm._11;
	Planes[2].b = vpm._24 + vpm._21;
	Planes[2].c = vpm._34 + vpm._31;
	Planes[2].d = vpm._44 + vpm._41;

	Planes[3].a = vpm._14 - vpm._11;
	Planes[3].b = vpm._24 - vpm._21;
	Planes[3].c = vpm._34 - vpm._31;
	Planes[3].d = vpm._44 - vpm._41;

	Planes[4].a = vpm._14 + vpm._12;
	Planes[4].b = vpm._24 + vpm._22;
	Planes[4].c = vpm._34 + vpm._32;
	Planes[4].d = vpm._44 + vpm._42;

	Planes[5].a = vpm._14 - vpm._12;
	Planes[5].b = vpm._24 - vpm._22;
	Planes[5].c = vpm._34 - vpm._32;
	Planes[5].d = vpm._44 - vpm._42;

	for( i=0; i<6; ++i )
		D3DXPlaneNormalize( &Planes[i], &Planes[i] );

}
/*--------------------------------------------
		Point inside frustum routine.
		Returns true if a point is in
		the frustum, false otherwise.
--------------------------------------------*/
BOOL Frustum_Class::_Point( D3DXVECTOR3 p )
{
	int i;
	for( i=0; i<6; ++i )
	{
		if(D3DXPlaneDotCoord( &Planes[i], &p )>=0.0f) return TRUE;
	}
	return FALSE;
}