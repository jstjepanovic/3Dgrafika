#include <iostream>
#include <cmath>
#include "tgaimage.h"

using namespace std;

// dimenzije slike
const int width  = 200;
const int height = 200;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (abs(x1 - x0) < abs(y1 - y0))
    {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    
    // zamijeni koordinate da pravac bude s lijeva na desno
    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    
    // racunanje nagiba pravca
    int dx = x1 - x0;
    int dy = y1 - y0;
    
    float derror = abs(2 * dy);
    float error  = 0;
    
    int y = y0;
    for (int x = x0; x <= x1; ++x)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        
        // akumuliraj pogresku
        error += derror;
        
        // ako je greska dovoljno velika, povecaj y za 1
        if (error > dx)
        {
            if (y1 > 0)
                y += 1;
            else
                y -= 1;
            error -= 2 * dx;
        }
    }
}

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);
    
    // nacrtaj nekoliko duzina    
    line(20,13,140,180, image, red);
    line(13,20,180,140, image, white);
    line(80,40,113,120, image, blue);
    
    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("lines5.tga");
}