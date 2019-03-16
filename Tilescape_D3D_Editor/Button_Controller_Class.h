#ifndef __BUTTON__CONTROLLER__CLASS__H__
#define __BUTTON__CONTROLLER__CLASS__H__

class Button_Class;
#include "InitD3D.h"
#include "LoadBitmap2.h"
/*

	Button_Controller_Class

---------------------------------------*/
class Button_Controller_Class 
{
	IDirect3DSurface9*	pBackBuffer;	//Pointer to the backbuffer
	//IDirect3DSurface9*	pD3DSurface;	//The loaded image to copy portions to Temp
	IDirect3DSurface9*	pD3DSurfaceTemp;//The temporary surface to copy to the Backbuffer
	Win_Bitmap*			Button_Bitmap;	//The image that the buttons are on
	Button_Class*		Buttons;		//An array of button objects
	int*				Button_States;	//Corresponding array of the states of buttons
	int					Num_Buttons;	//Total number of used buttons
	int					Max_Buttons;	//Maximum number of buttons that can be held
	D3DXIMAGE_INFO		Image_Info;		//image information
	int					Sw;				//screen width
	int					Sh;				//screen height
public:

	void Init( char * FileName, int MaxButtons, int Screen_Width, int Screen_Height  );
	
	Button_Class* GetButtonFromID( int ID );

	int Update(int mx, int my);	//returns if the cursor is over a button
	void Render();
	void Free();

	bool ButtonAction(int ID);			//Gets if the button has specified an action
	void AddChild(int ID, int childID);	//Adds a child to the button with id ID

	void AddButton(int x, int y, int xoff, int yoff, int w, int h, int ID, void Act(int ButtonID, Button_Controller_Class* bcs), void Deact(int ButtonID, Button_Controller_Class* bcs), DWORD flags);	//add a button

};
#endif