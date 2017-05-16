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
    printf("number of arguments: %d\n", argc);
    for(int i = 0; i < argc; i++){
        printf("the command line arguments are: %s\n", argv[i]);
    }

    const int n=32;
    int start_line = atoi(argv[4]);
    int end_line = atoi(argv[5]);
    if(end_line == 0){
        printf("specified number of line is 0, Exit\n");
        return 0;
    }
    ofstream outfile;
    outfile.open(argv[1]);
    ifstream file(argv[2]);

    double ***A, ***B;
    double kAB;
    A = new double**[32];
    for(int i = 0; i < 32; i++){
        A[i] = new double*[32];
        for(int j = 0; j < 32; j++){
            A[i][j] = new double[3];
        }
    }
    B = new double**[32];
    for(int i = 0; i < 32; i++){
        B[i] = new double*[32];
        for(int j = 0; j < 32; j++){
            B[i][j] = new double[3];
        }
    }

    clock_t start;
    double duration;
    start = clock();
    string str;
    int line_number = -1;
    while(getline(file, str)){
        line_number = line_number + 1;
        if(line_number >= end_line){
            break;
        }
        if(line_number < start_line){
            continue;
        }
        vector<string> v;
        split(str, ' ', v);
        int index = 0; 
        for(int k = 0; k < 3; k++){
            for(int i = 0; i < 32; i++){
                for(int j = 0; j < 32; j++){
                    A[i][j][k] = atof(v[index].c_str());
                    index++;
                }
            }
        }
        kAB = computeLeKernelRGB(A, A);
        printf("kAB = %f\n", kAB);
        ifstream file2(argv[3]);
        string str2;
        while(getline(file2, str2)){
            vector<string> v2;
            split(str2, ' ', v2);
            int index2 = 0;
            for(int k = 0; k < 3; k++){
                for(int i = 0; i < 32; i++){
                    for(int j = 0; j < 32; j++){
                        B[i][j][k] = atof(v2[index2].c_str());
                        index2++;
                    }
                }
            }
            clock_t begin = clock();
            for(int a = 0; a < 1000; a++){
                kAB = computeLeKernelRGB(A,B);
            }
            cout << (clock() - begin) / (double)CLOCKS_PER_SEC << endl;
            outfile << setprecision(12) << kAB;
            outfile << " ";
        }
        outfile << "\n";
        cout << "one iteration took " << (clock() - start) / (double)CLOCKS_PER_SEC;
    }
    duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout << "it takes " << duration << endl;
    return 0;
}

