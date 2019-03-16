/*
	Class to control the creation
	rendering, useage, and resources
	of objects.

	Derek Page 8-29-07

-------------------------------*/

#ifndef __OBJECT__CONTROLLER__H__
#define __OBJECT__CONTROLLER__H__

#include "InitD3D.h"

#include "Object_Class.h"
#include "Mesh_Controller.h"

class Object_Controller_Class{

	Texture_Controller_Class*	pTextureController;		// - Pointer to tex controller.
	Mesh_Controller_Class*		pMeshController;		// - Pointer to the mesh controller.
//	Mesh_Controller_Class*		pMeshController;		// - Pointer to mesh controller.
	//Object_Class**				pObjects;				//array of object pointers.
	Object_Class*				pObjects;				// - Objects linked list.
	//int							Num_Objects;			// - Number of objects in pObjects.
	//int							Max_Objects;			// - Maximum objects createable.


public:

	void			Init( Texture_Controller_Class* ptc, 
							Mesh_Controller_Class* pmc );			//Initialize.
	void			Add_Object( char* fileloc, DWORD UniqueID );	//Add an object from 'fileloc' with identifyer 'UniqueID'.
	Object_Class*	Get_Object(DWORD ID);
	void			Update( DWORD UniqueID );						//Update an object or -1 for all.
	void			Render( DWORD UniqueID );						//Render the object with id ID;.
	void			RenderAll();									//Render all objects.
	void			Free();										//Free resources;.

};

#endif