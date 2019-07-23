#include <iostream>
#include <cmath>
#include <vector>
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

using namespace std;

const int width  = 1000;
const int height = 1000;

const double PI = 3.14159265;

double getRadians(double value)
{
    return value * PI / 180;
}

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

struct Point
{
    double x;
    double y;
    double z;

    Point(double x, double y, double z = 0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    friend Point operator+(const Point& p, const Point& transfer)
    {
        return Point(p.x + transfer.x, p.y + transfer.y, p.z + transfer.z);
    }

    friend Point operator*(const Point& p, double mult)
    {
        return Point(p.x * mult, p.y * mult, p.z * mult);
    }

    Point& operator+=(const Point& transfer)
    {
        this->x += transfer.x;
        this->y += transfer.y;
        this->z += transfer.z;
        return *this;
    }

    Point& operator*=(double mult)
    {
        this->x *= mult;
        this->y *= mult;
        this->z *= mult;
        return *this;
    }

    friend ostream& operator<<(ostream& ost, const Point& p)
    {
        ost << "x = " << p.x << ' ' << "y = " << p.y << ' ' << "z = " << p.z;
        return ost;
    }
};

struct PointArray
{
    vector<Point> points;

    PointArray(vector<Point> points)
    {
        this->points = points;
    }

    void buildImage(TGAImage& image, TGAColor color)
    {
        for (unsigned i = 0; i < points.size(); i++)
        {
            line(points[i].x, points[i].y, points[(i + 1) % points.size()].x, points[(i + 1) % points.size()].y, image, color);
        }
    }

    void parallelTransfer(const Point& p)
    {
        for (auto& point : points)
        {
            point += p;
        }
    }

    void zoom(int mult)
    {
        for (auto& point : points)
        {
            point *= mult;
        }
    }

    void getCoords()
    {
        for (const auto& point : points)
        {
            cout << point << endl;
        }
    }
};

void transferTest(Point transferPoint)
{
    Point p1(0, 0);
    Point p2(200, 300);
    Point p3(400, 100);
    PointArray arr({p1, p2, p3});

    TGAImage image(height, width, TGAImage::RGB);
    arr.buildImage(image, red);
    arr.parallelTransfer(transferPoint);
    arr.buildImage(image, green);
    image.flip_vertically();
    image.write_tga_file("transfer.tga");
}

void zoomTest(int mult)
{
    Point p1(0, 0);
    Point p2(200, 300);
    Point p3(300, 100);
    PointArray arr({p1, p2, p3});

    TGAImage image(height, width, TGAImage::RGB);
    arr.buildImage(image, red);
    arr.zoom(mult);
    arr.buildImage(image, green);
    image.flip_vertically();
    image.write_tga_file("zoom.tga");
}

void angleTest(int angle)
{
    Point p1(0, 0);
    Point p2(500, 500);
    Point p3(900, 100);
    PointArray arr({p1, p2, p3});

    TGAImage image(height, width, TGAImage::RGB);
    arr.buildImage(image, red);
    arr.buildImage(image, green);
    image.flip_vertically();
    image.write_tga_file("angle.tga");
}

/* -------------------------------------- */

struct Matrix
{
    vector<vector<double>> matrix;

    enum class Type
    {
        ZOOM,
        TRANSFER,
        ANGLE,
        SDVIG
    };

    Matrix(Type type, vector<double> args)
    {
        if (type == Type::ZOOM)     getZoomMatrix(args[0], args[1], args[2]);
        if (type == Type::TRANSFER) getTransferMatrix(args[0], args[1], args[2]);
        if (type == Type::ANGLE)    getAngleMatrix(args[0]);
        if (type == Type::SDVIG)    getSdvigMatrix(args[0], args[1]);
    }

    Matrix getTransferMatrix(double Tx, double Ty, double Tz)
    {
        matrix.push_back({1,  0,  0,  0});
        matrix.push_back({0,  1,  0,  0});
        matrix.push_back({0,  0,  1,  0});
        matrix.push_back({Tx, Ty, Tz, 1});
        return *this;
    }

    Matrix getZoomMatrix(double Sx, double Sy, double Sz)
    {
        matrix.push_back({Sx,  0,  0, 0});
        matrix.push_back({ 0, Sy,  0, 0});
        matrix.push_back({ 0,  0, Sz, 0});
        matrix.push_back({ 0,  0,  0, 1});
        return *this;
    }

    Matrix getAngleMatrix(double angle)
    {
        double rad = getRadians(angle);

        matrix.push_back({ cos(rad), sin(rad), 0, 0});
        matrix.push_back({-sin(rad), cos(rad), 0, 0});
        matrix.push_back({        0,        0, 1, 0});
        matrix.push_back({        0,        0, 0, 1});
        return *this;
    }

    Matrix getSdvigMatrix(double a, double b)
    {
        matrix.push_back({1, a, 0, 0});
        matrix.push_back({b, 1, 0, 0});
        matrix.push_back({0, 0, 1, 0});
        matrix.push_back({0, 0, 0, 1});
        return *this;
    }

    vector<double> operator[](int index) const
    {
        return matrix[index];
    }
};

struct HomogenCoordArray
{
    double x;
    double y;
    double z;
    double w;

    HomogenCoordArray(double x, double y, double z, double w) : x(x), y(y), z(z), w(w)
    {
        normalize();
    }

    void update(Matrix matrix)
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

    void normalize()
    {
        x /= w;
        y /= w;
        z /= w;
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

    HomogenCoordArrayVector(vector<HomogenCoordArray> points)
    {
        this->points = points;
    }

    void buildImage(TGAImage& image, TGAColor color)
    {
        for (unsigned i = 0; i < points.size(); i++)
        {
            line(points[i].x, points[i].y, points[(i + 1) % points.size()].x, points[(i + 1) % points.size()].y, image, color);
        }
    }

    void update(Matrix matrix)
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

void build(HomogenCoordArrayVector vec, Matrix matrix, string filename)
{
    TGAImage image(height, width, TGAImage::RGB);
    vec.buildImage(image, red);
    vec.update(matrix);
    vec.buildImage(image, green);
    image.flip_vertically();
    image.write_tga_file((filename + ".tga").c_str());
}

int main()
{
    transferTest({200, 100});
    zoomTest(3);
    angleTest(30);

    /* ------------------ */

    double w = 1;

    HomogenCoordArray p1(0, 0, 100, w);
    HomogenCoordArray p2(500, 500, 300, w);
    HomogenCoordArray p3(900, 100, 200, w);

    HomogenCoordArrayVector vec({p1, p2, p3});
    Matrix matrix(Matrix::Type::ANGLE, {20});
    build(vec, matrix, "homoAngle");

    matrix = Matrix(Matrix::Type::ZOOM, {1.1, 1.1, 0});
    build(vec, matrix, "homoZoom");

    matrix = Matrix(Matrix::Type::TRANSFER, {200, 300, 0});
    build(vec, matrix, "homoTransfer");

    matrix = Matrix(Matrix::Type::SDVIG, {0.7, 0.5});
    build(vec, matrix, "homoSdvig");


    /*
    | 1 0 0    0 |
    | 0 1 0    0 |
    | 0 0 0 -1/C |
    | 0 0 0    1 |
    */

    return 0;
}
