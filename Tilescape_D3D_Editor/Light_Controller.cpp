#include "Light_Controller.h"
/* Light_Class functions */
void Light_Class::Create()
{
	pLight = new D3DLIGHT9();
	memset(pLight,0,sizeof(D3DLIGHT9));
}

void Light_Class::Free()
{ 
	if(pLight) delete pLight; 
}
/* Light_Controller_Class functions */
void Light_Controller_Class::Init(int MaxLights)
{
	pLights = new Light_Class*[MaxLights];
	Max_Lights = MaxLights;
}
void Light_Controller_Class::Add_Light()
{
	pLights[Num_Lights]->Create();
	Num_Lights++;
}
void Light_Controller_Class::PointLight(int Num, D3DXVECTOR3 pos)
{
	//D3DLIGHT_POINT          = 1,
	//D3DLIGHT_SPOT           = 2,
	//D3DLIGHT_DIRECTIONAL    = 3,
	pLights[Num]->Get_Light()->Type = D3DLIGHT_POINT;
	pLights[Num]->Get_Light()->Attenuation0 = 0.1f;
	pLights[Num]->Get_Light()->Diffuse.r = 1.0f;
	pLights[Num]->Get_Light()->Diffuse.g = 1.0f;
	pLights[Num]->Get_Light()->Diffuse.b = 1.0f;
	pLights[Num]->Get_Light()->Position = pos;
	pLights[Num]->Get_Light()->Range=200.0f;
	g_D3D_Struct->g_pD3DDevice->SetLight( Num, pLights[Num]->Get_Light() );
	g_D3D_Struct->g_pD3DDevice->LightEnable( Num, TRUE ); 
	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}
void Light_Controller_Class::Free()
{
	for( int i=0; i<Num_Lights; ++i )
	{
		if(pLights[i])
		{
			pLights[i]->Free();
		}
	}
}