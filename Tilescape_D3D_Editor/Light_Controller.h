/*

	Controller for light resources
	8-28-07

------------------------------------*/

#ifndef __LIGHT__CONTROLLER__CLASS__H__
#define __LIGHT__CONTROLLER__CLASS__H__

#include "InitD3D.h"

class Light_Class{
	D3DLIGHT9*		pLight;
	D3DXVECTOR3		Pos;
public:
	void Create();

	void		Set_Pos(D3DXVECTOR3 pos){Pos=pos;}
	D3DXVECTOR3 Get_Pos(){return Pos; }
	D3DLIGHT9* Get_Light(){ return pLight; }

	void Free();
	Light_Class(){ memset(this,0,sizeof(Light_Class)); }
};

class Light_Controller_Class{
	Light_Class**	pLights;
	int				Num_Lights;
	int				Max_Lights;
public:
	void Init( int MaxLights );
	void Add_Light();
	void Set( int Num ){ g_D3D_Struct->g_pD3DDevice->SetLight( Num, pLights[Num]->Get_Light() );}	//Set the light of index number 'Num'
	void PointLight( int Num, D3DXVECTOR3 pos );
	void Free();

};

#endif