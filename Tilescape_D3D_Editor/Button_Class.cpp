#include "Button_Class.h"

/*--------------------------------------------
|
|	Button_Class functions
|
--------------------------------------------*/
void Button_Class::Initialize(int x, int y, int ox, int oy, int w, int h, int id, void Act(int ButtonID, Button_Controller_Class* bcs), void Deact(int ButtonID, Button_Controller_Class* bcs),Button_Controller_Class* BCR, DWORD flags )
{
	Enabled = true;
	State=0;
	In_Use=true;
	Currently_Pressed=false;
	ID = id;
	dwFlags = flags;
	Activate_Function = Act;
	Deactivate_Function = Deact;
	MakeRect(x,y,ox,oy,w,h);
	pController = BCR;
	Dirty=TRUE;
	Width = w;
	Height = h;
}
void Button_Class::MakeRect( int x, int y, int ox, int oy, int w, int h )//instantiates the rectangle
{
	Rect.top = oy; 
	Rect.left = ox; 
	Rect.right = ox+w; 
	Rect.bottom = oy+h;
	Def.top = oy; 
	Def.left = ox; 
	Def.right = ox+w; 
	Def.bottom = oy+h;
	Screen.top = y; 
	Screen.left = x; 
	Screen.right = x+w; 
	Screen.bottom = y+h;

}
void Button_Class::SetState(int state)//sets the state of the button and chagnes it's image
{
	int w = Def.right - Def.left;

	State = state;

	if(State==0)//down
	{
		Rect.left = Def.left; 
		Rect.top = Def.top; 
		Rect.bottom = Def.bottom; 
		Rect.right = Def.right;
	}
	if( !(dwFlags & BTN_NOROLLOVER) )
	{
		if(State==1)
		{
			Rect.left = Def.right; Rect.top = Def.top; 
			Rect.bottom = Def.bottom; Rect.right = Def.right+(Def.right-Def.left);
		}
	}
	if(State==2)//up
	{
		if( dwFlags & BTN_NOROLLOVER)
		{
			Rect.left = Def.right; Rect.top = Def.top; 
			Rect.bottom = Def.bottom; Rect.right = Def.right+(Def.right-Def.left);
		}
		else
		{
			Rect.left = Def.right+(Def.right-Def.left); Rect.top = Def.top; 
			Rect.bottom = Def.bottom; Rect.right = Def.right+(Def.right-Def.left)+(Def.right-Def.left);
		}
	}
}
/*
	Depress all buttons in the switchstate (like a windows option button frame) 
	'Group' besides the button with id 'ID'.
	Group(flag):	Option group. i.e. "SWITCH_GROUP0"
	ID:				Button to activate.
	Function: Loop through the parent button of this
	button and set all it's children except this, to the Up state.
*/
void Button_Class::SwitchState(int ID)
{
	DWORD Group = ( (dwFlags&SWITCH_GROUP0) ? SWITCH_GROUP0 : ( (dwFlags&SWITCH_GROUP1) ? SWITCH_GROUP1 : 
		((dwFlags&SWITCH_GROUP2) ? SWITCH_GROUP2 : ((dwFlags&SWITCH_GROUP3) ? SWITCH_GROUP3 : SWITCH_GROUP0 ))));

	for( int i=0; i<Num_Children; ++i )
	{
		if( (Children[i]->Get_Flags() & BTN_SWITCHSTATE) && (Children[i]->Get_Flags() & Group) )
		{
			if(Children[i]->Get_ID() != ID )
			{
				//Children[i]->ForceState(0);/////////////////////////////////Change
				Children[i]->FuncState( 0 );
			}
			else
			{
				//nothing this is the right button
			}
		}
	}
}
void Button_Class::ForceState(int State)
{
	this->SetState(State);
}
void Button_Class::FuncState(int State)
{
	this->SetState(State);
	if(State==0) this->Deactivate_Function( this->ID, this->pController );
	if(State==2) this->Activate_Function( this->ID, this->pController );
}
void Button_Class::Add_Child(Button_Class* child)
{
	if(Children==0)
	{
		Num_Children=1;
		Children = (Button_Class**) malloc( sizeof(int) ) ;
		Children[0] = child;
		Children[0]->Set_Parent( this );
	}
	else
	{
		Num_Children++;
		Children = (Button_Class**) realloc( Children, sizeof(int)*Num_Children );
		Children[Num_Children-1] = child;
		Children[Num_Children-1]->Set_Parent( this );
	}
}
void Button_Class::EnableChildren()
{
	for( int i=0; i<Num_Children; ++i )
	{
		if(Children[i]->GetState()==2) //down
			Children[i]->EnableChildren();
		Children[i]->Set_CanDraw(true);
	}
}
void Button_Class::DisableChildren()
{
	for( int i=0; i<Num_Children; ++i )
	{	
		Children[i]->DisableChildren();
		Children[i]->Set_CanDraw(false);
	}
}
/*-------------------------------------------
	Handle Button Pressing procedures.
-------------------------------------------*/
int Button_Class::Update(int MouseX, int MouseY, bool Down)
{
	if(!Enabled) return 0; //return if the button is invisible

	int Ret=0;

	Dirty=FALSE; //reset

	if( (MouseX>Screen.left) && (MouseY>Screen.top) && (MouseX<Screen.right) && (MouseY<Screen.bottom) )
	{
		Ret=1;
		if(Down)	//Left mouse button is pressed
		{
			if(!Currently_Pressed) //if it has not been pressed yet
			{

				Currently_Pressed=1;	//make it pressed
				if(GetState()==(0))		//if the button is currently up
				{
					SetState(2);										//make the button pressed down
					Activate_Function( this->ID, this->pController );	//execute some action
				}
				else if( (GetState()==2) && (dwFlags&BTN_PUSHSTATE) && (!(dwFlags&BTN_SWITCHSTATE)) )//if the button is down
				{
					//if(GetState()==(2))//The button was in a 'pushed' state now it must be raised back up
					//{
						SetState(0);
						Deactivate_Function(this->ID, this->pController);	// execute the deactivation
					//}
				}	
				Dirty=TRUE; //the button must be updated

			}
			else
			{
				//Set the pressed state to 2 meaning the user is holding the mouse down
				Currently_Pressed=2;
			}
		}
		else//Left mouse button is not pressed
		{
			Currently_Pressed=0;
			if( !(dwFlags&BTN_PUSHSTATE) && !(dwFlags&BTN_SWITCHSTATE) && GetState()!=0 )//the button does not have a 'pushed-in' state, raise it back up
			{
				SetState(0);
				Dirty=TRUE;
				Deactivate_Function(this->ID, this->pController);
			}
		}

	}//if( (X<Pos.x)...
	return Ret;
}	
