#include <iostream>
#include <fstream>
#include <math.h>
//Y7EA0 Ordered Dithering
using namespace std;
typedef unsigned char   BYTE;   //  1 byte (0~255)
typedef unsigned short  WORD;   //  2 bytes (0~65536)
typedef unsigned long   DWORD;  //4 bytes (0~2^32 -1)

#pragma pack(push)	//store
#pragma pack(2)		//2-bytes aligned


struct INFO
{
    // BIT MAP FILE HEADER (14 bytes) from 16 reducing to 14
    /* Bitmap file format
    *
    * SECTION
    * Address:Bytes	Name
    *
    * HEADER:
    *	  0:	2		"BM" magic number
    *	  2:	4		file size
    *	  6:	4		junk
    *	 10:	4		Starting address of image data
    * BITMAP HEADER:
    *	 14:	4		header size
    *	 18:	4		width  (signed)
    *	 22:	4		height (signed)
    *	 26:	2		Number of color planes
    *	 28:	2		Bits per pixel
    *	[...]
    * [OPTIONAL COLOR PALETTE, NOT PRESENT IN 32 BIT BITMAPS]
    * BITMAP DATA:
    *	138:	X	Pixels
    */
    WORD bfType;          //BM -> 0x4d42 (19778)
    DWORD BfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
    // BIT MAP INFO HEADER(40 bytes)
    DWORD biSize;
    int biWidth;
    int biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
};

#pragma pack(pop)  	//restore



class Image
{
public:

    int height;
    int width;
    int rowsize;    // RGB -> 3 bytes(24 bits)
    BYTE* term;

    Image()   //storage is bottom-up,from left to right
    {
        height=0;
        width=0;
    }

    Image(int height,int width)
    {
        this->height=height;
        this->width=width;
        rowsize=(3*width+3)/4*4;   //set to be a multiple of "4"
        term=new BYTE[height*rowsize];
        for(int y=0; y<height; y++)
            for(int x=0; x<width; x++)
                term[y*rowsize+3*x]=term[y*rowsize+3*x+1]=term[y*rowsize+3*x+2]= 255;
    }

    void load(const char *filename)
    {
        INFO h;
        ifstream f;
        f.open(filename,ios::in|ios::binary);
        f.seekg(0,f.end);
        f.seekg(0,f.beg);
        f.read((char*)&h,sizeof(h));

        width=h.biWidth;
        height=h.biHeight;
        cout<<"reading from "<<filename<<"..."<<endl;
        *this=Image(height,width);
        f.read((char*)term,height*rowsize);
        f.close();
    }

    void save(const char* filename)
    {
        INFO h=
        {
            19778,   //0x4d42
            DWORD(54+rowsize*height),
            0,
            0,
            54,
            40,
            width,
            height,
            1,
            24,
            0,
            DWORD(rowsize*height),
            3780,   //3780
            3780,   //3780
            0,
            0
        };
        cout<<"writing into "<<filename<<"..."<<endl;
        ofstream f;
        f.open(filename,ios::out|ios::binary);
        f.write((char*)&h,sizeof(h));
        f.write((char*)term,rowsize*height);
        f.close();
    }
};


 // Variables
    long lIDXA = 0;
    double byteRGB_RA = 0;
    double byteRGB_GA = 0;
    double byteRGB_BA = 0;
    double resRGB  = 0;
    int Gray = 0;
    int  outDith    = 0; // output in dithering  white = 1 or black = 0
    int  orderedDithering [2][2] = {{ 0,2 },{3,1}};

 //Gray Image
Image grayscale(Image input){
        Image output = input ;
  for ( int iY = 0; iY < input.height; iY++ )
    {
        for ( int iX = 0; iX < input.width; iX++ )
        {
            lIDXA = ( iX *input.height* 3 ) + ( iY * 3 );
            byteRGB_BA =  input.term[lIDXA+0]; //Blue
            byteRGB_GA =  input.term[lIDXA+1]; //Green
            byteRGB_RA =  input.term[lIDXA+2]; //Red
            // To transform RGB to  Gray scale).
            resRGB =  ( byteRGB_RA +  byteRGB_GA +  byteRGB_BA)/3;

            input.term[lIDXA+0] = resRGB;
            input.term[lIDXA+1] = resRGB;
            input.term[lIDXA+2] = resRGB;
        }
    }

    return input;

}
//Dithering Image
Image dithering(Image input)
{

    Image output = input ;


      for( int y=0 ; y < input.height ; y+=3) // step 3 -> in pixels RGB
     {

        for(int x=0; x<input.width; x+=3)
        {

            for ( int i = 0; i < 2;  i++ )
            {

                for ( int j = 0; j < 2; j++ )
                {
                   // Read The index of pixel so mult 3
                    lIDXA = ((x+i)*3)+((y+j)*input.width* 3 );
                    Gray = input.term[lIDXA+0];
                    if ( Gray > orderedDithering  [ i ][ j ] )
                    {
                        outDith = 255;
                    }
                    else
                    {
                        outDith = 0;
                    }
                    // Half-tone image
                    input.term[lIDXA+0] = outDith;
                    input.term[lIDXA+1] = outDith;
                    input.term[lIDXA+2] = outDith;
                }
            }
        }
    }


    return output ;
}

int main()
{

    Image input,output;
  //  char filename[80];
  //  cout<<"Input a file: ";
   // cin>>filename;
   // input.load(filename);
   input.load("Lenna.bmp");
    output = grayscale (input );
    output=dithering(input );
    output.save("rahafssss.png");
    cout<<"Done";

}
