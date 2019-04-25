#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>

/*
I'm using globals in this example because i was having issues with setting 2D arrays as parameters for the functions.
Should work anyways but just a heads up.


*/

using namespace std;

void imWeight(string wFile, string outFile, int numRow, int numCol, string whichWeight);
void imBias(string bfile, int numCol, string whichBias);
int reLU(int input);
void dotProd(int * image, int numRow, int numCol, string whichLvl);

int weight1[784][100];
int weight2[100][10];
int bias1[100];
int bias2[10];

int image2[100];
int output[10];


int main()
{
    int image[784];
    ifstream infile;
   // ofstream outfile;
    int largeNum = 0;
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

        for (int i = 0; i < 10; i++)        //Looking through array to find largest number
        {
            if (output[i] > largeNum)
            {
                largeNum = i+1;
            }
        }

        cout << "The value is: " << largeNum << endl;


    }

    return 0;
}

//Dot product function....there might be something wrong here
void dotProd(int * image, int numRow, int numCol, string whichLvl)
{
    int tot = 0;
    int pos = 0;
    int sum = 0;
    for (int i = 0; i < numCol; i++)
    {
        sum = 0;
        for (int j = 0; j < numRow; j++)
        {
            if (whichLvl == "lvl1")
            {

            tot++;
            if (weight2[i][j] > 0)
            {
                pos++;
                //cout << image[j] << ", ";
                //cout << weight2[i][j] << "; ";
                //cout << sum << ", ";
            }

                sum += image[j] * weight1[i][j];
                //cout << sum << ", ";
            }
            else if (whichLvl == "lvl2")   //This step gives negative numbers which reLU makes = 0
            {
            tot++;
            if (weight2[i][j] > 0)
            {
                pos++;
                //cout << image[j] << ", ";
                //cout << weight2[i][j] << "; ";
                //cout << sum << ", ";
            }
                sum += image[j] * weight2[i][j];
            }
        }
        //cout << pos << "/" << tot << endl;
        //cout << endl;

        if (whichLvl == "lvl1")
        {
            image2[i] = reLU((sum+bias1[i]));
        }
        else if (whichLvl == "lvl2")
        {
            //cout << sum+bias2[i] << ", ";
            output[i] = reLU((sum+bias2[i]));
        }

    }

}

int reLU(int input)
{
    if (input < 0)
    {
        return 0;
    }
    else
    {
        return input;
    }
}


void imBias(string bfile, int numCol, string whichBias)  //Code to import bias from text file
{
    ifstream infile;

    infile.open(bfile);

    for (int i = 0; i < numCol; i++)
    {
        if (whichBias == "b1")
        {
            infile >> bias1[i];
            //cout << bias1[i] << "\t";
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
    ifstream file;
    ofstream outfile;

    file.open(wFile);
    outfile.open(outFile);

    for (int i = 0; i < numCol; i++)
    {
        for (int j = 0; j < numRow; j++)
        {
            if (whichWeight == "w1")
            {
                file >> weight1[i][j];
                outfile << weight1[i][j] << "\t";
                //cout << weight1[j][i] << "\t";
            }
            else if (whichWeight == "w2")
            {
                file >> weight2[i][j];
                outfile << weight2[i][j] << "\t";
                //cout << weight2[j][i] << "\t";
            }
        }
        outfile << endl;
        //cout << endl;
    }

    file.close();
    outfile.close();


}

