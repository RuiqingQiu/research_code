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

# the kernel function
f.write("double computeLeKernelRGB(double ***A, double ***B, int nFullyConnectedLayer\n{\n")
count = 0
f.write("double kAB, normAB;\n")
for maps in layer_pixel:
    f.write("\tconst int n" + str(count) + " = " + str(maps)+";\n")
    f.write("\tdouble k"+ str(count) + "[n" + str(count) + "][n" + str(count)+"];\n")
    f.write("\tdouble v"+ str(count) + "[n" + str(count) + "][n" + str(count)+"];\n")
    f.write("\tdouble u"+ str(count) + "[n" + str(count) + "][n" + str(count)+"];\n")
    count += 1




# Normal ArcCosine Function
ArcCosineCode = "double arcCosKernel(double dotProdAB, double normAB){const double pi = 3.14159265358979323846;double cosTh, theta, acosK;normAB += DBL_MIN;if(normAB < dotProdAB){normAB = dotProdAB;}cosTh = dotProdAB/normAB;theta = fastArcCosine(cosTh);double sinTh = sqrt(fabs(1.0-cosTh*cosTh));acosK = (normAB*sinTh + dotProdAB*(pi-theta))/pi;if(isnan(acosK)){printf(\"nan\\n\");}return (acosK);}\n"

f.write(ArcCosineCode)
# Write FastArcCosine Function
fastArcCosineCode = "inline double fastArcCosine(double x){const double a0 = 1.5707963050;const double a1 = -0.2145988016;const double a2 = +0.0889789874;const double a3 = -0.0501743046;const double a4 = +0.0308918810;const double a5 = -0.0170881256;const double a6 = +0.0066700901;const double a7 = -0.0012624911;double y; y = sqrt(1.0-x)*(a0+(a1+(a2+(a3+(a4+(a5+(a6+a7*x)*x)*x)*x)*x)*x)*x);return (y);}\n"
f.write(fastArcCosineCode)



