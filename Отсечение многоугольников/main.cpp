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

int startX = 700;
int startY = 0;
int endX   = 800;
int endY   = 1000;

struct Point
{
	int x;
	int y;
};

const double EPS = 1E-9;

inline int det(int a, int b, int c, int d)
{
    return a * d - b * c;
}

inline bool between(int a, int b, double c)
{
	return min(a,b) <= c + EPS && c <= max(a,b) + EPS;
}

inline bool intersect_1(int a, int b, int c, int d)
{
	if (a > b) swap (a, b);
	if (c > d) swap (c, d);
	return max(a,c) <= min(b,d);
}

bool intersect(Point a, Point b, Point c, Point d)
{
	int A1 = a.y - b.y,  B1 = b.x - a.x,  C1 = -A1 * a.x - B1 * a.y;
	int A2 = c.y - d.y,  B2 = d.x - c.x,  C2 = -A2 * c.x - B2 * c.y;
	int zn = det (A1, B1, A2, B2);
	if (zn != 0)
    {
		double x = -det (C1, B1, C2, B2) * 1.0 / zn;
		double y = -det (A1, C1, A2, C2) * 1.0 / zn;
		return between(a.x, b.x, x) && between(a.y, b.y, y)
			&& between(c.x, d.x, x) && between(c.y, d.y, y);
	}
	else
	{
		return det(A1, C1, A2, C2) == 0 &&
               det(B1, C1, B2, C2) == 0 &&
               intersect_1 (a.x, b.x, c.x, d.x) &&
               intersect_1 (a.y, b.y, c.y, d.y);
	}
}

string getSide(int x, int y)
{
    int A = startY - endY;
    int B = -(startX - endX);
    int C = -(A * startX + B * startY);
    if (A * x + B * y + C < 0)
        return "RIGHT";
    else
        return "LEFT";
}

Point getIntersectPoint(int x1, int y1, int x2, int y2)
{
    double xNumerator = static_cast<double>((x1 * y2 - y1 * x2) * (startX - endX) - (x1 - x2) * (startX * endY - endX * startY));
    double xDenominator = static_cast<double>((x1 - x2) * (startY - endY) - (y1 - y2) * (startX - endX));
    int x = static_cast<int>(round(xNumerator / xDenominator));

    double yNumerator = static_cast<double>((x1 * y2 - y1 * x2) * (startY - endY) - (y1 - y2) * (startX * endY - endX * startY));
    double yDenominator = static_cast<double>((x1 - x2) * (startY - endY) - (y1 - y2) * (startX - endX));
    int y = static_cast<int>(round(yNumerator / yDenominator));

    return {x, y};
}

void SutherlandHodgman(vector<Point> polygon, TGAImage& image)
{
    vector<Point> resultPoints;
    int n = polygon.size();
    for (int i = 0; i < n; i++)
    {
        Point p1 = {polygon[i].x, polygon[i].y};
        Point p2 = {polygon[(i + 1) % n].x, polygon[(i + 1) % n].y};

        if (getSide(p1.x, p1.y) == "LEFT")
        {
            resultPoints.push_back(p1);
        }
        if (intersect(p1, p2, {startX, startY}, {endX, endY}))
        {
            Point p = getIntersectPoint(p1.x, p1.y, p2.x, p2.y);
            resultPoints.push_back(p);
        }
    }

    int sz = resultPoints.size();
    for (int i = 0; i < sz; i++)
    {
        line(resultPoints[i].x, resultPoints[i].y, resultPoints[(i + 1) % sz].x, resultPoints[(i + 1) % sz].y, image, red);
    }
}

void sutherlandPolygonClippping()
{
    TGAImage image(height, width, TGAImage::RGB);

    vector<Point> polygon;
    polygon.push_back({0, 500}); /* 1 */
    polygon.push_back({500, 1000}); /* 2 */
    polygon.push_back({1000, 500}); /* 3 */
    polygon.push_back({500, 0}); /* 4 */

    /* DRAW POLYGON */

    int n = polygon.size();
    for (int i = 0; i < polygon.size(); i++)
    {
        line(polygon[i].x, polygon[i].y, polygon[(i + 1) % n].x, polygon[(i + 1) % n].y, image, green);
    }
    line(startX, startY, endX, endY, image, white);


    /* CLIP POLYGON */
    //SutherlandHodgman(polygon, image);

    image.flip_vertically();
    image.write_tga_file("sutherland.tga");
}



int main()
{
    sutherlandPolygonClippping();
    return 0;
}
