#include <vector>
#include <cmath>
#include <stack>
#include <iostream>
#include <cstdlib>
#include <map>
#include <algorithm>
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

using namespace std;

const int width  = 1000;
const int height = 1000;

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

void iterAlgo()
{
    /*TGAImage image(height, width, TGAImage::RGB);
    line(100, 500, 600, 400, image, green);
    line(600, 400, 700, 300, image, green);
    line(700, 300, 800, 500, image, green);
    line(800, 500, 900, 600, image, green);
    line(900, 600, 100, 500, image, green);*/

    vector<pair<int, int>> points;
    points.push_back({200, 600});
    points.push_back({300, 700});
    points.push_back({500, 400});
    points.push_back({600, 450});
    points.push_back({700, 200});

    TGAImage image(height, width, TGAImage::RGB);
    line(points[0].first, points[0].second, points[1].first, points[1].second, image, green);
    line(points[1].first, points[1].second, points[2].first, points[2].second, image, green);
    line(points[2].first, points[2].second, points[3].first, points[3].second, image, green);
    line(points[3].first, points[3].second, points[4].first, points[4].second, image, green);
    line(points[4].first, points[4].second, points[0].first, points[0].second, image, green);

    stack<pair<int, int>> s;
    s.push({300, 600});

    while (!s.empty())
    {
        pair<int, int> currentPoint = s.top();

        image.set(currentPoint.first, currentPoint.second, white);

        TGAColor color1 = image.get(currentPoint.first, currentPoint.second + 1); // UP
        TGAColor color2 = image.get(currentPoint.first + 1, currentPoint.second); // RIGHT
        TGAColor color3 = image.get(currentPoint.first, currentPoint.second - 1); // DOWN
        TGAColor color4 = image.get(currentPoint.first - 1, currentPoint.second); // LEFT

        if (color1 != green && color1 != white) // UP
        {
            s.push({currentPoint.first, currentPoint.second + 1});
        }
        else if (color2 != green && color2 != white) // RIGHT
        {
            s.push({currentPoint.first + 1, currentPoint.second});
        }
        else if (color3 != green && color3 != white) // DOWN
        {
            s.push({currentPoint.first, currentPoint.second - 1});
        }
        else if (color4 != green && color4 != white) // LEFT
        {
            s.push({currentPoint.first - 1, currentPoint.second});
        }
        else
        {
            s.pop();
        }
    }

    image.flip_vertically();
    image.write_tga_file("iterAlgo.tga");
}

void stringAlgo()
{
    vector<pair<int, int>> points;
    points.push_back({200, 600});
    points.push_back({300, 700});
    points.push_back({500, 400});
    points.push_back({600, 450});
    points.push_back({700, 200});

    TGAImage image(height, width, TGAImage::RGB);
    line(points[0].first, points[0].second, points[1].first, points[1].second, image, green);
    line(points[1].first, points[1].second, points[2].first, points[2].second, image, green);
    line(points[2].first, points[2].second, points[3].first, points[3].second, image, green);
    line(points[3].first, points[3].second, points[4].first, points[4].second, image, green);
    line(points[4].first, points[4].second, points[0].first, points[0].second, image, green);

    stack<pair<int, int>> s;
    s.push({499, 400});

    while (!s.empty())
    {
        pair<int, int> p = s.top();
        s.pop();

        int y = p.second;

        int xMin = p.first;
        while (image.get(xMin - 1, y) != green)
        {
            xMin--;
        }

        int xMax = p.first;
        while (image.get(xMax + 1, y) != green)
        {
            xMax++;
        }

        for (int i = xMin; i <= xMax; i++)
        {
            image.set(i, y, red);
        }

        for (int i = xMin; i <= xMax; i++)
        {
            if (image.get(i, y - 1) != green && image.get(i, y - 1) != red)
            {
                s.push({i, y - 1});
                break;
            }
        }

        for (int i = xMin; i <= xMax; i++)
        {
            if (image.get(i, y + 1) != green && image.get(i, y + 1) != red)
            {
                s.push({i, y + 1});
                break;
            }
        }
    }

    image.flip_vertically();
    image.write_tga_file("stringAlgo.tga");
}

void rebPoints()
{
    vector<pair<int, int>> points;
    points.push_back({200, 600});
    points.push_back({300, 700});
    points.push_back({500, 400});
    points.push_back({600, 450});
    points.push_back({700, 200});

    TGAImage image(height, width, TGAImage::RGB);
    line(points[0].first, points[0].second, points[1].first, points[1].second, image, green);
    line(points[1].first, points[1].second, points[2].first, points[2].second, image, green);
    line(points[2].first, points[2].second, points[3].first, points[3].second, image, green);
    line(points[3].first, points[3].second, points[4].first, points[4].second, image, green);
    line(points[4].first, points[4].second, points[0].first, points[0].second, image, green);

    /* FILL */
    map<int, vector<int>> greenPoints;
    for (int y = 0; y < 1000; y++)
    {
        for (int x = 0; x < 1000; x++)
        {
            if (image.get(x, y) == green)
            {
                greenPoints[y].push_back(x);
            }
        }
    }

    /* PARSING */

    for (auto& point : greenPoints)
    {
        vector<int>& xPoints = point.second;

        int counter = 0;
        while (counter < xPoints.size())
        {
            if (xPoints[counter + 1] - xPoints[counter] == 1)
            {
                xPoints.erase(xPoints.begin() + counter, xPoints.begin() + counter + 1);
            }
            else
            {
                counter++;
            }
        }

        if (xPoints.size() == 1)
        {
            xPoints.clear();
        }

        int pointPos = 0;
        if (xPoints.size() % 2 == 1)
        {
            for (int i = 0; i < points.size(); i++)
            {
                for (int j = 0; j < xPoints.size(); j++)
                {
                    if (xPoints[j] == points[i].first + 1 || xPoints[j] == points[i].first - 1 || xPoints[j] == points[i].first)
                    {
                        pointPos = j;
                    }
                }
            }
            xPoints.erase(xPoints.begin() + pointPos, xPoints.begin() + pointPos + 1);
        }
    }

    /* DRAW */
    for (const auto& point : greenPoints)
    {
        if (point.second.size() > 0)
        {
            for (int i = 0; i < point.second.size(); i += 2)
            {
                int border1 = point.second[i];
                int border2 = point.second[i + 1];
                for (int j = border1 + 1; j < border2; j++)
                {
                    image.set(j, point.first, red);
                }
            }
        }
    }

    image.flip_vertically();
    image.write_tga_file("rebPoints.tga");
}

void capAlgo()
{
    vector<pair<int, int>> points;
    points.push_back({200, 600});
    points.push_back({300, 700});
    points.push_back({500, 400});
    points.push_back({600, 500});
    points.push_back({700, 200});

    TGAImage image(height, width, TGAImage::RGB);
    line(points[0].first, points[0].second, points[1].first, points[1].second, image, green);
    line(points[1].first, points[1].second, points[2].first, points[2].second, image, green);
    line(points[2].first, points[2].second, points[3].first, points[3].second, image, green);
    line(points[3].first, points[3].second, points[4].first, points[4].second, image, green);
    line(points[4].first, points[4].second, points[0].first, points[0].second, image, green);

    struct S
    {
        int y;
        int yMax;
        double dx;
        double x;
    };

    vector<S> yList;
    for (int i = 0; i < points.size(); i++)
    {
        int y = points[i].second;
        int yMax = points[(i + 1) % points.size()].second;
        if (y > yMax) swap(y, yMax);
        double dx = (points[(i + 1) % points.size()].first - points[i].first)
                            / (points[(i + 1) % points.size()].second - points[i].second);
        double x = points[i].first + dx * (y - points[i].second);
        yList.push_back({y, yMax, dx, x});
    }

    sort(yList.begin(), yList.end(), [&yList](S s1, S s2){return s1.y < s2.y;});

    vector<S> cap;
    int y = yList[0].y;

    do
    {
        int counter = 0;
        while (counter < yList.size())
        {
            if (yList[counter].y == y)
            {
                cap.push_back(yList[counter]);
                yList.erase(yList.begin() + counter, yList.begin() + counter + 1);
            }
            else
            {
                counter++;
            }
        }

        sort(cap.begin(), cap.end(), [&cap](S s1, S s2){return s1.x < s2.x;});

        vector<int> greenPoints;
        for (int x = 0; x < 1000; x++)
        {
            if (image.get(x, y) == green)
            {
                greenPoints.push_back(x);
            }
        }

        counter = 0;
        while (counter < greenPoints.size())
        {
            if (greenPoints[counter + 1] - greenPoints[counter] == 1)
            {
                greenPoints.erase(greenPoints.begin() + counter, greenPoints.begin() + counter + 1);
            }
            else
            {
                counter++;
            }
        }

        int pointPos = 0;
        if (greenPoints.size() % 2 != 0)
        {
            for (int i = 0; i < points.size(); i++)
            {
                for (int j = 0; j < greenPoints.size(); j++)
                {
                    if (greenPoints[j] == points[i].first + 1 || greenPoints[j] == points[i].first - 1
                        || greenPoints[j] == points[i].first)
                    {
                        pointPos = j;
                    }
                }
            }
            greenPoints.erase(greenPoints.begin() + pointPos, greenPoints.begin() + pointPos + 1);
        }

        cout << y << ' ' << cap.size() << ' ' << greenPoints.size() << endl;

        for (int i = 0; i < greenPoints.size(); i += 2)
        {
            int border1 = greenPoints[i];
            int border2 = greenPoints[i + 1];
            for (int j = border1 + 1; j < border2; j++)
            {
                image.set(j, y, red);
            }
        }

        y++;

        int i = 0;
        while (i < cap.size())
        {
            if (y > cap[i].yMax)
            {
                cap.erase(cap.begin() + i, cap.begin() + i + 1);
            }
            else
            {
                cap[i].x += cap[i].dx;
                sort(cap.begin(), cap.end(), [&cap](S s1, S s2){return s1.x < s2.x;});
                i++;
            }
        }
    }
    while (!cap.empty());

    image.flip_vertically();
    image.write_tga_file("capAlgo.tga");
}

void newRebPoints()
{
    vector<pair<int, int>> points;
    points.push_back({200, 600});
    points.push_back({300, 700});
    points.push_back({500, 400});
    points.push_back({600, 500});
    points.push_back({700, 200});

    TGAImage image(height, width, TGAImage::RGB);
    line(points[0].first, points[0].second, points[1].first, points[1].second, image, green);
    line(points[1].first, points[1].second, points[2].first, points[2].second, image, green);
    line(points[2].first, points[2].second, points[3].first, points[3].second, image, green);
    line(points[3].first, points[3].second, points[4].first, points[4].second, image, green);
    line(points[4].first, points[4].second, points[0].first, points[0].second, image, green);

    struct Rebro
    {
        int x1;
        int y1;
        int x2;
        int y2;
    };

    vector<Rebro> rebra;
    for (int i = 0; i < points.size() - 1; i++)
    {
        rebra.push_back({points[i].first, points[i].second, points[i + 1].first, points[i + 1].second});
    }
    rebra.push_back({points[points.size() - 1].first, points[points.size() - 1].second, points[0].first, points[0].second});

    map<int, vector<int>> greenPoints;

    for (auto& rebro : rebra)
    {
        int y = rebro.y1;
        double dx = (double)(rebro.x2 - rebro.x1) / (rebro.y2 - rebro.y1);
        double x = rebro.x1;

        if (y <= rebro.y2 && x <= rebro.x2)
        {
            while (y <= rebro.y2)
            {
                greenPoints[y].push_back(x);
                y++;
                x += dx;
            }
        }
        if (y >= rebro.y2 && x <= rebro.x2)
        {
            while (y >= rebro.y2)
            {
                greenPoints[y].push_back(x);
                y--;
                x -= dx;
            }
        }
        if (y <= rebro.y2 && x >= rebro.x2)
        {
            while (y <= rebro.y2)
            {
                greenPoints[y].push_back(x);
                y++;
                x += dx;
            }
        }
        if (y >= rebro.y2 && x >= rebro.x2)
        {
            while (y >= rebro.y2)
            {
                greenPoints[y].push_back(x);
                y--;
                x -= dx;
            }
        }
    }

    for (auto& point : greenPoints)
    {
        vector<int>& xPoints = point.second;

        int counter = 0;
        while (counter < xPoints.size())
        {
            if (xPoints[counter + 1] - xPoints[counter] == 1)
            {
                xPoints.erase(xPoints.begin() + counter, xPoints.begin() + counter + 1);
            }
            else
            {
                counter++;
            }
        }

        if (xPoints.size() == 1)
        {
            xPoints.clear();
        }

        int pointPos = 0;
        if (xPoints.size() % 2 == 1)
        {
            for (int i = 0; i < points.size(); i++)
            {
                for (int j = 0; j < xPoints.size(); j++)
                {
                    if (xPoints[j] == points[i].first + 1 || xPoints[j] == points[i].first - 1 || xPoints[j] == points[i].first)
                    {
                        pointPos = j;
                    }
                }
            }
            xPoints.erase(xPoints.begin() + pointPos, xPoints.begin() + pointPos + 1);
        }
    }

    for (auto& point : greenPoints)
    {
        sort(point.second.begin(), point.second.end());
    }

    for (const auto& point : greenPoints)
    {
        if (point.second.size() > 0)
        {
            for (int i = 0; i < point.second.size(); i += 2)
            {
                int border1 = point.second[i];
                int border2 = point.second[i + 1];
                for (int j = border1; j <= border2 + 1; j++)
                {
                    image.set(j, point.first, red);
                }
            }
        }
    }

    image.flip_vertically();
    image.write_tga_file("newRebPoints.tga");
}

int main()
{
    iterAlgo();
    stringAlgo();
    capAlgo();

    rebPoints();

    newRebPoints();


    return 0;
}

