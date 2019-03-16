#include "Texture_Controller.h"
/*
	Initialize the texture controller, 
	- OLD max_textures 
	are the maximum amount of textures to load.
------------------------------------------------------*/
void Texture_Controller_Class::Init(  )
{
	/* -- OLD array method
		Max_Textures = max_textures;
		pTextures = new Texture_Object*[Max_Textures];
		memset( pTextures, 0, sizeof(Texture_Object*)*Max_Textures );
	*/
	_Ref = 100000;	//instantiate _Ref
}
/*
	Set the current texture to be drawn with an object.
-----------------------------------------------------------*/
void Texture_Controller_Class::Set_Texture( DWORD ID )
{
	//if(Assigned_Texture==ID) //tile struct changes this!!
	//	return;

	if(ID==NULL)
	{
		g_D3D_Struct->g_pD3DDevice->SetTexture( 0, 0 );
	}
	else
	{
		Texture_Object* pTexPtr = pTextures;
		while( pTexPtr != 0 )
		{
			if( pTexPtr->ID == ID )
			{
				//device.RenderState.SourceBlend = Direct3D.Blend.SourceAlpha;
				//device.RenderState.DestinationBlend = Direct3D.Blend.InvSourceAlpha;
				//device.RenderState.AlphaBlendEnable = true;
				g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA ); 
				g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
				g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
				g_D3D_Struct->g_pD3DDevice->SetTexture( 0, pTexPtr->pD3DTexture );
				Assigned_Texture = pTexPtr->ID;
				return;
			}
			pTexPtr = pTexPtr->pNext;
		}
	}
/* //-- OLD array method
	if(ID==NULL)
	{
		g_D3D_Struct->g_pD3DDevice->SetTexture( 0, 0 );
	}
	else	//Search for the texture
	{
		for( int i=0; i<Num_Textures; ++i )
		{
			if(pTextures[i]->ID==ID)
			{
				g_D3D_Struct->g_pD3DDevice->SetTexture( 0, pTextures[i]->pD3DTexture );
			}
		}
	}
*/

}
/*
	Add a texture from the file "filename'
	returns the identifyer of the texture
	this is useful if TEXID_DEFAULT (9999) is given
	as a parameter and the calling function needs
	to know the generated texture.
---------------------------------------------*/
DWORD Texture_Controller_Class::Add_Texture( char* filename, DWORD id )
{
	HRESULT hr;

	Texture_Object* pTexPtr = pTextures;//pointer to use while running through the linked list

	while(pTexPtr!=0)
	{
		if(strcmp(pTexPtr->FileLocation, filename)==0)
		{
			pTexPtr->Ref_Count++;
			return pTexPtr->ID;
		}
		pTexPtr = pTexPtr->pNext;
	}


	/* -- OLD array method
	//Make sure that the texture is not already loaded
	for(int i=0; i<Max_Textures; ++i )
	{
		if(pTextures[i]!=0)//make sure it's not in use
		{
			if(strcmp(filename, pTextures[i]->FileLocation)==0)
			{
				//Texture is already loaded
				pTextures[i]->Ref_Count++;
				return pTextures[i]->ID;
			}
		}
	}
*/
	//Instantiate the list
	if(pTextures!=0)
	{
		Texture_Object* pLast=0;	//keep a pointer to the last texture

		pTexPtr = pTextures; //reset

		//Find a texture at address zero. (not being used)
		while(pTexPtr!=0)
		{
			pLast = pTexPtr;
			pTexPtr = pTexPtr->pNext;
		}

		//Append a new one to the list
		pTexPtr = new Texture_Object();
		if(pLast!=0) pLast->pNext = pTexPtr;
		pTexPtr->pNext = 0;
	}
	else
	{
		pTextures = new Texture_Object();
		pTexPtr = pTextures;
	}
	//Load the texture
	hr = D3DXCreateTextureFromFileExA( g_D3D_Struct->g_pD3DDevice, 
		filename, 
		D3DX_DEFAULT, D3DX_DEFAULT, //w,h
		D3DX_DEFAULT, //miplevels
		0,//D3DX_DEFAULT, //useage
		D3DFMT_UNKNOWN,	//format
		D3DPOOL_DEFAULT, 
		D3DX_FILTER_LINEAR,
		D3DX_DEFAULT,  //D3DX_FILTER_BOX is default
		0xffff00ff,			//filter color a,r,g,b
		NULL,
		NULL,
		&pTexPtr->pD3DTexture );
	
	if(FAILED(hr)) 
	{
		NonFatal_Error(hr, __LINE__, __FILE__); 
		return FALSE; //Could not load the texture
	}

	pTexPtr->Ref_Count++;
	pTexPtr->FileLocation = new char[strlen(filename)+1];
	memset(pTexPtr->FileLocation,0,strlen(filename)+1);
	memcpy(pTexPtr->FileLocation, filename, strlen(filename));

	if(id==TEXID_DEFAULT) //Sets an application-generated identifyer value.
	{
		pTexPtr->ID = _Ref;
		if(_Ref<100000000)//---------May cause a bug. You should really search for unused refs at this point, or even recalculate all the refs of all the textures
			_Ref++; 
		else 
			_Ref=0;
	}
	else //Sets a user-generated identifyer value.
	{
		pTexPtr->ID = id;
	}

	Num_Textures++;
	return pTexPtr->ID;


	/* -- OLD array method
	//Else load the texture
	if(Num_Textures<Max_Textures)
	{
		for(int i=0; i<Max_Textures; ++i )
		{
			if(pTextures[i]==0)
			{
				//Create the texture
				pTextures[i] = new Texture_Object();
				//Load the texture
 				hr = D3DXCreateTextureFromFileA( g_D3D_Struct->g_pD3DDevice, filename, &pTextures[i]->pD3DTexture );
				
				if(FAILED(hr)) 
				{
					NonFatal_Error(hr, __LINE__, __FILE__); 
					return FALSE; //Could not load the texture
				}

				pTextures[i]->Ref_Count++;


				if(id==TEXID_DEFAULT) //Sets an application-generated identifyer value.
				{
					pTextures[i]->ID = _Ref;
					if(_Ref<2000000000)//---------May cause a bug. You should really search for unused refs at this point, or even recalculate all the refs of all the textures
						_Ref++; 
					else 
						_Ref=0;
				}
				else //Sets a user-generated identifyer value.
				{
					pTextures[i]->ID = id;
				}

				Num_Textures++;
				return pTextures[i]->ID;
			}
		}
	}
*/
	return FALSE;//There are no more textures left!
}

/*-------------------
	Free resources.
-------------------*/
void Texture_Controller_Class::Free()
{
	//collapse the linked list
	Texture_Object* pTexPtr = pTextures;
	Texture_Object* pTemp;
	while(pTexPtr!=0)
	{
		pTemp = pTexPtr;	
		pTexPtr = pTexPtr->pNext;
		if(pTemp->FileLocation) delete [] pTemp->FileLocation;
		pTemp->pD3DTexture->Release();
		delete pTemp;
	}
	/* -- OLD array mode
	int i;
	for( i=0; i<Max_Textures; ++i )
	{
		if(pTextures[i])
		{ 
			pTextures[i]->pD3DTexture->Release();
			delete pTextures[i];
		}
	}
	*/
}

/*
	- Remove a reference to a texture.
	- When the reference count equals zero, 
	the texture is released.
	- If an object references a released 
	texture the texture must be reloaded.
--------------------------------------------*/
void Texture_Controller_Class::Remove_Ref( DWORD Id )
{
	Texture_Object* pTexPtr = pTextures;
	Texture_Object* pPrev;	//To patch up the list

	while(pTexPtr!=0)
	{
		if(pTexPtr->ID==Id)
		{
			pTexPtr->Ref_Count--;
			if(pTexPtr->Ref_Count<=0)
			{
				pPrev->pNext = pTexPtr->pNext;	//Patch up the list
				if(pTexPtr->FileLocation) delete [] pTexPtr->FileLocation;
				pTexPtr->pD3DTexture->Release(); //free the resource
				delete pTexPtr;
				pTexPtr=0;
			}
			return;
		}
		pPrev = pTexPtr;
		pTexPtr=pTexPtr->pNext;
	}

	/* -- OLD array mode
	for( int i=0; i<Max_Textures; ++i )
	{
		if(pTextures[i]!=0)
		{
			if(pTextures[i]->ID == Id)
			{
				pTextures[i]->Ref_Count--;
				if(pTextures[i]->Ref_Count<=0)
				{
					pTextures[i]->pD3DTexture->Release();
					delete pTextures[i];
					pTextures[i]=0;
				}
				i=Max_Textures;
			}
		}
	}
	*/
}