#include "Polygon_Class.h"
Polygon_Class::Polygon_Class(){memset(this,0,sizeof(Polygon_Class));}
void Polygon_Class::operator=(Polygon_Class p){

	p1=p.p1;
	p2=p.p2;
	p3=p.p3;

}