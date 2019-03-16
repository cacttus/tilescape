/*--------------------------------------
|
|	Global object structure
|	Derek Page August 29, 2007
|
|	note: objects should use linked lists
------------------------*/


#ifndef __OBJECT__CLASS__H__
#define __OBJECT__CLASS__H__

#include <math.h>
#include "InitD3D.h"

class Object_Controller_Class;

/*
	Mesh Header
-----------------------------*/
#include "Boundary_Class.h"
#include "Mesh_Controller.h"

/*
	Object Structure
--------------------------------*/
class Object_Class//36 bytes
{
	friend class				Object_Controller_Class;

	Object_Class*				pNext;				// - The next object in the linked list.

	Object_Class*				Parent;				// - Parent object, if any.
	Object_Class**				Children;			// - Child Objects if any.
	
	D3DXVECTOR3					Pos;				// - Position.
	D3DXVECTOR3					Rotation;			// - Rotation
	D3DXVECTOR3					Speed;				// - Movement.
	
	//DWORD						Flags;				// - Flags, see below. --obsolete
	DWORD						ID;					// - Unique identifyer of the object.
	DWORD						MeshID;				// - Mesh Identifier, an object has only one mesh.

	WORD						Num_Children;		// - Number of child objects.

public:

	void Init( DWORD id );												//Load a mesh from a .X file
	void Render( Texture_Controller_Class* pTextureController,
		Mesh_Controller_Class* pMeshController );						//Render the object
	void Update( void );												//updates the object relative to it's bounding volume
	void Free( );														//Free resources.

	/* Position parameters */
	D3DXVECTOR3	Get_Pos(){ return Pos; }
	D3DXVECTOR3	Get_Speed(){ return Speed; }

//	DWORD		Get_Flags(){ return Flags; }
	DWORD		Get_ID(){ return ID; }

	BOOLEAN		Add_Child( Object_Class* Object );
	BOOLEAN		Del_Child( Object_Class* Object );

	BOOLEAN		Set_Movement( float speed, float XZDir );
	BOOLEAN		Set_Movement( float speed, float XZDir, float YZDir );

	Object_Class(){ memset(this,0,sizeof(Object_Class) ); }
	
};

#endif
