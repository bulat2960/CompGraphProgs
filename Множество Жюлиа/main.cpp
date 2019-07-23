#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "tgaimage.h"

using namespace std;

TGAColor color;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

struct Complex
{
    double re;
    double im;
};

Complex add(Complex z1, Complex z2)
{
    Complex result;
    result.re = z1.re + z2.re;
    result.im = z1.im + z2.im;
    return result;
};

Complex mult(Complex z1, Complex z2)
{
    Complex result;
    result.re = z1.re * z2.re - z1.im * z2.im;
    result.im = z1.re * z2.im + z1.im * z2.re;
    return result;
};

double abs(Complex z)
{
    return z.re * z.re + z.im * z.im;
};


int main()
{
    srand(time(0));

    int height = 2000;
    int width  = 2000;

    TGAImage image(height, width, TGAImage::RGB);

    Complex c;
    c.re = -0.623244;
    c.im = -0.4253546455;

    const double r = 1;
    const double xmin = -r;
    const double ymin = -r;
    const double xmax =  r;
    const double ymax =  r;

    Complex z;


    int maxIter = 256;

    double dx = (xmax - xmin) / height;
    double dy = (ymax - ymin) / width;

    for (int y = 0; y < width; y++)
    {
        cout << y << endl;
        for (int x = 0; x < height; x++)
        {
            z.re = xmin + x * dx;
            z.im = ymin + y * dy;
            int k = 0;
            for(int i = 0; i < maxIter; i++)
            {
                if (abs(z) < 5)
                {
                    z = add(mult(z, z), c);
                    k++;
                }
                else break;
            }

            TGAColor color = TGAColor(k * k, k * k, 0, 0);
            image.set(x, y, color);
        };
    }

    image.flip_vertically();
    image.write_tga_file("image.tga");
    return 0;
}
