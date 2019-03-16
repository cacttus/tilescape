/*
	Commondef - common definitions
	aug 23 2007
	*/

#ifndef __COMMDEF__H__
#define __COMMDEF__H__
/*---------------------------
	Tile Controller flags
---------------------------*/
#define FLAG_TL 0x80000000L // - Raise Top Left vert
#define FLAG_TR 0x40000000L
#define FLAG_BL 0x20000000L
#define FLAG_BR 0x10000000L
#define FLAG_TBM 0x08000000L // - top border mesh (there is no mesh there)
#define FLAG_RBM 0x04000000L // - right border mesh
#define FLAG_BBM 0x02000000L // - bottom border mesh
#define FLAG_LBM 0x01000000L // - left border mesh
#define FLAG_T_T 0x00800000L	// - tile is top of mesh (border)
#define FLAG_T_R 0x00400000L // - tile is right of mesh
#define FLAG_T_B 0x00200000L // - tile is bottom of mesh
#define FLAG_T_L 0x00100000L // - tile is left of mesh

/*---------------------
     Object flags
---------------------*/
#define OBJECT_VISIBLE	0x00000001	//Object is visible or not
#define OBJECT_REDRAW	0x00000002	//Object can be redrawn
//#define OBJECT_OTHER	0x00000004	//Object is OTHER
#define OBJECT_PARENT	0x00000008	//Object is the parent of another object
#define OBJECT_CHILD	0x00000010	//Object is the child of a parent object
//#define				0x00000020	//Object is to be redrawn
#define OBJECT_RENDERED	0x00000040	//Object has been rendered
#define	OBJECT_UPDATE	0x00000080	//Object is to be updated
#define OBJECT_ISPLAYER	0x00000100	//Object is a player object
#define OBJECT_LOCALCTL	0x00000200	//Object is controlled by the local user
#define OBJECT_CHKCOLLI	0x00000400	//Object is to be checked for collisions
#define OBJECT_MOVING	0x00000800	//Object is moving

/*-----------------------
	Buttons
-----------------------*/

/* Button States */
#define	BST_UP			0
#define	BST_ROLLOVER	1
#define	BST_DOWN		2

/* Button Type flags */
#define BTN_PUSHSTATE	0x00000001	//If pushed down, it stays there
#define BTN_NOROLLOVER	0x00000002	//The button does not have a rollover state
#define BTN_DISABLED	0x00000004	//Button is Disabled
#define BTN_INVISIBLE	0x00000008	//Button is not visible.
#define BTN_SWITCHSTATE	0x00000010	//Button cannot be depressed, but can be pressed

/* SWITCHSTATE groups */
#define SWITCH_GROUP0	0x00010000 //Switchstate group 0
#define SWITCH_GROUP1	0x00020000 
#define SWITCH_GROUP2	0x00040000
#define SWITCH_GROUP3	0x00080000

/*------------------------------
	Button Definitions 
---------------------------*/
/* Buttons */
#define BTN_DIGTOOL				1000
#define BTN_PATHTOOL			2000


/*DIGTOOL sub-buttons*/
#define BTN_RAISEHILL			1100
#define BTN_RAISELAND			1200
#define BTN_RAISELANDPLUS		1210
#define	BTN_RAISELANDMINUS		1220
#define BTN_RAISEALL			1300

/*PATHTOOL sub-buttons*/

/*------------------------
	Interface
------------------------*/

/* Global mode states */
#define GLOBAL_DIGTOOL			1000
#define GLOBAL_PATHTOOL			2000


/* DIG TOOL states */
#define DIGTOOL_RAISELAND		1100
#define DIGTOOL_RAISEHILL		1200
#define DIGTOOL_RAISEALL		1300

/* PATH TOOL states */

/*--------------------------
	Object Identifyers
--------------------------*/
/* Large Dodads */
#define OBJECT_DODADS_LARGE				10000
#define OBJECT_MULTI_SPHERE				10001
#define OBJECT_SPRING0					10002

/* Small Dodads */
#define OBJECT_DODADS_SMALL				30000

/*------------------------- //Texture Identifyers can be any number that a DWORD can hold.
	Texture Identifyers
-------------------------*/
//-----------------Object Textures
#define TEXID_DEFAULT				99999
#define TEXID_SPRING				10000

//-----------------Tile Textures
#define TEXID_TILE_GRASS			20000
#define TEXID_BASE_ROCK				20100

















































/*
//copies data byte - for - byte
void Quick_Mem_Copy(void* _dest, void* _src, int csize)
{
	__asm
	{
		//PUSHAD
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi


		//XOR ebx, ebx		;clear ebx register
		MOV ecx, csize		;move csize into ecx reg
		MOV esi, [_src]	
		MOV edi, [_dest]
		INVD  ; invalidate cache
	CTT:	
		SUB ecx, 1//3
		CMP ecx, 0
		JLE EXIT

		MOV eax, dword ptr [esi]
		//MOV ebx, dword ptr [esi+1]
		//MOV edx, dword ptr [esi+2]

		MOV dword ptr [edi], eax
		//MOV dword ptr [edi+1], ebx
		//MOV dword ptr [edi+2], edx

		ADD esi, 1//3
		ADD edi, 1//3

		JMP	CTT//jump back up
	EXIT:
		//POPAD
		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}*/


#endif
