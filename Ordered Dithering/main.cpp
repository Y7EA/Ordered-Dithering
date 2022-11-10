#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
using namespace std;
typedef unsigned char   BYTE;   //  1 byte (0~255)


typedef unsigned char   BYTE;   //  1 byte (0~255)
typedef unsigned short  WORD;   //  2 bytes (0~65536)
typedef unsigned long   DWORD;  //4 bytes (0~2^32 -1)

#pragma pack(push)    //store
#pragma pack(2)        //2-bytes aligned
struct INFO
{
    // BITMAPFILEHEADER (14 bytes) from 16 reducing to 14
    WORD bfType;          //BM -> 0x4d42 (19778)
    DWORD BfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
    // BITMAPINFOHEADER(40 bytes)
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
#pragma pack(pop)      //restore

int orderedDithering [2][2] ={{0,2},{3,1}};
int mapSize=4;
int size_x=2;

class Image
{
    public:
        
        int height;
        int width;
        int rowsize;    // bgr -> 3 bytes(24 bits)
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
           
            *this=Image(height,width);
            f.read((char*)term,height*rowsize);
            f.close();
        }
        
        void save(const char* filename)
        {
            INFO h=
            {19778,   //0x4d42
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
            ofstream f;
            f.open(filename,ios::out|ios::binary);
            f.write((char*)&h,sizeof(h));
            f.write((char*)term,rowsize*height);
            f.close();
        }
};

int pixels[3];
Image dithering(Image original , int orderedDithering [2][2]){
    Image input = original;
    for(int  y=0 ; y < input.height ; y++){
      int  row = y%mapSize ;
        for(int x=1; x<input.width; x++)
        {
            int col = x%mapSize ;
            int  blue    = pixels[x * 3 + 0];
            int  green   = pixels[x * 3 + 1];
            int  red     = pixels[x * 3 + 2];
            pixels[x * 3 + 0]   = (blue  < orderedDithering[col][row] ? 0 : 255);
            pixels[x * 3 + 1]   = (green < orderedDithering[col][row] ? 0 : 255);
            pixels[x * 3 + 2]   = (red   < orderedDithering[col][row] ? 0 : 255);
        }
    }
    return input;

}



int main(){
    Image input,output;
    input.load("lena.png");
    output = dithering (input , orderedDithering[2][2]);
    output.save("lena_out.png");
}
