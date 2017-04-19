#include <stdio.h>
#include <stdlib.h>
#include "computeLeKernel.h"
#include <iostream>
#include <fstream>
#include <vector>
// ================================
using namespace std;
int ReverseInt (int i);
void ReadMNIST(int NumberOfImages, int DataOfAnImage,vector<vector<double> > &arr);
void ReadMNISTtrain(int NumberOfImages, int DataOfAnImage,vector<vector<double> > &arr);
void read_Mnist_Label(vector<double> &vec);
void read_Mnist_Label_Train(vector<double> &vec);
int main()
{
  const int n=32;
  // double *A[n], *B[n];
  // double kAB;

  // for (int i=0; i<n; i++)
  // {
  //   A[i] = (double*)(calloc(n,sizeof(double)));
  //   B[i] = (double*)(calloc(n,sizeof(double)));
  //   for (int j=0; j<n; j++)
  //   {
  //     A[i][j] = (0.5*(i+j))/n;
  //     B[i][j] = (1.0*i*j)/(n*n);
  //   }
  // }
  vector<double> label;
  read_Mnist_Label(label);
  
  //for(int i = 0; i < label.size(); i++){
  //  printf("%d\n", int(label[i]));
  //}
  
  // kAB = computeLeKernel(A,B);
  // printf("kAB = %f\n", kAB);
  vector<vector<double> > test;
  ReadMNIST(10000,784,test);
  vector<vector<double> > train;
  ReadMNISTtrain(60000, 784, train);
  // the code you wish to time goes here
  vector<double> label_train;
  read_Mnist_Label_Train(label_train);
    // for(int i = 0; i < label.size(); i++){
    //   cout << label[i] << endl;
    // }
  vector<int> actual_pos;
  actual_pos.resize(60000);

  int actual = 0;
  // This steps give u the actual position of the label
  // This is because previously we have all the kernel values computed based
  // on the actual label (when we split them into classes and compute)
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < label_train.size(); j++){
      if(label_train[j] == i){
        actual_pos[j] = actual;
        actual++;
      }
    }
  }
  vector<vector<double> > new_train;
  new_train.resize(60000,vector<double>(784));
  for(int i = 0; i < 60000; i++){
    new_train[actual_pos[i]] = train[i];
  }
  // for(int i = 0; i < actual_pos.size(); i++){
  //   printf("actual: %d, label: %f\n", actual_pos[i], label[i]);
  // }
  // //order the training dataset to be all 0s + all 1s + all2s
  // return 0;


  //vector<double> label;
  //read_Mnist_Label(label);
  vector< vector<double> >::const_iterator row; 
  vector<double>::const_iterator col; 
  int index = 0;
  //Every row contain 784 values
  //double layer calculation
  ofstream outfile;
  double *A[n], *B[n];
  double kAB;
  for (int i=0; i<n; i++)
  {
    A[i] = (double*)(calloc(n,sizeof(double)));
    B[i] = (double*)(calloc(n,sizeof(double)));
  }
  outfile.open("/oasis/tscc/scratch/rqiu/mnist_libsvm_kernel.txt");
  //outer layer is test data
  for(int first = 0; first < 10000; first++){
    int start_s=clock();
    outfile << label[first];
    outfile << " 0:";
    outfile << first;
    outfile << " ";
    for(int second = 0; second < 60000; second++){
        for(int i = 2; i < 30; i++){
          for(int j = 2; j < 30; j++){
            A[i][j] = 0;
            B[i][j] = 0;
          }
        }
        for(int i = 2; i < 30; i++){
          for (int j= 2; j < 30; j++){
            A[i][j] = test[first][(j-2)+(i-2)*28]/255.0;
            B[i][j] = new_train[second][(j-2)+(i-2)*28]/255.0;
          } 
        }
        kAB = computeLeKernel(A,B);
        outfile << second+1;
        outfile << ":";
        outfile << kAB;
        outfile << " ";
    } 
    outfile << "\n";
    int stop_s=clock();
    cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;
    if(first == 1)
      break;
  }
//--------------
  //Single layer calculation for test data diagonal
  /*
  int start_s = clock();
  for(int first = 0; first < 10000; first++){
    double *A[n], *B[n];
    double kAB;
    for (int i=0; i<n; i++)
    {
      A[i] = (double*)(calloc(n,sizeof(double)));
      B[i] = (double*)(calloc(n,sizeof(double)));
    }
    for(int i = 1; i < 29; i++){
      for (int j= 1; j< 29; j++)
      {
        A[i][j] = test[first][(j-1)+(i-1)*28]/255.0;
        B[i][j] = test[first][(j-1)+(i-1)*28]/255.0;
        //printf("index = %d\n", (j-1)+(i-1)*28);

      }
    }
    */
    /*
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(A[i][j] == 0.0){
                printf("0 ");
            }
            else{
                printf("1 ");
            }
            //printf("%.2f ", A[i][j]);
        }
        printf("\n");
    }
    

    kAB = computeLeKernel(A,B);
    printf("%G\n", kAB);
  }
  int stop_s=clock();
  cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
  */
//-------------
}

 
int ReverseInt (int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1=i&255;
    ch2=(i>>8)&255;
    ch3=(i>>16)&255;
    ch4=(i>>24)&255;
    return((int)ch1<<24)+((int)ch2<<16)+((int)ch3<<8)+ch4;
}

void ReadMNISTtrain(int NumberOfImages, int DataOfAnImage,vector<vector<double> > &arr)
{
    arr.resize(NumberOfImages,vector<double>(DataOfAnImage));
    //ifstream file ("/home/rqiu/t10k-images.idx3-ubyte",ios::binary);
    //ifstream file ("/home/rqiu/t10k-images-idx3-ubyte");
    ifstream file ("/home/rqiu/train-images-idx3-ubyte");
    if (file.is_open())
    {
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        file.read((char*)&magic_number,sizeof(magic_number));
        magic_number= ReverseInt(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= ReverseInt(number_of_images);
        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= ReverseInt(n_rows);
        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= ReverseInt(n_cols);
        for(int i=0;i<number_of_images;++i)
        {
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    arr[i][(n_rows*r)+c]= (double)temp;
                }
            }
        }
    }
    else{
      cout << "file is not open" << endl;
    }
}

void ReadMNIST(int NumberOfImages, int DataOfAnImage,vector<vector<double> > &arr)
{
    arr.resize(NumberOfImages,vector<double>(DataOfAnImage));
    //ifstream file ("/home/rqiu/t10k-images.idx3-ubyte",ios::binary);
    ifstream file ("/home/rqiu/t10k-images-idx3-ubyte");
    //ifstream file ("/home/rqiu/train-images-idx3-ubyte");
    if (file.is_open())
    {
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        file.read((char*)&magic_number,sizeof(magic_number));
        magic_number= ReverseInt(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= ReverseInt(number_of_images);
        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= ReverseInt(n_rows);
        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= ReverseInt(n_cols);
        for(int i=0;i<number_of_images;++i)
        {
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    arr[i][(n_rows*r)+c]= (double)temp;
                }
            }
        }
    }
    else{
      cout << "file is not open" << endl;
    }
}
void read_Mnist_Label_Train(vector<double> &vec)
{
    vec.resize(60000);
    //ifstream file ("/home/rqiu/t10k-labels-idx1-ubyte");
    ifstream file ("/home/rqiu/train-labels-idx1-ubyte");

    if (file.is_open())

    {

        int magic_number = 0;

        int number_of_images = 0;

        int n_rows = 0;

        int n_cols = 0;

        file.read((char*) &magic_number, sizeof(magic_number));
        magic_number = ReverseInt(magic_number);

        file.read((char*) &number_of_images,sizeof(number_of_images));

        number_of_images = ReverseInt(number_of_images);

        for(int i = 0; i < number_of_images; ++i)

        {

            unsigned char temp = 0;

            file.read((char*) &temp, sizeof(temp));

            vec[i]= (double)temp;

        }

    }
    else{
      cout << "label file can't be opened" << endl;
    }

}

void read_Mnist_Label(vector<double> &vec)
{
    vec.resize(10000);
    //ifstream file ("/home/rqiu/train-labels-idx1-ubyte");
    ifstream file ("/home/rqiu/t10k-labels-idx1-ubyte");


    if (file.is_open())

    {

        int magic_number = 0;

        int number_of_images = 0;

        int n_rows = 0;

        int n_cols = 0;

        file.read((char*) &magic_number, sizeof(magic_number));
        magic_number = ReverseInt(magic_number);

        file.read((char*) &number_of_images,sizeof(number_of_images));

        number_of_images = ReverseInt(number_of_images);

        for(int i = 0; i < number_of_images; ++i)

        {

            unsigned char temp = 0;

            file.read((char*) &temp, sizeof(temp));

            vec[i]= (double)temp;

        }

    }
    else{
      cout << "label file can't be opened" << endl;
    }

}




 
// ================================
