#include "InitD3D.h"


struct D3D_Struct* g_D3D_Struct = new D3D_Struct();

/*---- Errrors ----*/

void ErrorCheck( HRESULT hr, DWORD line, char* file )
{
	if(FAILED(hr)) Fatal_Error(hr, __LINE__, __FILE__ );
}

void NonFatal_Error( HRESULT hr, DWORD line, char* file )//use hr,__LINE__,__FILE__
{
	char c[500]; 
	char l[30];
	memset(c,0,200);memset(l,0,30);
	_itoa_s(line,l,30,10) ;
	strcat_s(c, 500, "ERROR:\nFile: ");
	strcat_s(c, 500, file );
	strcat_s(c, 500, "\nLine: ");
	strcat_s(c, 500, l);
	strcat_s(c, 500, "\nType: ");
	strcat_s(c, 500, DXGetErrorString(hr));
	strcat_s(c, 500, "\nDescription: ");
	strcat_s(c, 500, DXGetErrorDescription(hr));
	MessageBox( NULL, c, "ERROR", MB_OK | MB_ICONEXCLAMATION );
}
void User_Fatal_Error( char* c )
{
	MessageBoxA( NULL, c, "Fatal Error!", MB_OK | MB_ICONEXCLAMATION );
	exit(1);
}
void Fatal_Error( HRESULT hr, DWORD line, char* file )//use hr,__LINE__,__FILE__
{
	char c[500]; 
	char l[30];
	memset(c,0,200);memset(l,0,30);
	_itoa_s(line,l,30,10) ;
	strcat_s(c, 500, "ERROR:\nFile: ");
	strcat_s(c, 500, file );
	strcat_s(c, 500, "\nLine: ");
	strcat_s(c, 500, l);
	strcat_s(c, 500, "\nType: ");
	strcat_s(c, 500, DXGetErrorString(hr));
	strcat_s(c, 500, "\nDescription: ");
	strcat_s(c, 500, DXGetErrorDescription(hr));
	MessageBox( NULL, c, "ERROR", MB_OK | MB_ICONEXCLAMATION );
	exit(0);
}

void NonFatal_Error( char* c )
{
	MessageBoxA(NULL, c, "ERROR", MB_OK | MB_ICONEXCLAMATION);
}


void Fatal_Error(char* c)
{
	MessageBoxA(NULL, c, "ERROR", MB_OK | MB_ICONEXCLAMATION);
	exit(0);
}

/*---- Direct3D Handling Structure ----*/

//Checks if the device was lost and reports an error
void D3D_Struct::CheckLostDevice( HRESULT hr ){
	if(FAILED(hr))
	{
		if(hr==D3DERR_DEVICELOST)
		{
			//Fatal_Error("Device Lost");
			//new - get the device back
			if(FAILED(g_pD3DDevice->Reset(&d3dpp)))
				User_Fatal_Error("Internal Error:\n The rendering device has been lost.");
		}

	}
}

//Direct3D Globals
void D3D_Struct::Init(HWND hWnd, int cWidth, int cHeight, bool Fullscreen)//Initialize Direct3D
{
	g_hWnd = hWnd;

	ScreenWidth = cWidth; ScreenHeight = cHeight;

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	memset(&d3dpp, 0, sizeof(d3dpp));

	d3dpp.Windowed = !Fullscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = cWidth;
	d3dpp.BackBufferHeight = cHeight;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;//_2_SAMPLES, _NONMASKABLE, _4_SAMPLES .._16_SAMPLES

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; 
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;


	int res;
	//first try to create in hardware, if fail then do software
	res = g_pD3D->CreateDevice(  D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL,  //where direct3d features are implemented, might wanna try D3DDEVTYPE_HAL for better cards or d3ddevtype_ref for older/slower cards
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, //flags, ex. D3DCREATE_MULTITHREADED, D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, 
		&g_pD3DDevice );
	bSoftwareProcessing=FALSE;
	//If this fails, chances are software rendering must be used.
	//If that doesn't work, the program exits.
	if(res!=D3D_OK)
	{
		res = g_pD3D->CreateDevice(  D3DADAPTER_DEFAULT, 
			D3DDEVTYPE_REF, 
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, 
			&g_pD3DDevice );
		if(res!= D3D_OK )
		{
			User_Fatal_Error("I'm having a problem running the game!\n(Mabe you should update your graphics card.)");
		}
		bSoftwareProcessing=TRUE;
	}

	//Create a ZBuffer
	g_pD3DDevice->CreateDepthStencilSurface( cWidth, cHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &Z_Buffer, NULL ); 
	
	g_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	return;
}

void D3D_Struct::BeginRender()//Begins a rendering sequence
{
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff0000ff, 1.0f, 0); 
	g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_pD3DDevice->BeginScene( ); 
	g_pD3DDevice->SetFVF( FVF_DESCRIPTOR );
}
void D3D_Struct::EndRender()//Ends the rendering sequence
{
	g_D3D_Struct->g_pD3DDevice->EndScene( ); 

	HRESULT hr=g_D3D_Struct->g_pD3DDevice->Present( NULL, NULL, NULL, NULL ); 
	CheckLostDevice( hr );
}
void D3D_Struct::Cleanup()//Cleans up all the trash
{
	if(g_pD3D)
	{ 
		g_pD3D->Release(); 
		g_pD3D=NULL;
	}
	if(g_pD3DDevice)
	{ 
		g_pD3DDevice->Release(); 
		g_pD3DDevice=NULL;
	}
	if(Z_Buffer)
	{
		Z_Buffer->Release();
		Z_Buffer=NULL;
	}
}

D3D_Struct::D3D_Struct(){memset(this,0,sizeof(D3D_Struct));}


