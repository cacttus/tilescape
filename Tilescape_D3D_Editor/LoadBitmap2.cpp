#include "LoadBitmap2.h"

/*================*/
/* Load the image */
/*================*/
bool Win_Bitmap::Load( char* file )
{
	/*==== Locals ====*/
	BITMAPINFOHEADER bih;
	BITMAPFILEHEADER bfh;
	fstream in;
	//unsigned int i, j;
	
	in.open( file, ios::binary | ios::in );
	
	if( !in.good() ) return 0;

	/*==== Store the headers ====*/
	in.read( (char*) &bfh, sizeof(BITMAPFILEHEADER) );
	in.read( (char*) &bih, sizeof(BITMAPINFOHEADER) );

	/*==== Store Variables ====*/

	Width =				bih.biWidth;
	Height =			bih.biHeight;
	Data_Length =		bih.biSizeImage;
	BPP =				bih.biBitCount;

	if( BPP == 0)
	{
		if( bih.biBitCount!=24 ){ in.close(); return 0; } //the image is not a 24 bit .bmp
		bih.biSizeImage = bih.biWidth * bih.biHeight * (BPP>>3);
	}

	/*==== Read in the image ====*/
	Data = new unsigned char[Data_Length];
	memset( Data, 0, Data_Length );
	in.read( (char*) Data, Data_Length );

	in.close();

	/*==== Flip, if reversed ====*/
	if( Height<0 ) VFlip();

	/*==== Flip-Flop the B and R values ====*/
	FlipBR();

	return 1;	
}
/* flip horizontally -- Tested, Works */
void Win_Bitmap::HFlip()
{

	unsigned char* Temp = new unsigned char[Data_Length];
	memcpy(Temp,Data,Data_Length);
	
	int i=0, j=0, k=Width;

	if(BPP/8==3)
	{
		for( i=0; i<Height; ++i)//for each row
		{
			//swap bytes
			for( j=0; j<Width; j++ )
			{
				Data[ (((i*Height)+j)*3) + 0  ] = Temp[ (((i*Height)+k)*3) + 0 ];
				Data[ (((i*Height)+j)*3) + 1  ] = Temp[ (((i*Height)+k)*3) + 1 ];
				Data[ (((i*Height)+j)*3) + 2  ] = Temp[ (((i*Height)+k)*3) + 2 ];
				k--;
			}
			k=Width;
		}
	}
	else if(BPP/8==4)
	{
		//use an int for faster code
		unsigned __int32* pointer = (unsigned int*)Data;
		unsigned __int32* tempptr = (unsigned int*)Temp;
		for( i=0; i<Height; ++i)//for each row
		{
			//swap bytes
			for( j=0; j<Width; j++ )
			{
				pointer[ (i*Height)+j  ] = tempptr[ (i*Height)+k ];
				k--;
			}
			k=Width;
		}
	}
	
	//Deallocate the temporary buffer
	delete Temp;

}
/* flip Vertically - works, tested*/
void Win_Bitmap::VFlip()
{

	unsigned char* Temp = new unsigned char[Data_Length];
	memcpy( Temp, Data, Data_Length );
	
	int i=0, j=0, k=Height;

	if(BPP/8==3)
	{
		for( i=0; i<Width; ++i)//for each row
		{
			//swap bytes
			for( j=0; j<Height; j++ )
			{
				Data[ (((j*Width)+i)*3) + 0  ] = Temp[ (((Width*Height)-((j+1)*Width)+i)*3) + 0 ];
				Data[ (((j*Width)+i)*3) + 1  ] = Temp[ (((Width*Height)-((j+1)*Width)+i)*3) + 1 ];
				Data[ (((j*Width)+i)*3) + 2  ] = Temp[ (((Width*Height)-((j+1)*Width)+i)*3) + 2 ];
				k--;
			}
			k=Height;
		}
	}
	else if( (BPP/8) == 4 )
	{
		//use an int for faster code
		unsigned __int32* pointer = (unsigned __int32*)Data;
		unsigned __int32* tempptr = (unsigned __int32*)Temp;

		for( i=0; i<Width; ++i )//for each row
		{
			//swap bytes
			for( j=0; j<Height; j++ )
			{
				pointer[ (j*Width)+i  ] = tempptr[ (Width*Height)-((j+1)*Width)+i ];
				k--;
			}
			k=Height;
		}

	}
	
	//Deallocate the temporary buffer
	delete Temp;

}
void Win_Bitmap::Rotate180()
{
	unsigned char* Temp = new unsigned char[Data_Length];
	memcpy( Temp, Data, Data_Length );
	
	int i=0, j=0;

	if(BPP/8==3)
	{
		/*
		for( i=0; i<Data_Length; i+=3 )
		{
			Data[i  ] = Temp[j  ];
			Data[i+1] = Temp[j+1];
			Data[i+2] = Temp[j+2];
			j-=3;
		}
		*/
	}
	else if(BPP/8==4)
	{
		//use an int for faster code
		unsigned __int32* pointer = (unsigned __int32*)Data;
		unsigned __int32* tempptr = (unsigned __int32*)Temp;
		for( i=0; i<Width; ++i)//for each column
		{
			//swap bytes
			for( j=0; j<Height; j++ )
			{
				pointer[ (j*Width)+i  ] = tempptr[ Width*Height-(j*Width)-i];
			}
		}
	}
	
	//Deallocate the temporary buffer
	delete Temp;

}
/*===========================*/
/* Add a transparent alpha   */
/*===========================*/
void Win_Bitmap::AddAlpha( unsigned char AR, unsigned char AG, unsigned char AB )
{
	int L, Len, n;
	unsigned char* B;

	if(BPP==32) return ;

	L = Data_Length/3;
	Len = Data_Length+L;
	B = new unsigned char[Len];//will hold temporary bitmap data

	/*==== Copy temporary data ====*/
	for( n=0; n<L; n++ )
	{
			B[(n*4)  ] = Data[n*3    ];
			B[(n*4)+1] = Data[(n*3)+1];
			B[(n*4)+2] = Data[(n*3)+2];

				//   red			green				blue				 this color will be the alpha
			if( B[(n*4)]==AR && B[(n*4)+1]==AG && B[(n*4)+2]==AB )		//a black transparent color//
				B[(n*4)+3]=0;//no alpha, see-through
			else 
				B[(n*4)+3]=255; //full alpha
	}

	/*==== Reallocate Materials ====*/
	//delete [] Data;
	Data = new unsigned char[Len];
	Data_Length = Len;
	memcpy( Data, B, Len );
	delete [] B;

	/*==== Set Variables ====*/
	BPP = 32;

}
/*
	Color Flipping Functions
	Exchange two of the three colors.
--------------------------------*/
void Win_Bitmap::FlipBR()
{
	unsigned int i=0;
	unsigned char TempByte;
	if( (BPP/8) == 3)
	{
		for( i=0; i<Data_Length; i+=3 )
		{
			TempByte=Data[i];
			Data[i]=	Data[i+2];		//B -> R
			//Data[i+1]=				//G
			Data[i+2]= TempByte;		//R -> B
		}
	}
	else if( (BPP/8) == 4)
	{
		for( i=0; i<Data_Length; i+=4 )
		{
			TempByte=Data[i];
			Data[i]=	Data[i+2];		//B -> R
			//Data[i+1]=				//G
			Data[i+2]= TempByte;		//R -> B
		}
	}
}
void Win_Bitmap::FlipGR()
{
	unsigned int i=0;
	unsigned char TempByte;

	if( (BPP/8) == 3)
	{
		for( i=0; i<Data_Length; i+=3 )
		{
			TempByte=Data[i];
			Data[i]=	Data[i+1];
			Data[i+1]=	TempByte;
		}
	}
	else if( (BPP/8) == 4)
	{
		for( i=0; i<Data_Length; i+=4 )
		{
			TempByte=Data[i];
			Data[i]=	Data[i+1];	
			Data[i+1]=	TempByte;
		}
	}
}
void Win_Bitmap::FlipBG()
{
	unsigned int i=0;
	unsigned char TempByte;

	if( (BPP/8) == 3)
	{
		for( i=0; i<Data_Length; i+=3 )
		{
			TempByte=Data[i+1];
			Data[i+1]=	Data[i+2];	
			Data[i+2]=	TempByte;		
		}
	}
	else if( (BPP/8) == 4)
	{
		for( i=0; i<Data_Length; i+=4 )
		{
			TempByte=Data[i+1];
			Data[i+1]=	Data[i+2];	
			Data[i+2]=	TempByte;
		}
	}
}
/*
	8-28-07
	Adds an actual alpha value, instead
	of just setting each Alpha value to 
	255

--------------------------------------*/
void Win_Bitmap::AddGradAlpha( unsigned char A )
{
	int Len;
	unsigned int n, L;
	unsigned char* B;

	if(BPP==32)
	{
		for( n=0; n<Data_Length; n+=4 )
			Data[n+3] = A;
	}
	else
	{

		L = Data_Length/3;
		Len = Data_Length+L;
		B = new unsigned char[Len];//will hold temporary bitmap data

		/*==== Copy temporary data ====*/
		for( n=0; n<L; n++ )
		{
				B[(n*4)  ] = Data[n*3    ];
				B[(n*4)+1] = Data[(n*3)+1];
				B[(n*4)+2] = Data[(n*3)+2];
				B[(n*4)+3]=A; //alpha
		}

		/*==== Reallocate Materials ====*/
		//delete [] Data;
		Data = new unsigned char[Len];
		Data_Length = Len;
		memcpy( Data, B, Len );
		delete [] B;

		/*==== Set Variables ====*/
		BPP = 32;
	}

}
/*================*/
/* Check the size */
/*================*/
bool Win_Bitmap::CheckSize( )
{
	//unsigned int SIZE;
	//int i;//, j;
//	unsigned char* B;

	/*==== Check that the image is not too big ====*/
	//GLint MaxSize;
	//int MaxSize;
	//glGetIntegerv( GL_MAX_TEXTURE_SIZE, &MaxSize );
	//if( Width>MaxSize || Height>MaxSize ) return 0;

	/*==== Check for a power of 2 boundary (256,512...) ====
	if( (Width%8!=0) || (Height%8!=0 ) )
	{
		// Pad Width with zeroes 
		if(Width%8!=0)
		{
			for( i=2; i<MaxSize; i*=2 )
			{		
				if(i>Width)
				{
					Width=i;
					i=MaxSize;
				}
			}
		}

		// Pad Height with zeroes 
		if(Height%8!=0)
		{
			for( i=2; i<MaxSize; i*=2 )
			{
				if(i>Height)
				{
						Height=i;
						i=MaxSize;
				}
			}
		}

		
//		SIZE = Width * Height * (BPP>>3);

		/*==== Realloc the data ====
		B = new unsigned char[ SIZE ];
		memset( B, 0, SIZE );
		memcpy( B, Data, Data_Length );
		delete [] Data;
		Data = new unsigned char[ SIZE ];
		memcpy( Data, B, SIZE );

		Data_Length = SIZE; 
		delete [] B;

		/*==== Flip, if reversed ====
		int j=Data_Length;
		B = new unsigned char[Data_Length];
		memcpy( B, Data, Data_Length );

		for( i=0; i<Data_Length; i+=3 )
		{
			Data[i  ] = B[j  ];
			Data[i+1] = B[j+1];
			Data[i+2] = B[j+2];
			j-=3;
		}

		delete [] B;
		*/
	//}
	return 1;
}

/*=============*/
/* Free Memory */
/*=============*/
void Win_Bitmap::freemem()
{
	if(Data!=0)	delete  Data;
}