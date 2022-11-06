#include <iostream>
#include <fstream>
using namespace std;
int checkColor(int a, int b);

unsigned char buf[512][512];

unsigned char out[512][512];

float ratio = 1.0 / 17;


int main()
{


    FILE *fp, *output;

    int i, j, k, l;

    /*
    4x4 Ordered Dither Matrix:

    [0 8 2 10]

    [12 4 14 6]

    [3 11 1  9]

    [15 7 13 5]
    */
    unsigned int dith[4][4] = {{0, 8, 2, 10},
                              {12, 4, 14, 6},
                              {3, 11, 1,  9},
                              {15, 7, 13, 5}};


    if((fp = fopen("Lenna.png", "rb")) == NULL)
    {

        printf("error opening file\n");

    }



    for (i = 0; i < 512; i++)
    {

        for (j = 0; j < 512; j++)
        {

            buf[i][j] = fgetc(fp);

        }

    }


    i = 0;

    j = 0;


    int x, y;

    int bd = 64;

    for (k = 0; k < 512; k++)
    {

        for (l = 0; l < 512; l++)
        {

            int oldPixel = buf[k][l];

            int value = (oldPixel + (ratio * dith[k%4][l%4]));

            int r = ((oldPixel >> 16) & 0xff) + value;

            int g = ((oldPixel >> 8) & 0xff) + value;

            int b = (oldPixel & 0xff) + value;

            int newPixel = 0x000000 | checkColor(r, bd) << 16 | checkColor(g, bd) << 8 | checkColor(b, bd);

            out[k][l] = newPixel;

        }

    }


    output = fopen("Lenna_Dither.png", "wb");



    for (i = 0; i < 512; i++)
    {

        for (j = 0; j < 512; j++)
        {

            fputc(out[i][j], output);

        }

    }



    fclose(output);

    fclose(fp);
    cout<<"Done";


    return 0;

}



int checkColor(int a, int b)
{

    return a / b * b;

}
