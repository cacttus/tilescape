/*--------------------------------------
	Indexed polygon class
	Derek Page sept 9, 2007
--------------------------------------*/

#ifndef __IPOLYGON__CLASS__H__
#define __IPOLYGON__CLASS__H__

class IPolygon16_Class {
public:
	unsigned short p1;		// - offset of p1
	unsigned short p2;		// - offset of p2
	unsigned short p3;		// - offset of p3
	bool bCanDraw;			// - if it can be drawn
	bool bDrawn;			// - if it has been drawn
	DWORD dwAttribGroup;	// - attribute group
};

class IPolygon32_Class {
public:
	unsigned int p1;		// - offset of p1
	unsigned int p2;		// - offset of p2
	unsigned int p3;		// - offset of p3
	bool bCanDraw;			// - if it can be drawn
	bool bDrawn;			// - if it has been drawn
	DWORD dwAttribGroup;	// - attribute group
};

#endif