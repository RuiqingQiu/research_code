#include <float.h>
#include <math.h>
#include "computeLeKernelRGB.h"
#include <stdio.h>
#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)
double computeLeKernelRGB(double ***A, double ***B, int nFullyConnectedLayer)
{
	double kAB, normAB;
	const int n0 = 32;
	double k0[n0][n0];
	double v0[n0][n0];
	double u0[n0][n0];
	const int n1 = 30;
	double k1[n1][n1];
	double v1[n1][n1];
	double u1[n1][n1];
	const int n2 = 28;
	double k2[n2][n2];
	double v2[n2][n2];
	double u2[n2][n2];
	const int n3 = 26;
	double k3[n3][n3];
	double v3[n3][n3];
	double u3[n3][n3];
	const int n4 = 24;
	double k4[n4][n4];
	double v4[n4][n4];
	double u4[n4][n4];
	const int n5 = 12;
	double k5[n5][n5];
	double v5[n5][n5];
	double u5[n5][n5];
	const int n6 = 10;
	double k6[n6][n6];
	double v6[n6][n6];
	double u6[n6][n6];
	const int n7 = 5;
	double k7[n7][n7];
	double v7[n7][n7];
	double u7[n7][n7];
	const int rf = 3;
	double k, u, v;
	for (int i=0; i<n0; i++){for (int j=0; j<n0; j++){double *a = A[i][j];double *b = B[i][j];k0[i][j] = a[0]*b[0] + a[1]*b[1] + a[2]*b[2]; u0[i][j] = a[0]*a[0] + a[1]*a[1] + a[2]*a[2];v0[i][j] = b[0]*b[0] + b[1]*b[1] + b[2]*b[2];}}	 //Convolutional Layer
for (int i=0; i<n1; i++){for (int j=0; j<n1; j++){double kk=0, uu=0, vv=0;for (int x=0; x<rf; x++)for (int y=0; y<rf; y++){kk += k0[i+x][j+y];uu += u0[i+x][j+y];vv += v0[i+x][j+y];}k1[i][j] = arcCosKernel(kk,sqrt(uu*vv));u1[i][j] = uu;v1[i][j] = vv;}}

	 //Convolutional Layer
for (int i=0; i<n2; i++){for (int j=0; j<n2; j++){double kk=0, uu=0, vv=0;for (int x=0; x<rf; x++)for (int y=0; y<rf; y++){kk += k1[i+x][j+y];uu += u1[i+x][j+y];vv += v1[i+x][j+y];}k2[i][j] = arcCosKernel(kk,sqrt(uu*vv));u2[i][j] = uu;v2[i][j] = vv;}}

	 //Convolutional Layer
for (int i=0; i<n3; i++){for (int j=0; j<n3; j++){double kk=0, uu=0, vv=0;for (int x=0; x<rf; x++)for (int y=0; y<rf; y++){kk += k2[i+x][j+y];uu += u2[i+x][j+y];vv += v2[i+x][j+y];}k3[i][j] = arcCosKernel(kk,sqrt(uu*vv));u3[i][j] = uu;v3[i][j] = vv;}}

	 //Convolutional Layer
for (int i=0; i<n4; i++){for (int j=0; j<n4; j++){double kk=0, uu=0, vv=0;for (int x=0; x<rf; x++)for (int y=0; y<rf; y++){kk += k3[i+x][j+y];uu += u3[i+x][j+y];vv += v3[i+x][j+y];}k4[i][j] = arcCosKernel(kk,sqrt(uu*vv));u4[i][j] = uu;v4[i][j] = vv;}}

	 //Subsampling by 2
for (int i=0; i<n5; i++){for (int j=0; j<n5; j++){int x = 2*i;int y = 2*j;k5[i][j] = k4[x][y] + k4[x+1][y] + k4[x][y+1] + k4[x+1][y+1];u5[i][j] = u4[x][y] + u4[x+1][y] + u4[x][y+1] + u4[x+1][y+1];v5[i][j] = v4[x][y] + v4[x+1][y] + v4[x][y+1] + v4[x+1][y+1];}}

	 //Convolutional Layer
for (int i=0; i<n6; i++){for (int j=0; j<n6; j++){double kk=0, uu=0, vv=0;for (int x=0; x<rf; x++)for (int y=0; y<rf; y++){kk += k5[i+x][j+y];uu += u5[i+x][j+y];vv += v5[i+x][j+y];}k6[i][j] = arcCosKernel(kk,sqrt(uu*vv));u6[i][j] = uu;v6[i][j] = vv;}}

	 //Subsampling by 2
for (int i=0; i<n7; i++){for (int j=0; j<n7; j++){int x = 2*i;int y = 2*j;k7[i][j] = k6[x][y] + k6[x+1][y] + k6[x][y+1] + k6[x+1][y+1];u7[i][j] = u6[x][y] + u6[x+1][y] + u6[x][y+1] + u6[x+1][y+1];v7[i][j] = v6[x][y] + v6[x+1][y] + v6[x][y+1] + v6[x+1][y+1];}}

	 //Fully Connected Layer
k = u = v = 0;for (int i=0; i<n7; i++){for (int j=0; j<n7; j++){k += k7[i][j];u += u7[i][j];v += v7[i][j];}}	normAB = sqrt(u*v);
	kAB = arcCosKernel(k,normAB);
	return (kAB);
}
double arcCosKernel(double dotProdAB, double normAB){const double pi = 3.14159265358979323846;double cosTh, theta, acosK;normAB += DBL_MIN;if(normAB < dotProdAB){normAB = dotProdAB;}cosTh = dotProdAB/normAB;theta = fastArcCosine(cosTh);double sinTh = sqrt(fabs(1.0-cosTh*cosTh));acosK = (normAB*sinTh + dotProdAB*(pi-theta))/pi;if(isnan(acosK)){printf("nan\n");}return (acosK);}
inline double fastArcCosine(double x){const double a0 = 1.5707963050;const double a1 = -0.2145988016;const double a2 = +0.0889789874;const double a3 = -0.0501743046;const double a4 = +0.0308918810;const double a5 = -0.0170881256;const double a6 = +0.0066700901;const double a7 = -0.0012624911;double y; y = sqrt(1.0-x)*(a0+(a1+(a2+(a3+(a4+(a5+(a6+a7*x)*x)*x)*x)*x)*x)*x);return (y);}
