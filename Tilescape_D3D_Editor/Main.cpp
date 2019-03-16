/*------------------------------
|	tile engine
|	main source
|
|	Derek Page Jul ??, 2007
|-----------------------------*/
#pragma pack(4)	//align all the structures
#define _CRT_SECURE_NO_DEPRECATE

/*--------------------
|		Include
|---------------------*/

/* Global */
#include "InitD3D.h"

/* Def */
#include "KEY_CODES.h"
#include "Commdef.h"
#include "Filedef.h"

/* Camera */
#include "Helicam_Class.h"

/* Interface */
#include "Button_Class.h"
#include "Tile_Picker_Class.h"
#include "Button_Functions.h"

/* Control */
#include "Texture_Controller.h"
#include "Object_Controller.h"
#include "Material_Controller_Class.h"
#include "Light_Controller.h"

/* TEST */
#include "Path_Class.h"
#include "Shop_Object.h"
#include "Tile_Controller_Class.h"

/*------------------- 
|		TEST
|-------------------*/

/*--------------------------
|		Structures
|--------------------------*/
Helicam_Class				* g_Camera =			new Helicam_Class();				// - Controls the camera
//Dig_Tool_Class				* g_DigToolClass =		new Dig_Tool_Class();			// - Controls the DIG TOOL

//Button_Controller_Class		* g_ButtonController =	new Button_Controller_Class();		// - Controls rendering and updating buttons
Tile_Controller_Class		* g_TileController =	new Tile_Controller_Class();
Object_Controller_Class		* g_ObjectController =	new Object_Controller_Class();		// - Controls Objects
Texture_Controller_Class	* g_TextureController = new Texture_Controller_Class();		// - Controls Textures
Mesh_Controller_Class		* g_MeshController =	new Mesh_Controller_Class();		// - Controls Meshes

Frustum_Class				* g_Frustum =			new Frustum_Class();				// - Controls the frustum

Path_Class					* g_TestPathClass =		new Path_Class();					//Controls the PATH TOOL
Tile_Picker_Class			* g_TilePicker =		new Tile_Picker_Class();			// - Draws the lines on top of tiles


/*
	Globals
------------------*/
bool	g_FullScreen = false;
int		g_Screen_Width = 800;
int		g_Screen_Height = 600;

int		g_GridBufLen = 0;

POINT*	g_CursorPos;
POINT*	g_Last_CursorPos;

bool	Draw_Faces = 1;
bool	Draw_Bases = 1;


/*
	Init
-----------------*/
void g_Init()
{

	g_CursorPos = new POINT();
	g_Last_CursorPos = new POINT();

	g_Camera->Init( D3DXVECTOR3(0.0f,0.0f,0.0f ) );

	g_TilePicker->Init(g_TileController);
	//Button::Init_Button_Menu(g_ButtonController, g_Screen_Width, g_Screen_Height);//Initializes all of the buttons
	
	g_TextureController->Init();
	
	g_TileController->Init2( 32, g_TextureController, g_Frustum );
	
	g_MeshController->Init( g_TextureController, g_Frustum );

	g_ObjectController->Init( g_TextureController, g_MeshController );
	g_ObjectController->Add_Object( FILE_MODEL_SHOP_HDSTAND, OBJECT_SPRING0 );

	//g_TestPathClass->Init( 64, "c:\\documents and settings\\default\\desktop\\path_test.tga", g_TileInfo );
}
bool TEST_KEYDOWN=false;
void g_Update()
{
	//if (Keydown(VK_UP)) { g_TileController->TestRaiseVerts(); }
	*g_Last_CursorPos = *g_CursorPos;
	GetCursorPos( g_CursorPos );

	/* Frustum */
	g_Frustum->Update();

	/* Camera */
	g_Camera->Update(g_Screen_Width, g_Screen_Height);
	
	g_Camera->Update_Mouse(g_Screen_Width, g_Screen_Height);

	//g_TileInfo->pTiles->CheckAllTilesVisible(g_Screen_Width, g_Screen_Height, g_TileInfo);

	//g_DigToolClass->Update(g_CursorPos, g_TileInfo, g_TilePicker, g_Screen_Width, g_Screen_Height);

	//g_ButtonController->Update( g_CursorPos->x, g_CursorPos->y );

	//Some short code to save a couple of frames
	if( (g_CursorPos->x != g_Last_CursorPos->x) || 
		(g_CursorPos->y != g_Last_CursorPos->y))
	{
		////////-----------------------------------------------------BUG - need to check if the mouse button is down, so we don't change tiles
		// New tile picking procedure 
		if(!Keydown(VK_LBUTTON))
		{
			g_TileController->PickTile( g_CursorPos, g_TilePicker->pHitStruct );
			//if( g_TileInfo->PickedTile!=-1 && g_TileInfo->PickedTile!=65535 )//Check for bad (no) tile
			{
				g_TilePicker->Update();
			}
		}
	}
	if(g_TilePicker->pHitStruct!=0)
	{
		if( Keydown(VK_UP) && (TEST_KEYDOWN==0) )
		{ 
			g_TileController->RaiseHillTile( g_TilePicker->pHitStruct ); 
			TEST_KEYDOWN=1;
		} else TEST_KEYDOWN=0;
		if( Keydown(VK_DOWN) && (TEST_KEYDOWN==0) )
		{ 
			g_TileController->RaiseSingleTile( g_TilePicker->pHitStruct ); 
			TEST_KEYDOWN=1;
		} else TEST_KEYDOWN=0;
	}
	//Should put any POST tile selection stuff here
	//if(Keydown(VK_LBUTTON))
	//g_TestPathClass->Set_Path( g_TileInfo->PickedTile );

}
void g_Render()
{

	g_D3D_Struct->g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff0000ff, 1.0f, 0); 
	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	//-----

	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );//Cull clockwise verts
	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_MULTISAMPLEMASK, 0xff00ffff );

	g_D3D_Struct->g_pD3DDevice->BeginScene( ); 
	g_D3D_Struct->g_pD3DDevice->SetFVF( FVF_DESCRIPTOR );

	//g_MeshController->Render( OBJECT_SPRING0, &D3DXVECTOR3(20.0f, 0.0f, 20.0f ) );
	g_ObjectController->Render( OBJECT_SPRING0 );

	g_TileController->Render( );

	g_TilePicker->Render();

	//Buttons
	//g_ButtonController->Render();

	g_D3D_Struct->g_pD3DDevice->EndScene( ); 
	HRESULT hr=g_D3D_Struct->g_pD3DDevice->Present( NULL, NULL, NULL, NULL ); 
	g_D3D_Struct->CheckLostDevice( hr ); 

}
void g_Cleanup()
{
	delete g_CursorPos;
	delete g_Last_CursorPos;

	g_TilePicker->Free();
	delete g_TilePicker;

	//g_ButtonController->Free();
	//delete g_ButtonController;

	g_ObjectController->Free();
	delete g_ObjectController;

	g_TextureController->Free();
	delete g_TextureController;

	g_MeshController->Free();
	delete g_MeshController;

	/*--------------------TEST */
	g_TestPathClass->Free();
	delete g_TestPathClass;

	g_TileController->Free();
	delete g_TileController;

} 

LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	int MouseWheel_Cur = 0;
	int MouseWheel_Last = 0;
	switch(message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_KEYDOWN:
			if(wParam==VK_ESCAPE)
				PostQuitMessage(0);
			break;
		case 0x020A://WM_MOUSEWHEEL
			MouseWheel_Cur = HIWORD(wParam);//GET_KEYSTATE_WPARAM(wParam);
			if(MouseWheel_Cur>0 && MouseWheel_Cur < 60000) g_Camera->Zoom( 1.5f );
			else if(MouseWheel_Cur>60000) g_Camera->Zoom( -1.5f );
			//MouseWheel_Last = MouseWheel_Cur;
			//zDelta = HIWORD(wParam);//GET_WHEEL_DELTA_WPARAM(wParam);
		break;

		default: break;
	}
	return DefWindowProc (hWnd, message, wParam, lParam);
}
/*
	Windows
------------------*/
int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	HWND hWnd;
	WNDCLASSEX wcex;
	MSG msg;

	memset( &wcex, 0,  sizeof( WNDCLASSEX) );

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpszClassName = "WindowClass";

	RegisterClassEx(&wcex);
	DWORD ExStyle = 0;//WS_EX_TOPMOST;//WS_EX_CLIENTEDGE;//WS_EX_APPWINDOW
	DWORD Style = WS_POPUP;//WS_OVERLAPPEDWINDOW;
	hWnd = CreateWindowExA( ExStyle, "WindowClass", "Tilescape_D3D_Editor", Style, 
	0, 0, g_Screen_Width, g_Screen_Height, NULL, NULL, hInstance, NULL );

	ShowWindow(hWnd, nCmdShow);

	g_D3D_Struct->Init( hWnd, g_Screen_Width, g_Screen_Height, g_FullScreen );

	g_Init();

	ZeroMemory( &msg, sizeof(msg) );
	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else{
			g_Update();
			g_Render();
		}
	}
	g_Cleanup();
	g_D3D_Struct->Cleanup();
	delete g_D3D_Struct;
	return (int)msg.wParam;
}

