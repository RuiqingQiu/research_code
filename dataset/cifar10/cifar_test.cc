#include <stdio.h>
#include <stdlib.h>
#include "computeLeKernelRGB.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string.h>
#include <cstdlib>
// ================================
using namespace std;
void split(const string& s, char c,
        vector<string>& v) {
    string::size_type i = 0;
    string::size_type j = s.find(c);

    while (j != string::npos) {
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);

        if (j == string::npos)
            v.push_back(s.substr(i, s.length()));
    }
}
int main(int argc, char* argv[])
{
    const int n=32;
    int number_of_images = 1000;
    ifstream file("/oasis/tscc/scratch/rqiu/cifar_train_no_colon.txt");

    double ****A, ****B;
    double kAB;
    A = new double***[number_of_images];
    for(int k = 0; k < number_of_images; k++){
        A[k] = new double**[32];
        for(int i = 0; i < 32; i++){
            A[k][i] = new double*[32];
            for(int j = 0; j < 32; j++){
                A[k][i][j] = new double[3];
            }
        }
    }
    B = new double***[number_of_images];
    for(int k = 0; k < number_of_images; k++){
        B[k] = new double**[32];
        for(int i = 0; i < 32; i++){
            B[k][i] = new double*[32];
            for(int j = 0; j < 32; j++){
                B[k][i][j] = new double[3];
            }
        }
    }

    clock_t start;
    start = clock();
    string str;
    int line_number = 0;
    while(getline(file, str)){
        if(line_number >= number_of_images){
            break;
        }
        vector<string> v;
        split(str, ' ', v);
        int index = 0; 
        for(int k = 0; k < 3; k++){
            for(int i = 0; i < 32; i++){
                for(int j = 0; j < 32; j++){
                    A[line_number][i][j][k] = atof(v[index].c_str());
                    index++;
                }
            }
        }
        line_number++;
        /*
        kAB = computeLeKernelRGB(A[0], A[0]);

        for(int k = 0; k < 3; k++){
            for(int i = 0; i < 32; i++){
                for(int j = 0; j < 32; j++){
                    cout << A[0][i][j][k] << " ";
                }
            }
            cout << endl;
        }
        printf("kAB = %f\n", kAB);

        break;
        */
    }
    ifstream test_file("/oasis/tscc/scratch/rqiu/cifar_test_no_colon.txt");
    line_number = 0;
    while(getline(test_file, str)){
        if(line_number >= number_of_images){
            break;   
        }
        vector<string> v;
        split(str, ' ', v);
        int index = 0;
        for(int k = 0; k < 3; k++){
            for(int i = 0; i < 32; i++){
                for(int j = 0; j < 32; j++){
                    B[line_number][i][j][k] = atof(v[index].c_str());
                    index++;
                }
            }
        }
        line_number++;
    }
    int* label = new int[number_of_images];
    ifstream label_file("cifar_test_label.txt");
    line_number = 0;
    while(getline(label_file, str)){
        if(line_number >= number_of_images){
            break;
        }
        label[line_number] = atof(str.c_str());
        line_number++;
    }
    ofstream outfile;
    outfile.open("/oasis/tscc/scratch/rqiu/cifar_test_libsvm.txt");
    for(int i = 0; i < number_of_images; i++){
        outfile << label[i] << " " << "0:" << (i+1) << " ";
        for(int j = 0; j < number_of_images; j++){
            kAB = computeLeKernelRGB(B[i], A[j], 5);
            outfile <<setprecision(12) << (j+1) << ":" << kAB << " ";
        }
        outfile << "\n";
    }
     
    double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout << "it takes " << duration << endl;
    return 0;
}

