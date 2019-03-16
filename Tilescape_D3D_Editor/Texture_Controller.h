/*

	Class to control the use of texures and
	avoid loading multiple indices of the
	same data.
	Derek Page aug 29,2007

	note : objects can access their texture
	locations from an indexed location in 
	an array of texture strings.

	note: optimization: change the texture
	indexing method from an array, to a 
	linked .list
--------------------------------------------*/

#ifndef __TEXTURE__CONTROLLER__H__
#define __TEXTURE__CONTROLLER__H__

#include "InitD3D.h"
#include "commdef.h"

struct Texture_Object{

	Texture_Object* pNext;				//Pointer to next texture
	//Texture_Object* pPrev;
	IDirect3DTexture9* pD3DTexture;
	char* FileLocation;					//Important, to avoid duplicate textures.
	DWORD ID;
	int Ref_Count;						//number of objects referencing this texture

	Texture_Object(){memset(this,0,sizeof(Texture_Object));}
};

class Texture_Controller_Class
{
	Texture_Object* pTextures;						//Linked list of textures
	int					Num_Textures;
	int					_Ref;						//a reference value, that is default.  This is used as an identifier for textures that are created with a call to D3DXLoadMeshHierarchyFromX()
	DWORD				Assigned_Texture;			//the last texture that was assigned.

public:
	void Init( );								//Initialize
	void Set_Texture( DWORD ID );					//Set current texture :old: int Stage, D3DTEXTURESTAGESTATETYPE TexStageState, DWORD TexStageVal 

	DWORD Add_Texture(char* filename, DWORD id );	//Add a texture
	void Remove_Ref( DWORD Id );					//Removes a reference to a texture, when a texture's reference is 0 it is freed

	// eh - char* Traverse_Fileroot( char* root );	//if the texture cannot be loaded, this will attempt to traverse the files in a common texture file location

	void Free();									//Free Resources

	Texture_Controller_Class(){ memset(this,0,sizeof(Texture_Controller_Class)); }
};

#endif