#include "InitD3D.h"
#include "Object_Controller.h"
#include "Texture_Controller.h"

void Object_Controller_Class::Init( Texture_Controller_Class* ptc, Mesh_Controller_Class* pmc )
{
	pTextureController = ptc;
	pMeshController = pmc;
	pObjects = new Object_Class();

}
/*
	Add an object to the controller.

	UniqueID: is a value that is used to 
	retrieve the object from the structure,
	it should be defined like "#define OBJECT_FOO 1004"

	fileloc: is the location of the object's .X file
*/
void Object_Controller_Class::Add_Object( char* fileloc, DWORD UniqueID )
{
	Object_Class* pTemp = pObjects;
	Object_Class* pPrev = pObjects;
	while( pTemp != 0 )
	{
		pPrev = pTemp;
		pTemp = pTemp->pNext;
	}
	pTemp = new Object_Class();
	pPrev->pNext = pTemp;
	pTemp->Init( UniqueID );
	pMeshController->Add_Mesh( fileloc, UniqueID );
	//check if the mesh is already loaded.

	//add a reference to the mesh


}
void Object_Controller_Class::Render( DWORD UniqueID )
{
	Object_Class* pTemp = pObjects;

	while( pTemp!= 0)
	{
		if(pTemp->ID ==UniqueID)
		{
			pTemp->Render( pTextureController, pMeshController );
			return;
		}

		pTemp = pTemp->pNext;
	}

}
void Object_Controller_Class::Free()
{
	Object_Class* pTemp = pObjects;
	Object_Class* pTemp2;
	while(pTemp!=0)
	{
		pTemp2 = pTemp;
		pTemp = pTemp->pNext;
		pTemp2->Free();
		delete pTemp2;
	}
	pObjects = 0;
}
Object_Class* Object_Controller_Class::Get_Object( DWORD ID )
{
	Object_Class* pTemp = pObjects;
	while(pTemp!=0)
	{
		if(pTemp->ID==ID) return pTemp;
	}
	return 0;
}