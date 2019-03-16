/*
	Namespace for button functions.
	- ButtonID is the id of the calling object
	- bcs is a pointer to the controller class that holds all the buttons
	- to access any button use bcs->GetButtonFromID(ButtonID)->...

*/
#include "commdef.h"
#include "Button_Class.h"
#include "Tile_Picker_Class.h"

#include "Dig_Tool_Class.h"

extern int g_User_State ;//GLOBAL_DIGTOOL;

namespace Button
{

	void Blank_Function(int ButtonID, Button_Controller_Class* bcs);//blank, test function

	void Button_Global_Deact(int ButtonID, Button_Controller_Class* bcs);
	void Button_Digtool_Down(int ButtonID, Button_Controller_Class* bcs);
	void Button_Digtool_Up(int ButtonID, Button_Controller_Class* bcs);
	void Button_RaiseHill_Down(int ButtonID, Button_Controller_Class* bcs);
	void Button_RaiseLand_Down(int ButtonID, Button_Controller_Class* bcs);
	void Button_RaiseLand_Up(int ButtonID, Button_Controller_Class* bcs);
	void Button_RaiseAll_Down(int ButtonID, Button_Controller_Class* bcs);
	void Button_RLP_Down( int ButtonID, Button_Controller_Class* bcs );
	void Button_RLM_Down( int ButtonID, Button_Controller_Class* bcs );
/*-------------------------------
|	Initialize all the buttons
-------------------------------*/
void Init_Button_Menu(Button_Controller_Class* pBcc, int sw, int sh );


};//end of namespace Button
