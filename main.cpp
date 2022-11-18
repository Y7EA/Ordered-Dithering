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
        rowsize=(3*width+3);
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
            DWORD(rowsize*height),
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
            72,
            72,
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
    long INDEX = 0;
    double byteRGB_RA = 0;
    double byteRGB_GA = 0;
    double byteRGB_BA = 0;
    double resRGB  = 0;
    int Gray = 0;
    int  outDith    = 0; // output in dithering  white = 1 or black = 0
    int  orderedDithering [2][2] = {	//	2x2  Dithering Matrix
   												{	 51, 206	},
 												{	153, 102	}
 											};
        /* Odered matrix = [0 , 2
                           3 , 1] --> in index range matrix 0 = 51 , 2 = 206 , 3 = 153 , 1 = 102   256/5 = 51
           */    
  //Gray Image
Image grayscale(Image input){
        Image output = input ;
  for ( int Y = 0; Y < input.height; Y++ )
    {
        for ( int X = 0; X < input.width; X++ )
        {
            INDEX = ( X *input.height* 3 ) + ( Y * 3 );
            byteRGB_BA =  input.term[INDEX+0]; //Blue
            byteRGB_GA =  input.term[INDEX+1]; //Green
            byteRGB_RA =  input.term[INDEX+2]; //Red
            // To transform RGB to  Gray scale).
            resRGB =  ( byteRGB_RA +  byteRGB_GA +  byteRGB_BA)/3;

            input.term[INDEX+0] = resRGB;
            input.term[INDEX+1] = resRGB;
            input.term[INDEX+2] = resRGB;
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
                    INDEX = ((x+j)*3)+((y+i)*input.width* 3 );
                    Gray = input.term[INDEX+0];
                    if ( Gray > orderedDithering  [ i ][ j ] )
                    {
                        outDith = 255;
                    }
                    else
                    {
                        outDith = 0;
                    }
                    // Half-tone image
                    input.term[INDEX+0] = outDith;
                    input.term[INDEX+1] = outDith;
                    input.term[INDEX+2] = outDith;
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
   cout << "                     ,---.           ,---.   " << endl;
	cout << "                    / /^`.\\.--''''-./,'^\\ \\	" << endl;
	cout << "                    \\ \\    _       _    / /	" << endl;
	cout << "                     `./  / __   __ \\  \\,'	" << endl;
	cout << "                      /    /_O)_(_O\\    \\	" << endl;
	cout << "                      |  .-'  ___  `-.  |	" << endl;
	cout << "                   .--|       \\_/       |--.	" << endl;
	cout << "                 ,'    \\   \\   |   /   /    `." << endl;
	cout << "                /       `.  `--^--'  ,'       \\" << endl;
	cout << "             .-^^^^^-.    `--.___.--'     .-^^^^^-." << endl;
	cout << ".-----------/         \\------------------/         \\--------------." << endl;
	cout << "| .---------\\         /----------------- \\         /------------. |" << endl;
	cout << "| |          `-`--`--'                    `--'--'-'             | |" << endl;
	cout << "| |                                                             | |" << endl;
	cout << "| |                            Done                             | |" << endl;
	cout << "| |                                                             | |" << endl;
	cout << "| |                                                             | |" << endl;
	cout << "| |_____________________________________________________________| |" << endl;
	cout << "|_________________________________________________________________|" << endl;
	cout << "                   )__________|__|__________(" << endl;
	cout << "                  |            ||            |" << endl;
	cout << "                  |____________||____________|" << endl;
	cout << "                    ),-----.(      ),-----.(" << endl;
	cout << "                  ,'   ==.   \\    /  .==    `." << endl;
	cout << "                 /            )  (            \\" << endl;
	cout << "                 `==========='    `==========='  " << endl;

}
