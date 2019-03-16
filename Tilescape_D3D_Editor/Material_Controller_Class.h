/*
	Class to control the useage 
	and creation of materials.

-----------------------------------*/

#ifndef __MATERIAL__CONTROLLER__CLASS__H__
#define __MATERIAL__CONTROLLER__CLASS__H__

#include "InitD3D.h"

class Material_Controller_Class{

	D3DMATERIAL9**	pMaterials;		//The Materials
	int Max_Materials;				//Maximum materials that can be held
	int Num_Materials;				//Total number of materials in buffer

public:
	void Init(int MaxMaterials);	//Initialize the structure
	void Add_Material();			//Add a material
	void Set( int Num ){ g_D3D_Struct->g_pD3DDevice->SetMaterial( pMaterials[Num]); } //Set the material of index 'Num'

	void StdShadow(int Num);		//Set material Num to be a standard shadow
	void Shiny(int Num);			//Set material Num to be a shiny surface

	void Free();					//Free resources

	Material_Controller_Class(){ memset(this,0,sizeof(Material_Controller_Class)); }

};



/*
	Material_Controller functions
-----------------------------------*/
void Material_Controller_Class::Init(int MaxMaterials)
{
	pMaterials = new D3DMATERIAL9*[MaxMaterials];
	Max_Materials=MaxMaterials;
}
void Material_Controller_Class::Add_Material()
{
	pMaterials[Num_Materials] = new D3DMATERIAL9();
	Num_Materials++;
}
void Material_Controller_Class::StdShadow(int Num)
{
	if(pMaterials[Num]!=0)
	{
		pMaterials[Num]->Ambient.r = 0.5f;
		pMaterials[Num]->Ambient.g = 0.5f;
		pMaterials[Num]->Ambient.b = 0.5f;
		pMaterials[Num]->Ambient.a = 1.0f;

		pMaterials[Num]->Diffuse.r = 0.2f;
		pMaterials[Num]->Diffuse.g = 0.2f;
		pMaterials[Num]->Diffuse.b = 0.2f;
		pMaterials[Num]->Diffuse.a = 1.0f;

		pMaterials[Num]->Emissive.r = 0.0f;
		pMaterials[Num]->Emissive.g = 0.0f;
		pMaterials[Num]->Emissive.b = 0.0f;
		pMaterials[Num]->Emissive.a = 0.0f;

		pMaterials[Num]->Power = 50.0f;

		pMaterials[Num]->Specular.r = 0.0f;
		pMaterials[Num]->Specular.g = 0.0f;
		pMaterials[Num]->Specular.b = 0.0f;
		pMaterials[Num]->Specular.a = 0.0f;
	}
}
void Material_Controller_Class::Shiny(int Num)
{
	if(pMaterials[Num]!=0)
	{
		pMaterials[Num]->Ambient.r = 0.8f;
		pMaterials[Num]->Ambient.g = 0.8f;
		pMaterials[Num]->Ambient.b = 0.8f;
		pMaterials[Num]->Ambient.a = 1.0f;

		pMaterials[Num]->Diffuse.r = 0.5f;
		pMaterials[Num]->Diffuse.g = 0.5f;
		pMaterials[Num]->Diffuse.b = 0.5f;
		pMaterials[Num]->Diffuse.a = 1.0f;

		pMaterials[Num]->Emissive.r = 0.2f;
		pMaterials[Num]->Emissive.g = 0.2f;
		pMaterials[Num]->Emissive.b = 0.2f;
		pMaterials[Num]->Emissive.a = 0.0f;

		pMaterials[Num]->Power = 50.0f;

		pMaterials[Num]->Specular.r = 0.3f;
		pMaterials[Num]->Specular.g = 0.3f;
		pMaterials[Num]->Specular.b = 0.3f;
		pMaterials[Num]->Specular.a = 1.0f;
	}
}
void Material_Controller_Class::Free()
{
	for( int i=0; i<Num_Materials; ++i )
	{
		if(pMaterials[i]) delete pMaterials[i];
	}
}


#endif
