#ifndef __SIDE__STRUCT__H__
#define __SIDE__STRUCT__H__

#include "polygon.h"
#include <memory.h>
struct Side_Struct{
	Polygon* tri[2];
	int Mode;//poly mode 0=.. 1=.` 2=`. 3=`` where 3 is a square side
	Side_Struct(){memset(this,0,sizeof(Side_Struct));}
};

#endif