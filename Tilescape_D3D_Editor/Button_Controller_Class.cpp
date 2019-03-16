#include "Button_Controller_Class.h"
#include "Button_Class.h"
#include "LoadBitmap2.h"

/*------------------------------------
|	
|	Button_Controller_Class Functions
|
----------------------------------*/

//The image can't be greater than 256x256
void Button_Controller_Class::Init( char* FileName, int MaxButtons, int Screen_Width, int Screen_Height )
{
	//get the back buffer
	//call back buffer's Update() fn
	Sw = Screen_Width;
	Sh = Screen_Height;

	Max_Buttons = MaxButtons;
	Num_Buttons = 0;

	Buttons = new Button_Class[MaxButtons];
	Button_States = new int[MaxButtons];
	
	//Load the button image
	Button_Bitmap = new Win_Bitmap();
	Button_Bitmap->Load(FileName);
	Button_Bitmap->AddAlpha(255,0,255);
	Button_Bitmap->VFlip();


	//Obtain a pointer to the back buffer
	g_D3D_Struct->g_pD3DDevice->GetBackBuffer( 0, //chain ??
								 0, //BackBuffer Index (0 for first one)
								 D3DBACKBUFFER_TYPE_MONO,
								 &pBackBuffer );

	//Create the surface to blit to
	HRESULT res;
	res = g_D3D_Struct->g_pD3DDevice->CreateOffscreenPlainSurface( //Create the Temporary surface
		Screen_Width,Screen_Height, 
		D3DFMT_A8R8G8B8, 
		D3DPOOL_SYSTEMMEM, 
		&pD3DSurfaceTemp, 
		NULL ); 
	if(res!=D3D_OK) Fatal_Error("Failed to create surface TEMP. ");

}
/*
	Update all the buttons based on the cursor posititon
------------------------------------------------------------*/
int Button_Controller_Class::Update(int mx, int my)
{

	bool down = ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) ? 1 : 0;

	for( int i=0; i<Num_Buttons; ++i )
	{
		if( Buttons[i].Get_CanDraw() )
		{
			Buttons[i].Update( mx, my, down );
		}
	}
	
	return 0;

}
/*
	Render all the buttons to the screen
	:copies from the button spritemap to the temporary surface
	:then from the temp surface to the back buffer
------------------------------------------------------*/
void Button_Controller_Class::Render()
{
	/*-- Global variables --*/
	HRESULT res=0;
	D3DLOCKED_RECT d3dlr;
	/* Set blending states 
	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, 255 );
	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	//destination blend factor
	g_D3D_Struct->g_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//g_D3D_Struct->g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
*/
	int Ix=0, Iy=0, Iw=0; //the surface image position, width (x,y,width)
	int Off_0=0, Off_1=0;

	for( int i=0; i<Num_Buttons; ++i )
	{
		if(Buttons[i].Get_CanDraw() && Buttons[i].In_Use ) // && Buttons[i].Get_Dirty() 
		{

			Ix = Buttons[i].Get_Surface_Rect()->left;
			Iy = Buttons[i].Get_Surface_Rect()->top;
			Iw = Button_Bitmap->Width;
			pBackBuffer->LockRect( &d3dlr, NULL, 0);
				for(int j=(Buttons[i].Get_Screen_Rect()->top); j<(Buttons[i].Get_Screen_Rect()->top)+(Buttons[i].Get_Height()); j++)//y
				{
					for(int k=(Buttons[i].Get_Screen_Rect()->left); k<(Buttons[i].Get_Screen_Rect()->left)+(Buttons[i].Get_Width()); k++)//x
					{
						if(Button_Bitmap->Data[(((Iy*Iw)+Ix)*4)+3]==255)
						{
							*((char*)d3dlr.pBits + (((j*Sw)+k)*4) + 0) = Button_Bitmap->Data[(((Iy*Iw)+Ix)*4)+2]; //B
							*((char*)d3dlr.pBits + (((j*Sw)+k)*4) + 1) = Button_Bitmap->Data[(((Iy*Iw)+Ix)*4)+1]; //G
							*((char*)d3dlr.pBits + (((j*Sw)+k)*4) + 2) = Button_Bitmap->Data[(((Iy*Iw)+Ix)*4)+0]; //R
							*((char*)d3dlr.pBits + (((j*Sw)+k)*4) + 3) = Button_Bitmap->Data[(((Iy*Iw)+Ix)*4)+3]; //A
						}
						Ix++;
						
					}
					Ix=Buttons[i].Get_Surface_Rect()->left;
					Iy++;
				}

			pBackBuffer->UnlockRect();

		}
	}
}
/**/
void Button_Controller_Class::Free()
{
/*	if(pD3DSurface)
	{
		pD3DSurface->Release();
		pD3DSurface=0;
	}*/
	if(Button_Bitmap)
	{
		Button_Bitmap->freemem();
	}
	//free the resource
	if(pBackBuffer)
	{
		pBackBuffer->Release();
		pBackBuffer=0;
	}
	if(pD3DSurfaceTemp)
	{
		pD3DSurfaceTemp->Release();
		pD3DSurfaceTemp=0;
	}
}
/*
	Add a button
	Parms:
		x:		x pos on screen
		y:		y pos on screen
		xoff:	offset to the first tile (in pixels)
		yoff:	y offset to the first tile (in pixels)
		w:		width of button
		h:		height of button
		ID:		unique identifyer to be used by the program
		act:	activation function pointer
		deact:	deactivation function pointer
		flags:	creation flags
*/
void Button_Controller_Class::AddButton(int x, int y, int xoff, int yoff, int w, int h, int ID, void Act(int ButtonID, Button_Controller_Class* bcs), void Deact(int ButtonID, Button_Controller_Class* bcs), DWORD flags)	//add a button
{
	for( int i=Num_Buttons; i<Max_Buttons; ++i )
	{
		if(Buttons[i].In_Use==false)
		{
			Buttons[i].Initialize(x,y,xoff,yoff,w,h, ID, Act, Deact, this, flags);//change
			i=Max_Buttons;
			Num_Buttons++;
		}

	}

}
Button_Class* Button_Controller_Class::GetButtonFromID( int ID )
{
	for( int i=0; i<Num_Buttons; ++i )
	{
		if(ID==Buttons[i].Get_ID())
		{
			return &Buttons[i];
		}
	}
	return 0;//return 0 if nothing
}
void Button_Controller_Class::AddChild(int ID, int childID)		//Adds a child to the button with id ID
{
	GetButtonFromID(ID)->Add_Child( GetButtonFromID(childID) );
}