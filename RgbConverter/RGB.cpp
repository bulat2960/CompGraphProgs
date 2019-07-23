#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

double min(double x, double y, double z)
{
    double minValue = INT_MAX;
	if (x >= y && z >= y) minValue = y;
	if (y >= x && z >= x) minValue = x;
	if (y >= z && x >= z) minValue = z;
    return minValue;
}

double max(double x, double y, double z)
{
    double maxValue = INT_MIN;
	if (x <= y && z <= y) maxValue = y;
	if (y <= x && z <= x) maxValue = x;
	if (y <= z && x <= z) maxValue = z;
    return maxValue;
}

void RgbToCmyk(double R, double G, double B) // http://www.rapidtables.com/convert/color/rgb-to-cmyk.htm
{
    double K = 1 - max(R, G, B);

    double C, M, Y;
    if (K == 1)
    {
        C = 0;
        M = 0;
        Y = 0;
    }
    else
    {
        C = (1 - R - K) / (1 - K);
        M = (1 - G - K) / (1 - K);
        Y = (1 - B - K) / (1 - K);
    }

    cout << "C = " << C << ", " << "M = " << M << ", " << "Y = " << Y << ", " << "K = " << K << endl << endl;
}

double RgbToXyzHelper(double K)
{
    const double a = 0.055;
    const double mu = 2.2;

    double value = 0.0;
    if (K > 0.04045)
    {
        value = pow((K + a) / (1 + a), mu);
    }
    else
    {
        value = K / 12.92;
    }
    return value;
}

void RgbToXyz(double R, double G, double B)
{
    vector<vector<double>> matrix = {{0.4124, 0.3576, 0.1805},
                                     {0.2126, 0.7152, 0.0722},
                                     {0.0193, 0.1192, 0.9505}};


    double Gr = RgbToXyzHelper(R);
    double Gg = RgbToXyzHelper(G);
    double Gb = RgbToXyzHelper(B);

    double X = matrix[0][0] * Gr + matrix[0][1] * Gg + matrix[0][2] * Gb;
    double Y = matrix[1][0] * Gr + matrix[1][1] * Gg + matrix[1][2] * Gb;
    double Z = matrix[2][0] * Gr + matrix[2][1] * Gg + matrix[2][2] * Gb;



	cout << "X = " << X << ", " << "Y = " << Y << ", " << "Z = " << Z << endl << endl;
}

void RgbToHsv(double R, double G, double B)
{
	double H = 0;
	double S = 0;
	double V = 0;
	double minValue = min(R, G, B);
    double maxValue = max(R, G, B);

    // Calculate V
    V = maxValue;

    // Calculate H
	if (maxValue == minValue)
    {
		H = 0;
    }
	if (maxValue == R && G >= B)
    {
		H = 60 * (G - B) / (maxValue - minValue);
	}
	if (maxValue == R && G < B)
    {
		H = 60 * (G - B) / (maxValue - minValue) + 360;
	}
	if (maxValue == G)
	{
		H = 60 * (B - R) / (maxValue - minValue) + 120;
	}
	if (maxValue == B)
	{
		H = 60 * (R - G) / (maxValue - minValue) + 240;
	}

	// Calculate S
	if (maxValue == 0)
    {
		S = 0;
    }
	else
    {
		S = 1 - (minValue / maxValue);
    }

	cout << "H = " << H << ", " << "S = " << S << ", " << "V = " << V << endl << endl;
}


void RgbToHls(double R, double G, double B)
{
	double H = 0;
	double L = 0;
	double S = 0;
	double minValue = min(R, G, B);
	double maxValue = max(R, G, B);

	// Calculate H
	if (minValue == maxValue)
    {
		H = 0;
    }
	if (maxValue == R && G >= B)
    {
        H = 60 * (G - B) / (maxValue - minValue);
	}
	if (maxValue == R && G < B)
    {
		H = 60 * (G - B) / (maxValue - minValue) + 360;
	}
	if (maxValue == G)
	{
		H = 60 * (B - R) / (maxValue - minValue) + 120;
	}
	if (maxValue == B)
	{
		H = 60 * (R - G) / (maxValue - minValue) + 240;
	}

	// Calculate L
	L = (maxValue + minValue) / 2;

	// Calculate S
	if (L == 0 || maxValue == minValue)
    {
		S = 0;
    }
	if (L > 0 && L <= 0.5)
    {
		S = (maxValue - minValue) / (2 * L);
    }
	if (L > 0.5)
    {
		S = (maxValue - minValue) / (2 - 2 * L);
    }

	cout << "H = " << H << ", " << "S = " << S << ", " << "L = " << L << endl << endl;
}

int main()
{
	double R, G, B;

	const int maxColorValue = 255;

	cout << "Enter R: ";
	cin >> R;
	cout << "Enter G: ";
	cin >> G;
	cout << "Enter B: ";
	cin >> B;

	R /= maxColorValue;
	G /= maxColorValue;
	B /= maxColorValue;

	cout << "RgbToCmyk test" << endl;
	RgbToCmyk(R, G, B);

	cout << "RgbToXyz test" << endl;
	RgbToXyz(R, G, B);

	cout << "RgbToHsv test" << endl;
	RgbToHsv(R, G, B);

	cout << "RgbToHls test" << endl;
	RgbToHls(R, G, B);
}
