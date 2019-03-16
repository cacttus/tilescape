/*-------------------------------------------
|	The first file in the new DirectX
|	game library.
|	Contains a class to initialize and handle
|	the DirectX api.
|
|	Derek Page
|
|	Wed, August 1, 2007 3:12p
---------------------------------------------*/

#ifndef __INIT__D3D__H__
#define __INIT__D3D__H__
#pragma once
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
//#include <d3dx9.h>
//#include <dxerr.h> //DXTRACE_MSG(str) - prints a string to the debug window DXGetErrorString(hr) - gets the error string from an hresult
#include "Vertex.h"
#include "string.h"

#define Keydown(KEY) ((GetAsyncKeyState(KEY) & 0x8000) ? 1 : 0)

/*-----------------------------
	Square of a number
-----------------------------*/
#define SQUARE(x) x*x
#define SQR(x) SQUARE(x)
#define EQU(x,y) (x==y ? 1 : 0)
#define EQUXCHG(x,y) (x==y ? x : 0)

/*-----------------------------
	Checks if x is between 
	single bounds. ( -1<x<1 )
	Returns true if it is.
-----------------------------*/
#define BOUNDS(x) (x>1 ? 0 : (x<-1 ? 0 : 1 ) )

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "kernel32.lib")
#pragma comment (lib, "user32.lib")

/*---- Reports errors ----*/
void ErrorCheck( HRESULT hr, DWORD line, char* file );

void User_Fatal_Error( char* c );

void NonFatal_Error( HRESULT hr, DWORD line, char* file );
void Fatal_Error( HRESULT hr, DWORD line, char* file );

void NonFatal_Error( char* c );
void Fatal_Error( char* c );

/*---- Direct3D Handling Structure ----*/
struct D3D_Struct 
{
	/*---- Global D3D ----*/
	IDirect3D9*				g_pD3D;			//global direct3d instance
	IDirect3DDevice9*		g_pD3DDevice;	//global device
	IDirect3DSurface9*		Z_Buffer;		//global zbuffer	

	int						ScreenWidth, ScreenHeight;

	HWND g_hWnd;							//global window

	D3DPRESENT_PARAMETERS d3dpp;			//global device params (in case it's lost)

	bool					bSoftwareProcessing;	//if using software processing

	//Checks if the device was lost and reports an error
	void CheckLostDevice( HRESULT hr );

	//Direct3D Globals
	void Init(HWND hWnd, int cWidth, int cHeight, bool Fullscreen);
	void BeginRender();
	void EndRender();
	void Cleanup();

	D3D_Struct();

};

extern struct D3D_Struct* g_D3D_Struct;

#endif