/*-----------------------------------------
#
#	Class to create buttons from a single
#	texture sheet. (256x256)
#
#	File Format:
#	[ up ] [down ] [over ] [ up ] [down ] [over ]
#	[ up ] [down ] [over ] [ up ] [down ] [over ] ...
#
#
-------------------------------------------*/



#ifndef __BUTTON__CLASS__H__
#define __BUTTON__CLASS__H__

#include "InitD3D.h"
#include "Commdef.h"
#include "LoadBitmap2.h"
#include "Button_Controller_Class.h"

class Button_Controller_Class;
//abstract class to derive all 3d rendering from, mabe used later
/*
class tdclass abstract{
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Free() = 0;
};
*/



/*

	Button_Class

----------------------------------------*/

class Button_Class {
	//POINT			Pos;					//Button position on the screen
	//POINT			OffPos;					//position of button in the image
	int				ID;						//Unique identifyer of the button
	int				Width;
	int				Height;
	RECT			Def;					//Default rectangle (first image) to reference from
	RECT			Rect;					//Rectangluar area of the button on the surface
	RECT			Screen;					//position on the screen

	DWORD			dwFlags;				//flags that specify the button type
	int				State;					//State 0=Up, 1=Rollover, 2=Down, 3=Disabled
	bool			Enabled;				//if button can be drawn, processed
	bool			Dirty;					//if the button is dirty( needs to be redrawn )
	int				Currently_Pressed;		//if button is currently pressed down
	int				Num_Children;			//Number of children
	Button_Class**	Children;				//Pointers to button children
	Button_Class*	Parent;					//Pointer to button parent

	Button_Controller_Class*	pController;//pointer to the controller

	void (*Activate_Function)(int ButtonID, Button_Controller_Class* bcs);			//Callback when the button is pushed
	void (*Deactivate_Function)(int ButtonID, Button_Controller_Class* bcs);		//Callback when button is deactivated

	void MakeRect( int x, int y, int ox, int oy, int w, int h );					//instantiates the rectangle

	void SetState(int state);				//sets the state of the button and changes it's image

public:

	bool In_Use;							//if it is in use (or can be reused)
	void Initialize(int x, int y, int ox, int oy, int w, int h, int id, void Act(int ButtonID, Button_Controller_Class* bcs), void Deact(int ButtonID, Button_Controller_Class* bcs),Button_Controller_Class* BCR, DWORD flags );
	int Update( int MouseX, int MouseY, bool Down );	
	
	void Add_Child(Button_Class* child);	//Add the child 'child' to the children
	void EnableChildren();					//Enable and show the button's children 
	void DisableChildren();					//Disable and hide the children

	void SwitchState(int ID);				//depresses all buttons that are of type "SwitchState" and presses in only ID
	void ForceState(int State);				//Forces the button into a state without executing the handling callback
	void FuncState(int State);				//Forces the button into a state with executing the handling callback

	void AddMode(int modeID, int newx, int newy); //adds a new mode to the button id is the identifyer, newx,y are the new locations of the sprite

	int GetState(){ return State; }			//Gets the state that a button is in

	POINT	Get_Pos(){ POINT p; p.x=Screen.left; p.y=Screen.top; return p; }
	int		Get_ID(){ return ID; }
	int		Get_Width(){ return Width; }
	int		Get_Height(){ return Height; }
	RECT*	Get_Surface_Rect(){ return &Rect; }
	RECT*	Get_Screen_Rect(){ return &Screen; }
	bool	Get_CanDraw(){ return Enabled; }
	bool	Get_Dirty(){ return Dirty; }
	DWORD	Get_Flags(){ return dwFlags; }
	Button_Class* Get_Parent(){ return Parent; }

	void	Set_CanDraw( bool val ){ Enabled=val; }
	void	Set_Parent( Button_Class* parent ){ Parent = parent; }

	Button_Class(){memset(this,0,sizeof(Button_Class));}
	
};



#endif