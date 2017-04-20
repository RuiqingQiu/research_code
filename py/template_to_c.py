import sys

print sys.argv

file = sys.argv[1]

layer_pixel = []
structure = []
count = 0
with open(file) as template_file:
    for line in template_file:
        if count == 0:
            tmp = line.split()
            for item in tmp:
                layer_pixel.append(int(item.split(':')[1]))
            print layer_pixel
            count += 1
        else:
            tmp = line.split(':')
            if len(tmp) != 2:
                continue
            structure.append(line.split(':')[1].rstrip())
print structure

# Generate c code
f = open('tmp.c', 'w')
# Write header
header = "#include <float.h>\n#include <math.h>\n#include \"computeLeKernelRGB.h\"\n#include <stdio.h>\n"
f.write(header)

define="#define MAX(a,b) ((a) > (b) ? a : b)\n#define MIN(a,b) ((a) < (b) ? a : b)\n"
f.write(define)

# the kernel function
f.write("double computeLeKernelRGB(double ***A, double ***B, int nFullyConnectedLayer)\n{\n")
count = 0
f.write("\tdouble kAB, normAB;\n")
count = 0
for maps in layer_pixel:
    if count == len(layer_pixel) - 1:
        f.write("\tconst int rf = " + str(maps) + ";\n")
    else:
        f.write("\tconst int n" + str(count) + " = " + str(maps)+";\n")
        f.write("\tdouble k"+ str(count) + "[n" + str(count) + "][n" + str(count)+"];\n")
        f.write("\tdouble v"+ str(count) + "[n" + str(count) + "][n" + str(count)+"];\n")
        f.write("\tdouble u"+ str(count) + "[n" + str(count) + "][n" + str(count)+"];\n")
    count += 1
f.write("\tdouble k, u, v;\n")

# input layer
f.write("\tfor (int i=0; i<n0; i++){for (int j=0; j<n0; j++){double *a = A[i][j];double *b = B[i][j];k0[i][j] = a[0]*b[0] + a[1]*b[1] + a[2]*b[2]; u0[i][j] = a[0]*a[0] + a[1]*a[1] + a[2]*a[2];v0[i][j] = b[0]*b[0] + b[1]*b[1] + b[2]*b[2];}}")

def writeConv(num):
    string = "\t //Convolutional Layer\n"
    string += "for (int i=0; i<n" + str(num) + "; i++){"
    string += "for (int j=0; j<n" + str(num) + "; j++){"
    string += "double kk=0, uu=0, vv=0;"
    string += "for (int x=0; x<rf; x++)"
    string += "for (int y=0; y<rf; y++){"
    string += "kk += k" + str(num-1) + "[i+x][j+y];"
    string += "uu += u" + str(num-1) + "[i+x][j+y];"
    string += "vv += v" + str(num-1) + "[i+x][j+y];}"
    string += "k" + str(num) + "[i][j] = arcCosKernel(kk,sqrt(uu*vv));"
    string += "u" + str(num) + "[i][j] = uu;"
    string += "v" + str(num) + "[i][j] = vv;}}\n\n"
    return string

def writeSub(num):
    string = "\t //Subsampling by 2\n"
    string += "for (int i=0; i<n" + str(num) + "; i++){"
    string += "for (int j=0; j<n" + str(num) + "; j++){"
    string += "int x = 2*i;"
    string += "int y = 2*j;"
    string += "k" + str(num) + "[i][j] = k" + str(num-1) + "[x][y] + k" + str(num-1) + "[x+1][y] + k" + str(num-1) + "[x][y+1] + k" + str(num-1) + "[x+1][y+1];"
    string += "u" + str(num) + "[i][j] = u" + str(num-1) + "[x][y] + u" + str(num-1) + "[x+1][y] + u" + str(num-1) + "[x][y+1] + u" + str(num-1) + "[x+1][y+1];"
    string += "v" + str(num) + "[i][j] = v" + str(num-1) + "[x][y] + v" + str(num-1) + "[x+1][y] + v" + str(num-1) + "[x][y+1] + v" + str(num-1) + "[x+1][y+1];}}\n\n"
    return string

def writeMax(num):
    string = "\t //Maxpooling by 2\n"
    string += "for (int i=0; i<n" + str(num) + "; i++){"
    string += "for (int j=0; j<n" + str(num) + "; j++){"
    string += "int x = 2*i;"
    string += "int y = 2*j;"
    string += "k" + str(num) + "[i][j] =MAX(MAX(MAX(k" + str(num-1) + "[x][y], k" + str(num-1) + "[x+1][y]), k" + str(num-1) + "[x][y+1]), k" + str(num-1) + "[x+1][y+1]);"
    string += "u" + str(num) + "[i][j] =MAX(MAX(MAX(u" + str(num-1) + "[x][y], u" + str(num-1) + "[x+1][y]), u" + str(num-1) + "[x][y+1]), u" + str(num-1) + "[x+1][y+1]);"
    string += "v" + str(num) + "[i][j] =MAX(MAX(MAX(v" + str(num-1) + "[x][y], v" + str(num-1) + "[x+1][y]), v" + str(num-1) + "[x][y+1]), v" + str(num-1) + "[x+1][y+1]);}}\n\n"
    return string

def writeFully(num):
    string = "\t //Fully Connected Layer\n"
    string += "k = u = v = 0;"
    string += "for (int i=0; i<n" + str(num-1) + "; i++){"
    string += "for (int j=0; j<n" + str(num-1) + "; j++){"
    string += "k += k" + str(num-1) + "[i][j];"
    string += "u += u" + str(num-1) + "[i][j];"
    string += "v += v" + str(num-1) + "[i][j];}}"
    string += "\tnormAB = sqrt(u*v);\n"
    string += "\tkAB = arcCosKernel(k,normAB);\n"
    string += "\treturn (kAB);\n"
    return string

count = 1
for s in structure:
    if s == 'CONV':
        f.write(writeConv(count))
    elif s == 'SUB':
        f.write(writeSub(count))
    elif s == 'FULLY':
        f.write(writeFully(count))
    elif s == 'MAX':
        f.write(writeMax(count))
    else:
        print "error, structure unknown"
    count += 1

f.write("}\n")




# Normal ArcCosine Function
ArcCosineCode = "double arcCosKernel(double dotProdAB, double normAB){const double pi = 3.14159265358979323846;double cosTh, theta, acosK;normAB += DBL_MIN;if(normAB < dotProdAB){normAB = dotProdAB;}cosTh = dotProdAB/normAB;theta = fastArcCosine(cosTh);double sinTh = sqrt(fabs(1.0-cosTh*cosTh));acosK = (normAB*sinTh + dotProdAB*(pi-theta))/pi;if(isnan(acosK)){printf(\"nan\\n\");}return (acosK);}\n"

f.write(ArcCosineCode)
# Write FastArcCosine Function
fastArcCosineCode = "inline double fastArcCosine(double x){const double a0 = 1.5707963050;const double a1 = -0.2145988016;const double a2 = +0.0889789874;const double a3 = -0.0501743046;const double a4 = +0.0308918810;const double a5 = -0.0170881256;const double a6 = +0.0066700901;const double a7 = -0.0012624911;double y; y = sqrt(1.0-x)*(a0+(a1+(a2+(a3+(a4+(a5+(a6+a7*x)*x)*x)*x)*x)*x)*x);return (y);}\n"
f.write(fastArcCosineCode)



