#include "tgaimage.h"
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;

const TGAColor red = TGAColor(255, 0, 0, 255);

const int HEIGHT = 100;
const int WIDTH = 100;

void DigitDiffAnalyz(double x1, double y1, double x2, double y2)
{
    const double E = 0.001;

    TGAImage image(HEIGHT, WIDTH, TGAImage::RGB);

    double Px = x2 - x1;
	double Py = y2 - y1;

	if (Py == 0)
    {
        if (Px < 0) swap(x1, x2);
        for (int i = x1; i < x2; i++)
        {
            image.set(i, y1, red);
        }
    }
    else if (Px == 0)
    {
        if (Py < 0) swap(y1, y2);
        for (int i = y1; i < y2; i++)
        {
            image.set(x1, i, red);
        }
    }
    else
    {
        int signX = (Px > 0) ? 1 : -1;
        int signY = (Py > 0) ? 1 : -1;

        while (true)
        {
            x1 += signX;
            y1 += signY * abs(Py / Px);
            image.set(x1, y1, red);
            if (x1 - x2 < E && y1 - y2 < E)
            {
                break;
            }
        }
    }
	image.flip_vertically();
	image.write_tga_file("DigitDiffAnalyz.tga");
}

void LineBrez(double x1, double y1, double x2, double y2)
{
    if (x1 > x2)
    {
        swap(x1, x2);
        swap(y1, y2);
    }

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int signX = x1 < x2 ? 1 : -1;
    int signY = y1 < y2 ? 1 : -1;
    int error = dx - dy;

	TGAImage image(100, 100, TGAImage::RGB);
	while (true)
    {
        image.set(x1, y1, red);
        if (error * 2 >= -dy)
        {
            error -= dy;
            x1 += signX;
        }
        if (error * 2 <= dx)
        {
            error += dx;
            y1 += signY;
        }

        if (x1 == x2 && y1 == y2)
        {
            break;
        }
    }

	image.flip_vertically();
	image.write_tga_file("LineBrez.tga");
}

void CircleBrez(int x0, int y0, int r)
{
    double x = 0;
	double y = r;
	double Dd = 2 - 2 * r;

	TGAImage image(HEIGHT, WIDTH, TGAImage::RGB);

	while(y >= 0)
    {
		image.set(x0 + x, y0 - y, red);
		image.set(x0 + x, y0 + y, red);
		image.set(x0 - x, y0 + y, red);
		image.set(x0 - x, y0 - y, red);

		// Dd = (x + 1)^2 + (y - 1)^2 - R^2
		if (Dd < 0) // POINT - INSIDE CIRCLE
        {
            // di = |Dg| - |Dd| = |(x + 1)^2 + y^2 - R^2| - |(x + 1)^2 + (y - 1)^2 - R^2|
            //    = ... (Dd < 0) ... = 2 * (Dd + y) - 1
            double di = 2 * (Dd + y) - 1;
            if (di < 0) // Pg nearer than Pd, HORIZONTAL MOVE
            {
                x++;
                Dd += 2 * x + 1;
            }
            if (di > 0) // Pd nearer than Pg, DIAGONAL MOVE
            {
                y--;
                x++;
                Dd += 2 * (x - y) + 6;
            }
        }
        else if (Dd > 0) // POINT - OUTSIDE CIRCLE
        {
            // di = |Dd| - |Dv| = |(x + 1)^2 + (y - 1)^2 - R^2| - |x^2 + (y - 1)^2 - R^2|
            //    = ... (Dd > 0) ... = 2 * (Dd - x) - 1
            double di = 2 * (Dd - x) - 1;
            if (di < 0) // Pd nearer than Pv, DIAGONAL MOVE
            {
                x++;
                y--;
                Dd += 2 * (x - y) + 6;
            }
            if (di > 0) // Pv nearer than Pd, VERTICAL MOVE
            {
                y--;
                Dd += -2 * y + 1;
            }
        }
        else // POINT - ON CIRCLE
        {
            x++;
            y--;
            Dd += 2 * (x - y) + 6;
        }
	}

	image.flip_vertically();
	image.write_tga_file("CircleBrez.tga");
}

void CircleParametric(int x0, int y0, int r)
{
	TGAImage image(HEIGHT, WIDTH, TGAImage::RGB);

	for (int x = 0; x <= r / sqrt(2); x++)
    {
		int y = (int) (sqrt(pow(r, 2) - pow(x, 2)));
		image.set(x0 + x, y0 + y, red);
		image.set(x0 + y, y0 + x, red);
		image.set(x0 + y, y0 - x, red);
		image.set(x0 + x, y0 - y, red);
		image.set(x0 - x, y0 - y, red);
		image.set(x0 - y, y0 - x, red);
		image.set(x0 - y, y0 + x, red);
		image.set(x0 - x, y0 + y, red);
	}
	image.flip_vertically();
	image.write_tga_file("CircleParametric.tga");
}

int main()
{
    TGAImage image(HEIGHT, WIDTH, TGAImage::RGB);
    image.set(1, 1, red);
    image.set(1, 2, red);

    /*DigitDiffAnalyz(80, 70, 50, 60);
    LineBrez(5, 80, 60, 20);
    CircleBrez(50, 50, 30);
    CircleParametric(70, 40, 20);*/
    return 0;
}

