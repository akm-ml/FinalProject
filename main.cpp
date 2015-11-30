/*
Dataset is in the file pr_data. Consists of:

-10,000,000 records
-5 dimensional data
-0 or 1 class label

PART ONE

A: Determine an optimal quantizing using L quantizing intervals
   for each dimension. Note, the total number of bins then is
   L^5.
B: Use an identity gain matrix and determine a discrete Bayes
   rule to classify the data
C: Use the first 3,333,333 records to train the quantizer,
   estimate the bin probabilities, and calculate a decision rule.
D: Use the second 3,333,333 records to estimate a confusion matrix
   so you can calculate the probability of correct identification,
   Pc(L)

PART TWO

A: Vary L. Denote Lbest the value of L with the highest probability c
   of correct identification Pc(L)
B: Pc(Lbest) is biased high because the first 1/3 of dataset was
   used to determine bin probabilities, the decision rule and the
   best number of quantizing intervals.

PART THREE

A: Apply discrete Bayes rule associated with Lbest on the last
   3,333,334 records to estimate an unbiased estimate of the
   probability of correct identification.
B: This probability of correct identification should be at least
   95%.
*/



#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>


using namespace std;

// K is number of datapoints in training set
// ie K is number of rows in array
const int K = 180;

// N -1 is dimension of data
// ie N is number of columns in array
// The last column is the class of data
const int N = 6;

// L is the number of quantizing intervals
const int L = 3;


int main()
{
    // Create a 2 dimensional vector in which to store the file
    vector<vector<double>> FileInput;


    ifstream file("C:\\Users\\Alexander\\Documents\\CSC_MachineLearning_Code\\MachineLearning_FinalProject\\testmatrix2.txt");

    string line;

    // Error if file not found in directory
    if(!file)
    {
        std::cout<<"Error opening output file"<< std::endl;

        return -1;
    }


    // Storing data file into a multidimensional vector
    while (!file.eof()) // reads until the end of th efile
    {
        for(int i = 0; i < 1; i++)
        {
            vector<double> tempVec; // temporary values to store the file
            double tempData;

            for (int j = 0; j < N; j++)
            {
                file  >> tempData;
                tempVec.push_back(tempData); // store each line into a temporary vector
            }

            FileInput.push_back(tempVec); // place into FileInput line by line using the temporary vectors
        }
    }

/*
    // Output size of FileInput. The output will be the number of ROWS.
    cout << "FileInput.size() = " << FileInput.size() << endl;
    // the file input size is K+1. Not sure how to fix this!!!!!!!!!!

    // Output the multidimensional vector
    cout << "Contents of vector FileInput: " << endl;
    for(int Row = 0; Row < FileInput.size() -1; Row++) // Stop at (FileInput.size() - 1) because the size for some reason is 1 larger than it should be. Change this if the issue is fixed.
    {
        for(int Col = 0; Col < FileInput[Row].size(); Col++)
            cout << FileInput[Row][Col] << " ";

        cout << endl;
    }
    cout << endl;
*/


    // Now we set up the
    //******QUANTIZER*********
    //************************
    //************************

    // First we set up 5 (K/3)-dimensional vectors
    // To store each column of TrainingData into

    int QuantizingData = K/3;

    vector<double> vecQuantizerComponent1(QuantizingData);
    vector<double> vecQuantizerComponent2(QuantizingData);
    vector<double> vecQuantizerComponent3(QuantizingData);
    vector<double> vecQuantizerComponent4(QuantizingData);
    vector<double> vecQuantizerComponent5(QuantizingData);


    for(int i = 0; i < QuantizingData; i++)
        vecQuantizerComponent1[i] = FileInput[i][0];

    for(int i = 0; i < QuantizingData; i++)
        vecQuantizerComponent2[i] = FileInput[i][1];

    for(int i = 0; i < QuantizingData; i++)
        vecQuantizerComponent3[i] = FileInput[i][2];

    for(int i = 0; i < QuantizingData; i++)
        vecQuantizerComponent4[i] = FileInput[i][3];

    for(int i = 0; i < QuantizingData; i++)
        vecQuantizerComponent5[i] = FileInput[i][4];


    // Next, we want to rearrange each vector
    // into ascending order

    sort(vecQuantizerComponent1.begin(), vecQuantizerComponent1.end());
    sort(vecQuantizerComponent2.begin(), vecQuantizerComponent2.end());
    sort(vecQuantizerComponent3.begin(), vecQuantizerComponent3.end());
    sort(vecQuantizerComponent4.begin(), vecQuantizerComponent4.end());
    sort(vecQuantizerComponent5.begin(), vecQuantizerComponent5.end());




    /*
    Now we want to split each vecQuantizerComponent into quantizing intervals.

    l indexes the quantizing interval, l=1,2,...,L
    n indexes the components n=1,2,....,N-1 (N=6 but last value is class value)

    the l-th quantizing interval [Begin_nl, End_nl) for the n-th component
    is defined by:

                   Begin_nl = vecQuantizerComponent[(l-1)k/L + 1]
                   End_nl = vecQuantizerComponent[lK/L + 1]
    */

    // We create two vectors, one for the beginning component of
    // each quantizing interval and one for the end component.

    // So the interval [Begin_nl, End_nl) can be accessed at
    // [QuantizingIntervalsBegin[n][l], QuantizingIntervalsEnd[n][l])

    vector<vector<double>> QuantizingIntervalsBegin(N-1, vector<double>(L));
    vector<vector<double>> QuantizingIntervalsEnd(N-1, vector<double>(L));


    // Now we place the quantizing interval values into the vectors
    for(int i = 0; i < L; i++)
    {
        int temp1 = i*QuantizingData/L;
        int temp2 = (i+1)*QuantizingData/L;

        QuantizingIntervalsBegin[0][i] = vecQuantizerComponent1[temp1];
        QuantizingIntervalsEnd[0][i] = vecQuantizerComponent1[temp2];

        QuantizingIntervalsBegin[1][i] = vecQuantizerComponent2[temp1];
        QuantizingIntervalsEnd[1][i] = vecQuantizerComponent2[temp2];

        QuantizingIntervalsBegin[2][i] = vecQuantizerComponent3[temp1];
        QuantizingIntervalsEnd[2][i] = vecQuantizerComponent3[temp2];

        QuantizingIntervalsBegin[3][i] = vecQuantizerComponent4[temp1];
        QuantizingIntervalsEnd[3][i] = vecQuantizerComponent4[temp2];

        QuantizingIntervalsBegin[4][i] = vecQuantizerComponent5[temp1];
        QuantizingIntervalsEnd[4][i] = vecQuantizerComponent5[temp2];
    }

    // Set value of last quantizing interval to last value in component vector
    QuantizingIntervalsEnd[0][L-1] = vecQuantizerComponent1[QuantizingData-1];
    QuantizingIntervalsEnd[1][L-1] = vecQuantizerComponent2[QuantizingData-1];
    QuantizingIntervalsEnd[2][L-1] = vecQuantizerComponent3[QuantizingData-1];
    QuantizingIntervalsEnd[3][L-1] = vecQuantizerComponent4[QuantizingData-1];
    QuantizingIntervalsEnd[4][L-1] = vecQuantizerComponent5[QuantizingData-1];

    //output the arrays
    cout << "QuantizingIntevalsBegin: " << endl;
    for(int row = 0; row < N-1; row++)
    {
        for(int col = 0; col < L; col++)
            cout << QuantizingIntervalsBegin[row][col] << " ";
        cout<<endl;
    }

    cout<<endl;
    cout << "QuantizingIntervalsEnd: " << endl;
    for(int row = 0; row < N-1; row++)
    {
        for(int col = 0; col < L; col++)
            cout << QuantizingIntervalsEnd[row][col] << " ";
        cout<<endl;
    }

    return 0;
}
