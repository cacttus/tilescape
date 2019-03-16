/*
!!!!!!!!!!!!!!!!!!!!!!!!OBSOLETE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	DirectX Texture class
	Derek Page
	August 5, 2007
----------------------------------*/

#ifndef __TEXTURE__CLASS__H__
#define __TEXTURE__CLASS__H__
#define _CRT_SECURE_NO_DEPRECATE
#include "InitD3D.h"

//#pragma message( "---------------->WARNING: CLASS \"Texture_Class\" IS OBSOLETE. USE \"Texture_Controller.h\" INSTEAD.<----------------")

class Texture_Class
{

public:
	IDirect3DTexture9* d3dTexture;


	Texture_Class(){memset(this,0,sizeof(Texture_Class));}
	~Texture_Class(){;}
	void Load(char* fileloc, IDirect3DDevice9* device)
	{	
		HRESULT hr = D3DXCreateTextureFromFileA( device, fileloc, &d3dTexture );
		if(hr!=D3D_OK) Fatal_Error("Fail to load tex from old tex class.");
	}
	void Free()
	{
		if(d3dTexture)
		{
			d3dTexture->Release();
			d3dTexture = 0;
		}
	}
};
/*
!!!!!!!!!!!!!!!!!!!!!!!!OBSOLETE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

----------------------------------*/




#endif