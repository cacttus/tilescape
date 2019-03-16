#include "Button_Functions.h"

int g_User_State = 0;	//declared externally in button_Functions.h

namespace Button
{

	void Blank_Function(int ButtonID, Button_Controller_Class* bcs){;}//blank, test function

/*
---------------------------DIG TOOL
*/
	
	void Button_Global_Deact(int ButtonID, Button_Controller_Class* bcs)//call when user un-presses a global butotn
	{
		g_User_State = 0; //the user is in no state now
	}
	void Button_Digtool_Down(int ButtonID, Button_Controller_Class* bcs)//Handles when the dig tool button is pressed
	{
		g_User_State = GLOBAL_DIGTOOL;
		//Make the children visible
		bcs->GetButtonFromID(ButtonID)->EnableChildren();
	}
	void Button_Digtool_Up(int ButtonID, Button_Controller_Class* bcs)//Handles when the dig tool button is pressed up
	{
		g_User_State = 0;
		bcs->GetButtonFromID(ButtonID)->DisableChildren();
	}
	void Button_RaiseHill_Down(int ButtonID, Button_Controller_Class* bcs)
	{
		bcs->GetButtonFromID(ButtonID)->Get_Parent()->SwitchState(ButtonID);
		DigToolState = DIGTOOL_RAISEHILL;
	}
	void Button_RaiseLand_Down(int ButtonID, Button_Controller_Class* bcs)
	{
		bcs->GetButtonFromID(ButtonID)->Get_Parent()->SwitchState(ButtonID);
		bcs->GetButtonFromID(ButtonID)->EnableChildren();
		DigToolState = DIGTOOL_RAISELAND;
	}
	void Button_RaiseLand_Up(int ButtonID, Button_Controller_Class* bcs)
	{
		bcs->GetButtonFromID(ButtonID)->DisableChildren();
	}
	void Button_RaiseAll_Down(int ButtonID, Button_Controller_Class* bcs)
	{
		bcs->GetButtonFromID(ButtonID)->Get_Parent()->SwitchState(ButtonID);
		DigToolState = DIGTOOL_RAISEALL;
	}
	void Button_RLP_Down( int ButtonID, Button_Controller_Class* bcs )
	{
		//bcs->GetButtonFromID(ButtonID)->Get_Parent()->
		if(DigTool_RaiseLandRadius<5) DigTool_RaiseLandRadius++;
	}
	void Button_RLM_Down( int ButtonID, Button_Controller_Class* bcs )
	{
		if(DigTool_RaiseLandRadius>1) DigTool_RaiseLandRadius--;
	}
/*
	-------------------------------- PATH TOOL
*/
	void Button_PathTool_Down(int ButtonID, Button_Controller_Class* bcs)//Handles when the dig tool button is pressed
	{
		g_User_State = GLOBAL_PATHTOOL;
		bcs->GetButtonFromID(ButtonID)->EnableChildren();
	}
	void Button_PathTool_Up(int ButtonID, Button_Controller_Class* bcs)//Handles when the dig tool button is pressed up
	{
		g_User_State = 0;
		bcs->GetButtonFromID(ButtonID)->DisableChildren();
	}


/*-------------------------------
|	Initialize all the buttons
-------------------------------*/
void Init_Button_Menu(Button_Controller_Class* pBcc, int sw, int sh )
{
	pBcc->Init( "Buttons\\Dig_Tool.bmp", 10, sw, sh );
	//	  AddButton( x,y,   xoff,yoff,   w,h,   ID,   DownCallback,   UpCallback,   FLAGS );
/* BTN_DIGTOOL */
	pBcc->AddButton( 20,0, 0,54, 31,22, BTN_DIGTOOL, &Button::Button_Digtool_Down, &Button::Button_Digtool_Up, BTN_NOROLLOVER | BTN_PUSHSTATE);
	{
		pBcc->AddButton( 20,21, 0,76, 31,22, BTN_RAISELAND, &Button::Button_RaiseLand_Down, &Button::Button_RaiseLand_Up, BTN_NOROLLOVER | BTN_SWITCHSTATE | SWITCH_GROUP0);
		pBcc->AddChild( BTN_DIGTOOL, BTN_RAISELAND ); //add btn_raiseland to btn_Digtool's children
		{
			pBcc->AddButton( 55,24, 0,38, 16,16, BTN_RAISELANDPLUS, &Button::Button_RLP_Down, &Button::Blank_Function, BTN_NOROLLOVER );
			pBcc->AddChild( BTN_RAISELAND, BTN_RAISELANDPLUS );

			pBcc->AddButton( 0,24, 32,38, 16,16, BTN_RAISELANDMINUS, &Button::Button_RLM_Down, &Button::Blank_Function, BTN_NOROLLOVER );
			pBcc->AddChild( BTN_RAISELAND, BTN_RAISELANDMINUS );
		}//BTN_RAISELAND

		pBcc->AddButton( 20,42, 0,98, 31,22, BTN_RAISEHILL, &Button::Button_RaiseHill_Down, &Button::Blank_Function, BTN_NOROLLOVER | BTN_SWITCHSTATE | SWITCH_GROUP0 );
		pBcc->AddChild( BTN_DIGTOOL, BTN_RAISEHILL ); 
	
		pBcc->AddButton( 20,63, 0,120, 31,22, BTN_RAISEALL, &Button::Button_RaiseAll_Down, &Button::Blank_Function, BTN_NOROLLOVER | BTN_SWITCHSTATE | SWITCH_GROUP0 );
		pBcc->AddChild( BTN_DIGTOOL, BTN_RAISEALL ); 
	}//BTN_DIGTOOL

/* BTN_PATHTOOL */
	pBcc->AddButton( 80,0, 0,142, 31,22, BTN_PATHTOOL, &Button::Button_PathTool_Down, &Button::Button_PathTool_Up, BTN_NOROLLOVER | BTN_PUSHSTATE);


}

};