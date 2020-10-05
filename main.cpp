#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>


using namespace std;

void imWeight(string wFile, string outFile, int numRow, int numCol, string whichWeight);
void imBias(string bfile, int numCol, string whichBias);
void dotProd(int * image, int numRow, int numCol, string whichLvl);

int weight1[100][784];
int weight2[10][100];
int bias1[100];
int bias2[10];

int image2[100];
int output[10];


int main()
{
    int image[784];
    ifstream infile;
    int guess;
   // ofstream outfile;
    int largeNum;
    string line;
    int num = 0;

    infile.open("test.txt");
    //outfile.open("trainOut.out");

    //Importing Weights and transposing onto right config
    imWeight("quantOutW.out", "weight1.out", 784, 100, "w1");
    imWeight("quantOut2W.out", "weight2.out", 100, 10, "w2");

    //importing Biases
    imBias("quantOutB.out", 100, "b1");
    imBias("quantOut2B.out", 10, "b2");




    while(!infile.eof())  //Going through all the pictures
    {

        for (int i = 0; i < 784; i++)
        {
                infile >> line;
                sscanf(line.c_str(), "%d", &num);
                image[i] = num*15/255;
        }

        dotProd(image,784, 100, "lvl1");   //Layer1
        dotProd(image2, 100, 10, "lvl2");   //Layer2

        largeNum = output[0];
        guess = 1;
        for (int i = 0; i < 10; i++)        //Looking through array to find largest number
        {
            if (output[i] > largeNum)
            {
                largeNum = output[i];
                guess = i;
            }
        }

        cout << guess << "\t";

    }

    return 0;
}

//Dot product function
void dotProd(int * image, int numRow, int numCol, string whichLvl)
{
    #pragma HLS inline
    #pragma HLS INTERFACE ap_ctrl_none port=return
    #pragma HLS INTERFACE s_axilite port=image
    #pragma HLS INTERFACE s_axilite port=numRow
    #pragma HLS INTERFACE s_axilite port=numCol
    #pragma HLS INTERFACE s_axilite port=whichLvl

    int sum = 0;
    for (int i = 0; i < numCol; i++)
    {
        sum = 0;
        for (int j = 0; j < numRow; j++)
        {
        #pragma HLS loop_flatten
            if (whichLvl == "lvl1")
            {
                sum += image[j] * weight1[i][j];
            }
            else if (whichLvl == "lvl2")   //This step gives negative numbers which reLU makes = 0
            {
                //cout << sum << endl;
                //cout << image2[j] << "*" << weight2[i][j] << " = " << image2[j] * weight2[i][j] << endl;
                sum += image2[j] * weight2[i][j];
                //cout << sum << endl << endl;
            }
        }

        if (whichLvl == "lvl1")
        {
            sum += bias1[i];

            if(sum < 0) {image2[i] = 0;}
            else if (sum >= 0) {image2[i] = sum;}

        }
        else if (whichLvl == "lvl2")
        {
            sum += bias2[i];

            if(sum < 0) {output[i] = 0;}
            else if (sum >= 0) {output[i] = sum; }
        }
    }

}


void imBias(string bfile, int numCol, string whichBias)  //Code to import bias from text file
{
    #pragma HLS inline
    #pragma HLS INTERFACE ap_ctrl_none port=return
    #pragma HLS INTERFACE s_axilite port=bfile
    #pragma HLS INTERFACE s_axilite port=numCol
    #pragma HLS INTERFACE s_axilite port=whichBias

    ifstream infile;

    infile.open(bfile);

    for (int i = 0; i < numCol; i++)
    {
        #pragma HLS loop_flatten
        if (whichBias == "b1")
        {
            infile >> bias1[i];
        }
        else if (whichBias == "b2")
        {
            infile >> bias2[i];
        }

    }
    infile.close();


}


void imWeight(string wFile, string outFile, int numRow, int numCol, string whichWeight)
{
    #pragma HLS inline
    #pragma HLS INTERFACE ap_ctrl_none port=return
    #pragma HLS INTERFACE s_axilite port=wFile
    #pragma HLS INTERFACE s_axilite port=outFile
    #pragma HLS INTERFACE s_axilite port=numRow
    #pragma HLS INTERFACE s_axilite port=numCol
    #pragma HLS INTERFACE s_axilite port=whichWeight

    ifstream file;
    ofstream outfile;

    file.open(wFile);
    outfile.open(outFile);

    for (int i = 0; i < numCol; i++)
    {
        for (int j = 0; j < numRow; j++)
        {
        #pragma HLS loop_flatten
            if (whichWeight == "w1")
            {
                file >> weight1[i][j];
                outfile << weight1[i][j] << "\t";
            }
            else if (whichWeight == "w2")
            {
                file >> weight2[i][j];
                outfile << weight2[i][j] << "\t";
            }
        }
        outfile << endl;
    }

    file.close();
    outfile.close();
}

