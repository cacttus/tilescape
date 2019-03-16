#include "Object_Class.h"

void Object_Class::Init( DWORD id )
{
	ID = MeshID = id;
}
/*

	Render the object at it's position
	in the world.

-----------------------------------------*/
void Object_Class::Render( Texture_Controller_Class* pTextureController, Mesh_Controller_Class* pMeshController )
{

	pMeshController->Render( MeshID, &Pos );

}
/*
	Add an identifyer of a texture
	the identifyer specifies the
	identity of a texture in
	the Texture_Controller_Class.
	Parms: 
		Tex_ID = the unique identifyer;
		StageState = the d3d flag of the state type;
		StageVal = the value of the StageState flag
----------------------------------*/

/*

	Add a child to an object
	- Returns true if success
	- Returns false if failure

-----------------------------------------------*/
BOOLEAN Object_Class::Add_Child(Object_Class* Object)
{

	if(Num_Children==0)
	{
		Children = new Object_Class*[1];
		Children[1] = Object;
		Num_Children=1;	
	}
	else
	{
		Object_Class** TEMP = new Object_Class*[Num_Children+1];
		__try
		{ 	//Copy the child data into a newly allocated array
			//sizeof(int) -- copying pointers, not the actual data
			memcpy(TEMP,Children,sizeof(int)*Num_Children);
		}
		__except(EXCEPTION_ACCESS_VIOLATION == GetExceptionCode())
		{
  			return FALSE;
		}
		__try
		{
			delete [] Children;		//Delete the old array
		}
		__except(EXCEPTION_ACCESS_VIOLATION == GetExceptionCode())
		{
			return FALSE;
		}
		Children = TEMP;
	}
	//Flags |= OBJECT_PARENT;
	return TRUE;
}
/*
 	Start an object moving along
	the xz plane
---------------------------------------------------*/
BOOLEAN	Object_Class::Set_Movement( float speed, float XZDir )
{
	
	Speed.x = sin( XZDir ) * speed;
	Speed.z = cos( XZDir ) * speed;
	if( (Speed.x != 0.0f) && (Speed.y!= 0.0f) && (Speed.z!= 0.0f) )
	//Flags |= OBJECT_MOVING;
	return TRUE;
}

BOOLEAN	Object_Class::Set_Movement( float speed, float XZDir, float YZDir )
{
	
	Speed.x = sin( XZDir ) * speed;
	Speed.y = cos( YZDir ) * speed;
	Speed.z = cos( XZDir ) * speed;
	if( (Speed.x != 0.0f) && (Speed.y!= 0.0f) && (Speed.z!= 0.0f) )
	//Flags |= OBJECT_MOVING;
	return TRUE;
}

void Object_Class::Free()
{
	
}