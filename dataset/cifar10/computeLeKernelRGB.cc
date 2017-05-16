#include <float.h>
#include <math.h>
#include "computeLeKernelRGB.h"
#include <stdio.h>

// ================================================================================

double computeLeKernelRGB(double ***A, double ***B, int nFullyConnectedLayer)
{
  const int n0 = 32; //  input layer n0 x n0 units
  const int n1 = 28; //  first layer n1 x n1 feature maps 
  const int n2 = 14; // second layer n2 x n2 feature maps
  const int n3 = 10; //  third layer n3 x n3 feature maps
  const int n4 = 5;  // fourth layer n4 x n4 feature maps
  const int rf = 5;  // receptive field size
  
  double k0[n0][n0], u0[n0][n0], v0[n0][n0];
  double k1[n1][n1], u1[n1][n1], v1[n1][n1];
  double k2[n2][n2], u2[n2][n2], v2[n2][n2];
  double k3[n2][n2], u3[n3][n3], v3[n3][n3];
  double k4[n2][n2], u4[n4][n4], v4[n4][n4];
  double k5, u5, v5;
  double kAB, normAB;

  // INPUT LAYER
  for (int i=0; i<n0; i++)
    for (int j=0; j<n0; j++)
    {
      double *a = A[i][j];
      double *b = B[i][j];
      k0[i][j] = a[0]*b[0] + a[1]*b[1] + a[2]*b[2]; // RGB channels
      u0[i][j] = a[0]*a[0] + a[1]*a[1] + a[2]*a[2];
      v0[i][j] = b[0]*b[0] + b[1]*b[1] + b[2]*b[2];
    }
  
  // FIRST LAYER (CONVOLUTIONAL)
  for (int i=0; i<n1; i++)
    for (int j=0; j<n1; j++)
    {
      double kk=0, uu=0, vv=0;     
      for (int x=0; x<rf; x++)
        for (int y=0; y<rf; y++)
        {
          kk += k0[i+x][j+y];
          uu += u0[i+x][j+y];
          vv += v0[i+x][j+y];
        }     
      k1[i][j] = arcCosKernel(kk,sqrt(uu*vv));
      u1[i][j] = uu;
      v1[i][j] = vv;
    }

  // SECOND LAYER (SUBSAMPLING BY TWO)
  for (int i=0; i<n2; i++)
    for (int j=0; j<n2; j++)
    {
      int x = 2*i;
      int y = 2*j;
      k2[i][j] = k1[x][y] + k1[x+1][y] + k1[x][y+1] + k1[x+1][y+1];
      u2[i][j] = u1[x][y] + u1[x+1][y] + u1[x][y+1] + u1[x+1][y+1];
      v2[i][j] = v1[x][y] + v1[x+1][y] + v1[x][y+1] + v1[x+1][y+1];
    }
      
  // THIRD LAYER (CONVOLUTIONAL)
  for (int i=0; i<n3; i++)
    for (int j=0; j<n3; j++)
    {
      double kk=0, uu=0, vv=0;
      for (int x=0; x<rf; x++)
        for (int y=0; y<rf; y++)
        {
          kk += k2[i+x][j+y];
          uu += u2[i+x][j+y];
          vv += v2[i+x][j+y];
        }
      k3[i][j] = arcCosKernel(kk,sqrt(uu*vv));
      u3[i][j] = uu;
      v3[i][j] = vv;
    }

  // FOURTH LAYER (SUBSAMPLING BY TWO)
  for (int i=0; i<n4; i++)
    for (int j=0; j<n4; j++)
    {
      int x = 2*i;
      int y = 2*j;
      k4[i][j] = k3[x][y] + k3[x+1][y] + k3[x][y+1] + k3[x+1][y+1];
      u4[i][j] = u3[x][y] + u3[x+1][y] + u3[x][y+1] + u3[x+1][y+1];
      v4[i][j] = v3[x][y] + v3[x+1][y] + v3[x][y+1] + v3[x+1][y+1];
    }

  // FIFTH LAYER (FULLY CONNECTED)
  k5 = u5 = v5 = 0;
  for (int i=0; i<n4; i++)
    for (int j=0; j<n4; j++)
    {
      k5 += k4[i][j];
      u5 += u4[i][j];
      v5 += v4[i][j];
    }
  normAB = sqrt(u5*v5);
  kAB = arcCosKernel(k5,normAB);
  
  // ADDITIONAL LAYERS (FULLY CONNECTED)
  for (int layer=1; layer<nFullyConnectedLayer; layer++)
    kAB = arcCosKernel(kAB,normAB);
  return (kAB);
}

// ================================================================================

double arcCosKernel(double dotProdAB, double normAB)
{
  const double pi = 3.14159265358979323846;
  double cosTh, theta, acosK;

  normAB += DBL_MIN;
  if(normAB < dotProdAB){
    normAB = dotProdAB;
  }
  /*
  if (normAB==dotProdAB)                  // handles special cases of
    return (normAB);                      // zero norm and zero angle
  cosTh = dotProdAB/(normAB+DBL_MIN);     // avoid division by zero
  */
  cosTh = dotProdAB/normAB;
  theta = fastArcCosine(cosTh);           // theta = acos(cosTh);
  double sinTh = sqrt(fabs(1.0-cosTh*cosTh));
  acosK = (normAB*sinTh + dotProdAB*(pi-theta))/pi;
  if(isnan(acosK)){
    printf("nan\n");
  }

  return (acosK);
}

// ================================================================================

inline double fastArcCosine(double x)
{
  // Handbook of Mathematical Functions
  // Abramowitz & Stegun (page 82)
  // acos(x) for 0 <= x <= 1
  const double a0 = 1.5707963050;
  const double a1 = -0.2145988016;
  const double a2 = +0.0889789874;
  const double a3 = -0.0501743046;
  const double a4 = +0.0308918810;
  const double a5 = -0.0170881256;
  const double a6 = +0.0066700901;
  const double a7 = -0.0012624911;
  double y;
  
  y = sqrt(1.0-x)*(a0+(a1+(a2+(a3+(a4+(a5+(a6+a7*x)*x)*x)*x)*x)*x)*x);
  return (y);
}

// ================================================================================

