/*==================================
#
#	New bitmap loading routine.
#
#	Derek Page Mar 14, 2007 4:54p
#
#	Stored as RGBA (byte,byte,byte,byte)
#
#
#=================================*/

#ifndef __BITMAP_LOAD2_HEAD__
#define __BITMAP_LOAD2_HEAD__

//#pragma message( "LoadBitmap: Deprecated, use LoadBitmap2 instead" )

#include <windows.h> 
#include <fstream>

using namespace std;

struct Win_Bitmap{

	unsigned int Data_Length;	//Length of data (in bytes)
	unsigned char* Data;		//Pointer to data	
	int Width;					//Width of image
	int Height;					//Height of image
	int BPP;					//Bits per pixel

	/* Functions */
	bool Load( char* file );				//Load bitmap from a file
	void AddAlpha( unsigned char AR, unsigned char AG, unsigned char AB ); //add an alpha channel (make the colorAR,AG,AB have full alpha)
	void AddGradAlpha( unsigned char A );	//??? dont know
	bool CheckSize(void);					//doesn't work...
	void HFlip();							//Flip the bitmap horizontally
	void VFlip();							//Flip the bitmap vertically
	void Rotate180();						//rotate bitmap 180deg
	void FlipBR();							//Flip-Flop blue and red
	void FlipGR();							//Flip-Flop green and red		
	void FlipBG();							//Flip-Flop blue and green							
	void ColorSwap(unsigned char c1, unsigned char c2); //flip flop two colors
	void freemem();							//Free memory
	Win_Bitmap(){ memset(this,0,sizeof(Win_Bitmap)); }

};

#endif