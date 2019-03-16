#include "Tile_Class.h"
/*-----------------------------------------
	This will generate a newly created
	tile indices based on default values.
	e.g. it is flat and the tiles
	have the id of grass and
	bases have the id of rock.

-----------------------------------------*/
void Tile_Class::Gen_20( /*int W, int BH, */DWORD IND, DWORD &REL_IND )
{
		memset(this, 0, sizeof(Tile_Class));
		
		Index = IND;
		bCanDraw = TRUE;

		TopPolys[0].p1 = (WORD)(REL_IND);
		TopPolys[0].p2 = (WORD)(REL_IND+2);
		TopPolys[0].p3 = (WORD)(REL_IND+3);

		TopPolys[1].p1 = (WORD)(REL_IND);
		TopPolys[1].p2 = (WORD)(REL_IND+1);
		TopPolys[1].p3 = (WORD)(REL_IND+2);
		REL_IND+=4;
	
		BasePolys[0].p1 = (WORD)(REL_IND);
		BasePolys[0].p2 = (WORD)(REL_IND+2);
		BasePolys[0].p3 = (WORD)(REL_IND+3);

		BasePolys[1].p1 = (WORD)(REL_IND);
		BasePolys[1].p2 = (WORD)(REL_IND+1);
		BasePolys[1].p3 = (WORD)(REL_IND+2);
		REL_IND+=4;

		BasePolys[2].p1 = (WORD)(REL_IND);
		BasePolys[2].p2 = (WORD)(REL_IND+2);
		BasePolys[2].p3 = (WORD)(REL_IND+3);

		BasePolys[3].p1 = (WORD)(REL_IND);
		BasePolys[3].p2 = (WORD)(REL_IND+1);
		BasePolys[3].p3 = (WORD)(REL_IND+2);
		REL_IND+=4;

		BasePolys[4].p1 = (WORD)(REL_IND);
		BasePolys[4].p2 = (WORD)(REL_IND+2);
		BasePolys[4].p3 = (WORD)(REL_IND+3);

		BasePolys[5].p1 = (WORD)(REL_IND);
		BasePolys[5].p2 = (WORD)(REL_IND+1);
		BasePolys[5].p3 = (WORD)(REL_IND+2);
		REL_IND+=4;

		BasePolys[6].p1 = (WORD)(REL_IND);
		BasePolys[6].p2 = (WORD)(REL_IND+2);
		BasePolys[6].p3 = (WORD)(REL_IND+3);

		BasePolys[7].p1 = (WORD)(REL_IND);
		BasePolys[7].p2 = (WORD)(REL_IND+1);
		BasePolys[7].p3 = (WORD)(REL_IND+2);
		REL_IND+=4;

		/* just shit */
		TopPolys[0].bCanDraw =			TRUE;
		TopPolys[0].bDrawn =			FALSE;
		TopPolys[0].dwAttribGroup =		TEXID_TILE_GRASS;

		TopPolys[1].bCanDraw =			TRUE;
		TopPolys[1].bDrawn =			FALSE;
		TopPolys[1].dwAttribGroup =		TEXID_TILE_GRASS;

		for( int j=0; j<8; ++j )
		{
			BasePolys[j].bCanDraw = TRUE;
			BasePolys[j].bDrawn = FALSE;
			BasePolys[j].dwAttribGroup = TEXID_BASE_ROCK;//Base texture groups start at 25
		}
		
}