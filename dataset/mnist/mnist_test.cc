#include <stdio.h>
#include <stdlib.h>
#include "computeLeKernel.h"
#include <iostream>
#include <fstream>
#include <vector>
//#include <string.h>
#include <string>
#include <cstdlib>
#include <iomanip>
using namespace std;


void split(const string& s, char c, vector<string>& v){
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

int main()
{
    int number_of_images = 1000;
    const int n = 32;

    double ***A, ***B;
    double kAB;
    A = new double**[number_of_images];
    for(int k = 0; k < number_of_images; k++){
        A[k] = new double*[n];
        for(int i = 0; i < n; i++){
            A[k][i] = new double[n];
            for(int j = 0; j < n; j++){
                A[k][i][j] = 0.0;
            }
        }
    }
    B = new double**[number_of_images];
    for(int k = 0; k < number_of_images; k++){
        B[k] = new double*[n];
        for(int i = 0; i < n; i++){
            B[k][i] = new double[n];
            for(int j = 0; j < n; j++){
                B[k][i][j] = 0.0;
            }
        }
    }
    ifstream file("mnist_train_no_colon.txt");
    ofstream outfile;
    outfile.open("mnist_test_libsvm.txt");
    string str;
    int line_number = 0;
    clock_t start = clock();
    while(getline(file, str)){
        if(line_number >= number_of_images){
            break;
        }
        vector<string> v;
        split(str, ' ', v);
        int index = 0;
        for(int i = 0; i < 32; i++){
            for(int j = 0; j < 32; j++){
                A[line_number][i][j] = atof(v[index].c_str());
                index++;
            }
        }
        line_number++;
    }

    ifstream test_file("mnist_test_no_colon.txt");
    line_number = 0;
    while(getline(test_file, str)){
        if(line_number >= number_of_images){
            break;
        }
        vector<string> v;
        split(str, ' ', v);
        int index = 0;
        for(int i = 0; i < 32; i++){
            for(int j = 0; j < 32; j++){
                B[line_number][i][j] = atof(v[index].c_str());
                index++;
            }
        }
        line_number++;
    }

    int* label = new int[number_of_images];
    ifstream label_file("mnist_test_label.txt");
    line_number = 0;
    while(getline(label_file, str)){
        if(line_number >= number_of_images){
            break;
        }
        label[line_number] = atof(str.c_str());
        line_number++;
    }
/*
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 32; j++){
            cout << A[0][i][j] << " ";
        }
        cout << endl;
    }
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 32; j++){
            cout << B[0][i][j] << " ";
        }
        cout << endl;
    }
    */
    
    for(int i = 0; i < number_of_images; i++){
        outfile << label[i] << " " << "0:" << (i+1) << " ";
        for(int j = 0; j < number_of_images; j++){
            kAB = computeLeKernel(B[i], A[j]);
            outfile << setprecision(12) << (j+1) << ":" << kAB << " ";
            
        }
        outfile << "\n";
    }
    double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout << "it takes " << duration << endl;
}




