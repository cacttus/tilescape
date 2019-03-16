#include "Helicam_Class.h"

void Helicam_Class::Init(D3DXVECTOR3 Lookat)
{
	Rho = 15.0f;
	//Phi = D3DXToRadian(70.0f);		//70 degrees
	Theta = D3DXToRadian(45.0f);	//45 degrees
	Height = 10.0f;
	
	lookat = Lookat;

	//Calculate the ratio between height and radius
	pos.x = lookat.x + Rho * cos(Theta);
	pos.y = lookat.y + Height;
	pos.z = lookat.z + Rho * sin(Theta);

	//Set the max and min zoom distances
	Zoom_MaxDist = 100.0f; 
	Zoom_MinDist = 1.0f;

	up.x=0.0f;up.y=1.0f;up.z=0.0f;
}
void Helicam_Class::Rotate(float Amt)
{
	Theta += D3DXToRadian(Amt);
	//Note: might want to carry the result
	//of the below operation over
	if(Theta>3.1415926535f*2.0f) Theta=0.0f;
	if(Theta<0.0f) Theta = 3.1415926535f*2.0f;
	//LastCamPt = CamPt;
	//Theta = (float)(theta*3.14159265)/180;
	pos.x =lookat.x+ Rho*cos(Theta);
	pos.z =lookat.z+ Rho*sin(Theta);
}
void Helicam_Class::Zoom( float Amt )
{
	//Amt = 4.0f;
	//Amt would be the amount to zoom in, if the camera were at the Max dist
	//it will slowly attenuate thereafter
	//range is sin 90 - 270
	//make sure that
	
	float NR = (Rho/Height)*Amt;
	float NH = (Height/Rho)*Amt;

	//Max_Zoom_Dist-Min_Zoom_Dist*
	//Amt*= Rho*100/Height;
	//NR*=Amt;
	//NH*=Amt;
	if( Rho + NR <=3.0f ) 
		Rho = 3.1f;
	else	
		Rho += NR;

	if( Height + NH <=3.1f ) 
		Height = 3.0f; 
	else
		Height += NH;

	pos.x = lookat.x + Rho * cos(Theta);
	pos.y = lookat.y + Height;
	pos.z = lookat.z + Rho * sin(Theta);
	return;
}
void Helicam_Class::Update(int w, int h)
{
	D3DXMATRIXA16 View, World;
	D3DXMATRIX Projection;

	if( Keydown( VK_UP) )Height+=0.1f;//test
if( Keydown( VK_DOWN) )Height-=0.1f;

	/* Get the shape of the window rectangle, this 
	is useful for different types of displays */
	RECT rc;
	GetWindowRect( g_D3D_Struct->g_hWnd, &rc );

	/* Reset the world matrix */
	D3DXMatrixIdentity(&World);
	g_D3D_Struct->g_pD3DDevice->SetTransform(D3DTS_WORLD, &World);

	/* Set the viewport matrix */
	D3DXMatrixLookAtLH( &View, &pos, &lookat, &up );
	//D3DXMatrixLookAtLH( &View, &D3DXVECTOR3(0.0f,0.0f,50.0f), &D3DXVECTOR3(0.0f,0.0f,0.0f), &D3DXVECTOR3(0.0f,1.0f,0.0f));
	g_D3D_Struct->g_pD3DDevice->SetTransform( D3DTS_VIEW, &View ); 

	/* Set the projection matrix */
	D3DXMatrixPerspectiveFovLH(&Projection, D3DX_PI/4, static_cast<float>(rc.right)/static_cast<float>(rc.bottom), 1.0f, 500.0f); 
	g_D3D_Struct->g_pD3DDevice->SetTransform( D3DTS_PROJECTION, &Projection ); 

}
void Helicam_Class::Update_Mouse(int w, int h)
{
	POINT MousePos;
	GetCursorPos( &MousePos );
	int ST = 3; //pixel border in which to move
	float StrafeAmt = 1.7f;
	float MoveAmt=0.08f;
	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		Rotate( (float)(MouseLastPos.x - MousePos.x) );
	}

	if( (MousePos.x <ST) && (MousePos.x>-1) )	//move left
	{
		D3DXVECTOR3 Ortho = lookat-pos;
		D3DXVec3Cross( &Ortho, &Ortho, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
		D3DXVec3Normalize( &Ortho,  &Ortho );
		pos.x += Ortho.x * StrafeAmt;
		pos.z += Ortho.z * StrafeAmt;
		lookat.x += Ortho.x * StrafeAmt;
		lookat.z += Ortho.z * StrafeAmt;
	}
	if( (MousePos.x > w-ST) && (MousePos.x < w) )	//move right
	{
		D3DXVECTOR3 Ortho = lookat-pos;
		D3DXVec3Cross( &Ortho, &Ortho, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
		D3DXVec3Normalize(&Ortho,  &Ortho );
		pos.x = pos.x - (Ortho.x * StrafeAmt);
		pos.z = pos.z - (Ortho.z * StrafeAmt);
		lookat.x = lookat.x - (Ortho.x * StrafeAmt);
		lookat.z = lookat.z - (Ortho.z * StrafeAmt);
	}
	if( (MousePos.y <ST) && (MousePos.y > -1 ) )  	//move up
	{
		lookat.x -= (( lookat.x - (pos.x)) )*-MoveAmt; 
		lookat.z -= (( lookat.z - (pos.z)) )*-MoveAmt; 
		pos.x =lookat.x+ Rho*cos(Theta);
		pos.y =lookat.y+ Height;
		pos.z =lookat.z+ Rho*sin(Theta);
	}
	if( (MousePos.y > h-ST) && (MousePos.y < h) )	//move down
	{
		lookat.x += (( lookat.x - (pos.x)) )*-MoveAmt; 
		lookat.z += (( lookat.z - (pos.z)) )*-MoveAmt; 
		pos.x =lookat.x+ Rho*cos(Theta);
		pos.y =lookat.y+ Height;
		pos.z =lookat.z+ Rho*sin(Theta);
	}

	MouseLastPos = MousePos;
}
