#ifndef __BASE__STRUCT__H__
#define __BASE__STRUCT__H__

/*
	Base Structure
	Holds polygon information 
	for tiles

---------------------------*/
struct Base_Struct 
{
	Base_Struct* Prev;
	Base_Struct* Next;
	int Material;
	int Height; //Crucial, height of the base
	Side_Struct Sides[4];
	Base_Struct(){ memset( this, 0, sizeof(Base_Struct) ); }
};

#endif