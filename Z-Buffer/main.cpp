#include <vector>
#include <cmath>
#include <iostream>
#include <limits>
#include <algorithm>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

#include <bitset>

const int width  = 800;
const int height = 800;
const int depth  = 255;

using namespace std;

void line(Vec3i p1, Vec3i p2, vector<Vec3i>& edgePoints)
{
    bool steep = false;
    if (abs(p1.x - p2.x) < abs(p1.y - p2.y))
    {
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
        steep = true;
    }
    if (p1.x > p2.x)
    {
        swap(p1, p2);
    }

    for (int i = p1.x; i <= p2.x; i++)
    {
        float phi = (p1.x == p2.x) ? 1.0 : (float)(i - p1.x) / (float)(p2.x - p1.x);
        Vec3i P = Vec3f(p1) + Vec3f(p2 - p1) * phi;
        if (steep)
        {
            edgePoints.push_back({P.y, P.x, P.z});
        }
        else
        {
			edgePoints.push_back({P.x, P.y, P.z});
        }
    }
}

void triangle(Vec3i p1, Vec3i p2, Vec3i p3, vector<vector<int>>& zBuffer)
{
    vector<Vec3i> points;
    line(p1, p2, points);
    line(p2, p3, points);
    line(p3, p1, points);

    sort(points.begin(), points.end(), [&points](Vec3i v1, Vec3i v2){return v1.y < v2.y;});

    for (unsigned i = 1; i < points.size(); i++)
	{
	    Vec3i A = points[i - 1];
        Vec3i B = points[i];
	    if (A.y != B.y)
        {
            continue;
        }
        if (A.x >= B.x)
        {
            swap(A, B);
        }
        for (int j = A.x; j <= B.x; j++)
        {
            double alpha = (A.x == B.x) ? 1.0 : static_cast<double>(j - A.x) / static_cast<double>(B.x - A.x);
            Vec3i C = static_cast<Vec3i>(Vec3f(A) + Vec3f(B - A) * alpha);
            if (zBuffer[C.x][C.y] < C.z)
            {
                zBuffer[C.x][C.y] = C.z;
            }
        }
	}
}

int main()
{
    Model* model = new Model("obj/african_head.obj");

    vector<vector<int>> zBuffer;
    for (int i = 0; i < width; i++)
    {
        vector<int> zBufferLine;
        for (int j = 0; j < height; j++)
        {
            zBufferLine.push_back(INT_MIN);
        }
        zBuffer.push_back(zBufferLine);
    }

    for (int i = 0; i < model->nfaces(); i++)
    {
        vector<int> face = model->face(i);
        vector<Vec3i> coords(3);
        for (int j = 0; j<3; j++)
        {
            Vec3f v = model->vert(face[j]);
            coords[j] = Vec3i((v.x + 1.0) * width / 2.0, (v.y + 1.0) * height / 2.0, (v.z + 1.0) * depth / 2.0);
        }
        triangle(coords[0], coords[1], coords[2], zBuffer);
    }

    TGAImage image(width, height, TGAImage::GRAYSCALE);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            image.set(i, j, TGAColor(zBuffer[i][j], 0));
        }
    }
    image.flip_vertically();
    image.write_tga_file("Z-Buffer.tga");

    delete model;
    return 0;
}

