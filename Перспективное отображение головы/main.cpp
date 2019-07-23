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

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
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

struct HomogenCoordArray
{
    double x;
    double y;
    double z;
    double w;

    HomogenCoordArray(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

    void update(vector<vector<double>> matrix)
    {
        double tx = x * matrix[0][0] + y * matrix[1][0] + z * matrix[2][0] + w * matrix[3][0];
        double ty = x * matrix[0][1] + y * matrix[1][1] + z * matrix[2][1] + w * matrix[3][1];
        double tz = x * matrix[0][2] + y * matrix[1][2] + z * matrix[2][2] + w * matrix[3][2];
        double tw = x * matrix[0][3] + y * matrix[1][3] + z * matrix[2][3] + w * matrix[3][3];

        x = tx;
        y = ty;
        z = tz;
        w = tw;
    }

    vector<double> getDecart()
    {
        return {x/w, y/w, z/w};
    }

    friend ostream& operator<<(ostream& ost, const HomogenCoordArray& arr)
    {
        ost << "[" << arr.x << ' ' << arr.y << ' ' << arr.z << ' ' << arr.w << "]";
        return ost;
    }
};

struct HomogenCoordArrayVector
{
    vector<HomogenCoordArray> points;

    void update(vector<vector<double>> matrix)
    {
        for (auto& p : points)
        {
            p.update(matrix);
        }
    }

    friend ostream& operator<<(ostream& ost, const HomogenCoordArrayVector& arr)
    {
        for (unsigned i = 0; i < arr.points.size(); i++)
        {
            ost << arr.points[i] << endl;
        }
        return ost;
    }
};

const double PI = 3.14159265;

double getRadians(double value)
{
    return value * PI / 180;
}

vector<vector<double>> getViewMatrix(double c)
{
    vector<vector<double>> matrix;
    matrix.push_back({1, 0, 0,    0});
    matrix.push_back({0, 1, 0,    0});
    matrix.push_back({0, 0, 0, -1/c});
    matrix.push_back({0, 0, 0,    1});
    return matrix;
}

vector<vector<double>> getAngleMatrixXY(double angle)
{
    vector<vector<double>> matrix;
    double rad = getRadians(angle);
    matrix.push_back({ cos(rad), sin(rad), 0, 0});
    matrix.push_back({-sin(rad), cos(rad), 0, 0});
    matrix.push_back({        0,        0, 1, 0});
    matrix.push_back({        0,        0, 0, 1});
    return matrix;
}

vector<vector<double>> getAngleMatrixXZ(double angle)
{
    vector<vector<double>> matrix;
    double rad = getRadians(angle);
    matrix.push_back({ cos(rad), 0, sin(rad), 0});
    matrix.push_back({        0, 1,        0, 0});
    matrix.push_back({-sin(rad), 0, cos(rad), 0});
    matrix.push_back({        0, 0,        0, 1});
    return matrix;
}

vector<vector<double>> getAngleMatrixYZ(double angle)
{
    vector<vector<double>> matrix;
    double rad = getRadians(angle);
    matrix.push_back({1,         0,        0, 0});
    matrix.push_back({0,  cos(rad), sin(rad), 0});
    matrix.push_back({0, -sin(rad), cos(rad), 0});
    matrix.push_back({0,         0,        0, 1});
    return matrix;
}

void lines(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
{
    line(t0.x, t0.y, t1.x, t1.y, image, white);
    line(t1.x, t1.y, t2.x, t2.y, image, white);
    line(t2.x, t2.y, t0.x, t0.y, image, white);
}

int main()
{
    Model* model = new Model("obj/african_head.obj");

    TGAImage image(width, height, TGAImage::RGB);

    HomogenCoordArrayVector v;

    for (int i = 0; i < model->nfaces(); i++)
    {
        vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++)
        {
            Vec3f coords = model->vert(face[j]);
            v.points.push_back(HomogenCoordArray({coords.x, coords.y, coords.z, 1}));
        }
    }

    v.update(getAngleMatrixYZ(0));
    v.update(getAngleMatrixXZ(0));
    v.update(getViewMatrix(100));
    vector<vector<double>> decCoords;
    for (unsigned i = 0; i < v.points.size(); i++)
    {
        decCoords.push_back(v.points[i].getDecart());
    }

    for (int i = 0; i < decCoords.size(); i += 3)
    {
        vector<double> p1 = decCoords[i];
        vector<double> p2 = decCoords[i + 1];
        vector<double> p3 = decCoords[i + 2];

        Vec2i pp1 = Vec2i((p1[0] + 1.0) * width / 2.0, (p1[1] + 1.0) * height / 2.0);
        Vec2i pp2 = Vec2i((p2[0] + 1.0) * width / 2.0, (p2[1] + 1.0) * height / 2.0);
        Vec2i pp3 = Vec2i((p3[0] + 1.0) * width / 2.0, (p3[1] + 1.0) * height / 2.0);

        lines(pp1, pp2, pp3, image, white);
    }

    image.flip_vertically();
    image.write_tga_file("file.tga");

    delete model;
    return 0;
}

