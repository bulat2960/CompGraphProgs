#include <vector>
#include <cmath>
#include <stack>
#include <algorithm>
#include <iostream>
#include <map>
#include <cstdlib>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

using namespace std;

const int width  = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1))
    {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error2 += derror2;

        if (error2 > dx)
        {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void triangle(Vec2i p1, Vec2i p2, Vec2i p3, TGAImage& image, TGAColor color)
{
	line(p1.x, p1.y, p2.x, p2.y, image, color);
	line(p2.x, p2.y, p3.x, p3.y, image, color);
	line(p3.x, p3.y, p1.x, p1.y, image, color);
}

int main()
{
    Model* model = new Model("obj/african_head.obj");

    TGAImage image(width, height, TGAImage::RGB);

	vector<vector<double>> finalCoords;
    for (int i = 0; i < model->nfaces(); i++)
    {
        vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++)
        {
            Vec3f coords = model->vert(face[j]);
			finalCoords.push_back({coords.x, coords.y, coords.z});
        }
    }

    for (int i = 0; i < decCoords.size(); i += 3)
    {
        vector<double> p1 = finalCoords[i];
        vector<double> p2 = finalCoords[i + 1];
        vector<double> p3 = finalCoords[i + 2];

        Vec2i pp1 = Vec2i((p1[0] + 1.0) * width / 2.0, (p1[1] + 1.0) * height / 2.0);
        Vec2i pp2 = Vec2i((p2[0] + 1.0) * width / 2.0, (p2[1] + 1.0) * height / 2.0);
        Vec2i pp3 = Vec2i((p3[0] + 1.0) * width / 2.0, (p3[1] + 1.0) * height / 2.0);

        triangle(pp1, pp2, pp3, image, white);
    }

    image.flip_vertically();
    image.write_tga_file("file.tga");

    delete model;
    return 0;
}
