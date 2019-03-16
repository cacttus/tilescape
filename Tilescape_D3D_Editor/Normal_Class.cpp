#include "Normal_Class.h"
void Normal_Class::Make_From_Plane( D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3 )
{
	//(p3-p1).cross(p2-p1)
	D3DXVECTOR3 Temp;
	Normal = p3-p1;
	Temp = p2-p1;
	D3DXVec3Cross(&Normal, &Normal, &Temp);
	D3DXVec3Normalize(&Normal, &Normal);

}
void Normal_Class::Make_From_Vertex( D3DXVECTOR3 PlaneNormal, D3DXVECTOR3 Vertice )
{
	//(p3-p1).cross(p2-p1)
	Normal.x = PlaneNormal.x + Vertice.x;
	Normal.y = PlaneNormal.y + Vertice.y;
	Normal.z = PlaneNormal.z + Vertice.z;
	D3DXVec3Normalize(&Normal, &Normal);
}