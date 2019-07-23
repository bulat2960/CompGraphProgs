#include <iostream>
#include <vector>
#include <cmath>
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

using namespace std;

const int width  = 1000;
const int height = 1000;

const int xMin = 200;
const int xMax = 800;
const int yMin = 200;
const int yMax = 800;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

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

/* ----------------------------------------------------- */

int computeOutCode(double x, double y)
{
	int code = INSIDE;

	if (x < xMin) code |= LEFT;
	if (x > xMax) code |= RIGHT;
	if (y < yMin) code |= BOTTOM;
	if (y > yMax) code |= TOP;

	return code;
}

void drawRectangle(int x1, int y1, int x2, int y2, TGAImage& image, TGAColor color)
{
    line(x1, y1, x1, y2, image, color);
    line(x1, y2, x2, y2, image, color);
    line(x2, y2, x2, y1, image, color);
    line(x2, y1, x1, y1, image, color);
}

void cohenSutherland(int x1, int y1, int x2, int y2, TGAImage& image)
{
	int code1 = computeOutCode(x1, y1);
	int code2 = computeOutCode(x2, y2);

	bool accept = false;

	while (true)
    {
		if (!(code1 | code2))
		{
			accept = true;
			break;
		}
        else if (code1 & code2)
        {
			break;
		}
		else
        {
			int x, y;

			int outCode = code1 ? code1 : code2;

			if (outCode & TOP)
            {
				x = x1 + (x2 - x1) * (yMax - y1) / (y2 - y1);
				y = yMax;
			}
			else if (outCode & BOTTOM)
            {
				x = x1 + (x2 - x1) * (yMin - y1) / (y2 - y1);
				y = yMin;
			}
			else if (outCode & RIGHT)
            {
				y = y1 + (y2 - y1) * (xMax - x1) / (x2 - x1);
				x = xMax;
			}
			else if (outCode & LEFT)
            {
				y = y1 + (y2 - y1) * (xMin - x1) / (x2 - x1);
				x = xMin;
			}

			if (outCode == code1)
            {
				x1 = x;
				y1 = y;
				code1 = computeOutCode(x1, y1);
			}
			else
            {
				x2 = x;
				y2 = y;
				code2 = computeOutCode(x2, y2);
			}
		}
	}

	if (accept)
    {
		line(x1, y1, x2, y2, image, green);
	}
}

void cohenAlgo()
{
    TGAImage image(height, width, TGAImage::RGB);
    cohenSutherland(100, 300, 500, 900, image);
    cohenSutherland(500, 900, 800, 100, image);
    cohenSutherland(800, 100, 100, 300, image);

    drawRectangle(xMin, yMin, xMax, yMax, image, white);

    image.flip_vertically();
    image.write_tga_file("cohen.tga");
}

/* ---------------------------------------------------------- */

struct Point
{
    int x;
    int y;

    Point()
    {
        this->x = 0;
        this->y = 0;
    }

    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    Point operator=(const Point& point)
    {
        this->x = point.x;
        this->y = point.y;
    }

    friend Point operator-(const Point& p1, const Point& p2)
    {
        return Point(p1.x - p2.x, p1.y - p2.y);
    }

    friend Point operator+(const Point& p1, const Point& p2)
    {
        return Point(p1.x + p2.x, p1.y + p2.y);
    }

    Point operator*(double d)
    {
        this->x *= d;
        this->y *= d;
        return *this;
    }

    bool operator==(const Point& point)
    {
        return (this->x == point.x) && (this->y == point.y);
    }
};

struct Edge
{
    Point p;
    Point normal;
    Edge(Point p, Point normal)
    {
        this->p = p;
        this->normal = normal;
    }
};

vector<pair<int, int>> clipPoints = {
                                        {100, 100}, /* 1 */
                                        {900, 500}, /* 2 */
                                        {100, 900}, /* 3 */
                                    };

void drawClipWindow(TGAImage& image, TGAColor color)
{
    line(clipPoints[0].first, clipPoints[0].second, clipPoints[1].first, clipPoints[1].second, image, color);
    line(clipPoints[1].first, clipPoints[1].second, clipPoints[2].first, clipPoints[2].second, image, color);
    line(clipPoints[2].first, clipPoints[2].second, clipPoints[0].first, clipPoints[0].second, image, color);
}

int getScalarProduct(Point p1, Point p2)
{
    return p1.x * p2.x + p1.y * p2.y;
}

void CyrusBeck(int x1, int y1, int x2, int y2, vector<Edge> edges, TGAImage& image)
{
    Point p1(x1, y1);
    Point p2(x2, y2);

    if (p1 == p2)
    {
        return;
    }
    else
    {
        Point D = p2 - p1;
        double tInput = 0;
        double tOutput = 1;

        for (const auto& edge : edges)
        {

            double result = getScalarProduct(D, edge.normal);
            if (result != 0)
            {
                double t = -(getScalarProduct(p1 - edge.p, edge.normal)) / result;
                if (result < 0)
                {
                    if (t > tInput) tInput = t;
                }
                else
                {
                    if (t < tOutput) tOutput = t;
                }
            }
        }

        if (tInput < tOutput)
        {
            Point start = p1 + (p2 - p1) * tInput;
            Point finish = p1 + (p2 - p1) * tOutput;
            line(start.x, start.y, finish.x, finish.y, image, green);
        }
    }
}

void cyrusAlgo()
{
    TGAImage image(height, width, TGAImage::RGB);

    vector<Edge> edges;
    edges.push_back(Edge(
                    Point(clipPoints[0].first, clipPoints[0].second),
                    Point(clipPoints[1].second - clipPoints[0].second, clipPoints[0].first - clipPoints[1].first)
                    ));
    edges.push_back(Edge(
                    Point(clipPoints[1].first, clipPoints[1].second),
                    Point(clipPoints[2].second - clipPoints[1].second, clipPoints[1].first - clipPoints[2].first)
                    ));
    edges.push_back(Edge(
                    Point(clipPoints[2].first, clipPoints[2].second),
                    Point(clipPoints[0].second - clipPoints[2].second, clipPoints[2].first - clipPoints[0].first)
                    ));

    CyrusBeck(700, 900, 0, 500, edges, image);
    CyrusBeck(0, 500, 800, 100, edges, image);
    CyrusBeck(800, 100, 700, 900, edges, image);

    drawClipWindow(image, white);

    image.flip_vertically();
    image.write_tga_file("cyrus.tga");
}

/* ---------------------------------------------------------- */

bool clipParameter(int P, int Q, double& tInput, double& tOutput)
{
    if (P > 0)
    {
        double t = (double)Q / P;
        if (t > tOutput) return false; /* tInputMax > tOutputMin - line is outside */
        else if (t > tInput) tInput = t;
    }
    if (P < 0)
    {
        double t = (double)Q / P;
        if (t < tInput) return false; /* tInputMax > tOutputMin - line is outside */
        else if (t < tOutput) tOutput = t;
        else if (Q > 0) return false; /* line || edge */
    }
}

void LiangBarsky(int x1, int y1, int x2, int y2, TGAImage& image)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (dx == 0 && dy == 0)
    {
        return;
    }
    else
    {
        int startX = x1;
        int endX = x2;
        int startY = y1;
        int endY = y2;

        double tInput = 0.0;
        double tOutput = 1.0;
        bool test1 = clipParameter(dx, xMin - x1, tInput, tOutput);
        bool test2 = clipParameter(-dx, x1 - yMax, tInput, tOutput);
        bool test3 = clipParameter(dy, yMin - y1, tInput, tOutput);
        bool test4 = clipParameter(-dy, y1 - yMax, tInput, tOutput);
        if (test1 || test2 || test3 || test4)
        {
            if (tInput > 0)
            {
                startX += dx * tInput;
                startY += dy * tInput;
            }
            if (tOutput < 1)
            {
                endX = x1 + dx * tOutput;
                endY = y1 + dy * tOutput;
            }
        }

        line(startX, startY, endX, endY, image, green);
    }
}

void liangAlgo()
{
    TGAImage image(height, width, TGAImage::RGB);

    LiangBarsky(100, 500, 500, 900, image);
    LiangBarsky(500, 900, 900, 500, image);
    LiangBarsky(900, 500, 500, 100, image);
    LiangBarsky(500, 100, 100, 500, image);

    drawRectangle(xMin, yMin, xMax, yMax, image, white);

    image.flip_vertically();
    image.write_tga_file("liang.tga");
}

int main()
{
    cohenAlgo();
    cyrusAlgo();
    liangAlgo();
    return 0;
}
