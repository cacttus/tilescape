/*

	Class controls the loading and unloading
	of meshes.
	-Meshes support animation
	-All meshes are loaded from directX (.X) files.

---------------------------*/


#ifndef __MESH__CONTROLLER__CLASS__H__
#define __MESH__CONTROLLER__CLASS__H__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "D3D_Mesh.h"
#include "Frustum_Class.h"

/*
	Class to control the creation
	deletion and rendering of
	meshes.
--------------------------------*/
class Mesh_Controller_Class
{
	friend class	Object_Class;
	friend class	Object_Controller_Class;

	D3D_Mesh*		pMeshes;		//Linked list of meshes
	int				Num_Meshes;		//Total number of meshes being used
	bool			Initialized;

	Texture_Controller_Class* p_TextureController;
	Frustum_Class*			  pFrustum;

	void Add_Mesh( char* file, DWORD id );		//-- Should not be called alone(should make private), the ref would be incramented without a binding object and the mesh would never free!!!!!!!

	void Render( DWORD id, D3DXVECTOR3* Pos );					//-- You should not have to call this function, the object structure should implement it( should make private ). Render the mesh with the ID id.
	void Render( DWORD id, D3DXMATRIX* Mat );	
	void Remove_Ref( DWORD id );				//Removes a reference counter from a mesh	

public:
	void Init(Texture_Controller_Class* tc, Frustum_Class* pf);	//Initialize the structure
	void Update();

	void Free();

	Mesh_Controller_Class(){memset(this,0,sizeof(Mesh_Controller_Class));}

};

#endif
