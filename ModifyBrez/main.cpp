#include "tgaimage.h"
#include <cmath>
#include <iostream>

const TGAColor red = TGAColor(255, 0, 0, 255);

const int HEIGHT = 1000;
const int WIDTH = 1000;

const double I = 1.0;

using namespace std;

void line(int x1, int y1, int x2, int y2)
{
    TGAImage image(HEIGHT, WIDTH, TGAImage::RGB);

    if (x1 == x2)
    {
        if (y1 > y2)
        {
            swap(y1, y2);
            swap(x1, x2);
        }
        for (int i = y1; i <= y2; i++)
        {
            image.set(x1, i, red);
        }
    }
    else if (y1 == y2)
    {
        if (x1 > x2)
        {
            swap(y1, y2);
            swap(x1, x2);
        }
        for (int i = x1; i <= x2; i++)
        {
            image.set(i, y1, red);
        }
    }
    else
    {
        bool step = false;
        if (abs(x1 - x2) < abs(y1 - y2))
        {
            swap(x1, y1);
            swap(x2, y2);
            step = true;
        }

        if (x1 > x2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }


        int x = x1;
        int y = y1;
        double Px = abs(x2 - x1);
        double Py = abs(y2 - y1);
        int signX = x1 < x2 ? 1 : -1;
        int signY = y1 < y2 ? 1 : -1;
        double t = Py / Px;
        double d = 0.5;
        double dmax = 1.0 - t;


        TGAColor c1 = TGAColor(255 * d, 0, 0, 255 * d);
        image.set(step ? y : x, step ? x : y, c1);

        while (true)
        {
            double t = (double)(x - x1) / (x2 - x1);
            y = y1 * (1.0 - t) + y2 * t;

            if (d >= dmax)
            {
                d -= dmax;
            }
            else
            {
                d += t;
            }
            x += signX;

            if (x == x2 && y == y2)
            {
                break;
            }

            TGAColor c1 = TGAColor(255 * d, 0, 0, 255 * d);
            image.set(step ? y : x, step ? x : y, c1);
        }
    }

    image.flip_vertically();
    image.write_tga_file("line.tga");
}

int main()
{
    line(500, 500, 800, 1000);
    return 0;
}
