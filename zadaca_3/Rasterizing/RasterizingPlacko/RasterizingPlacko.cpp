#include <iostream>
#include <cmath>
#include "tgaimage.h"
#include <vector>
#include <map>
#include "OBJparser.h"

using namespace std;

struct tekstura {
    TGAImage image;

    tekstura(const char* filename) {
        image.read_tga_file(filename);
    }
};




// dimenzije slike
const int width = 200;
const int height = 200;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);


void aquireimage(TGAImage& image, const char *filename) {
    image.read_tga_file(filename);
    cout << image.get_height() << " x " << image.get_width() << endl;
}

float determinant2(float a, float b, float c, float d) {
    return a * d - (b * c);
}


void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
    bool steep = false;

    if (abs(x1 - x0) < abs(y1 - y0)) {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }

    //zamijeni koordinate da pravac ide slijeva na desno
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    float d = abs((float)2 * dy);
    float error = 0;

    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        /*float t = (float)(x - x0) / (x1 - x0);
        float y = (1.0 - t) * y0 + t * y1;*/

        if (steep) {
            image.set((int)y, (int)x, color);
        }
        else {
            image.set((int)x, (int)y, color);
        }

        error += d;

        if (error > dx) {
            if (y1 > y0) {
                y += 1;
            }
            else {
                y -= 1;
            }
            error -= 2 * dx;
        }

    }
}





void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage& image, tekstura t) {
    line(x0, y0, x1, y1, image, white);
    line(x1, y1, x2, y2, image, white);
    line(x2, y2, y0, y0, image, white);

    float alpha, beta, gama;

    float A = determinant2(x1 - x0, x2 - x0, y1 - y0, y2 - y0);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            
            beta = determinant2(i - x0, x2 - x0, j - y0, y2 - y0) / A;
            gama = determinant2(x1 - x0, i - x0, y1 - y0, j - y0) / A;

            alpha = 1 - beta - gama;

            if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gama >= 0 && gama <= 1) {
                image.set(i, j, white);
            }
        }
    }

    for (int i = 0; i < image.get_width(); i++)
    {
        for (int j = 0; j < image.get_height(); j++)
        {
            if (image.get(i, j).r == white.r && image.get(i, j).g == white.g && image.get(i, j).b == white.b) {
                image.set(i, j, t.image.get(i, j));
            }
        }
    }

    
}

void loadModule(string filenameOBJ, TGAImage& image, tekstura& t) {
    vector<vector<float>> vertex;
    vector<vector<map<int, int>>> faces;
    vector<vector<float>> normals;


    OBJparser(filenameOBJ, vertex, faces, normals);

    vector<vector<float>> tritocke;


    for (auto f : faces) {
        
        for (auto lines : f) {

            for (auto vertInd : lines) {

                vector<float> vert = vertex[vertInd.first - 1];
                tritocke.push_back(vert);
            }
            
        }
        
        triangle((int)tritocke[0][0], (int)tritocke[0][1], (int)tritocke[1][0], (int)tritocke[1][1], (int)tritocke[2][0], (int)tritocke[2][1], image, t);
        tritocke.clear();
    }

}

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);
    TGAImage image1;
    TGAImage image2(400, 400, TGAImage::RGB);

    aquireimage(image1, "nova_slika.tga");

    tekstura t1("nova_slika.tga");

    // nacrtaj nekoliko duzina    
    /*line(20, 13, 140, 180, image, red);
    line(13, 20, 180, 140, image, white);
    line(80, 40, 113, 120, image, blue);*/
    triangle(20, 20, 50, 150, 120, 20, image, t1);

    loadModule("creation.obj", image2, t1);
    
    

    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("lines5.tga");
    image1.write_tga_file("slika.tga");
    image2.flip_vertically();
    image2.write_tga_file("objectSlika.tga");
    cout << "Rasterizing done" << endl;
}