/*
 * Quantizer.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: Kelsey
 */


#include "Quantizer.h"
#include <random>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

Quantizer::Quantizer(int L, double pr, vector<double> priorProb, map<string,int> binProbabilitiesClass0, map<string,int> binProbabilitiesClass1) {
	quantizedLevelsPerComponent = L;
	probabilityOfCorrectIdentification = pr;
	priors = priorProb;
	decisionRuleClass0 = binProbabilitiesClass0; //the map
	decisionRuleClass1 = binProbabilitiesClass1;
}

Quantizer::~Quantizer() {
	// TODO
}

void Quantizer::reSet(int L, double pr, vector<double> priorProb, map<string,int> binProbabilitiesClass0, map<string,int> binProbabilitiesClass1) {
	quantizedLevelsPerComponent = L;
	probabilityOfCorrectIdentification=pr;
	priors = priorProb;
	decisionRuleClass0 = binProbabilitiesClass0;
	decisionRuleClass1 = binProbabilitiesClass1;
}

double Quantizer::getProb() {
	return probabilityOfCorrectIdentification;
}

void Quantizer::setFinalTestCorrectness(double ftc) {
	finalTestCorrectness = ftc;
}

vector<double> Quantizer::getPriors() {
	return priors;
}

void Quantizer::printQuantizer() {
	cout << "THIS IS THE DATA FOR THE FINAL QUANTIZER" << endl;
	cout << "L: " << quantizedLevelsPerComponent << endl;
	cout << "Probability of correct Id estimate " << probabilityOfCorrectIdentification << endl;
	cout << "Prior probabilities: " << priors.at(2) << ", " << priors.at(3) << endl;
	cout << "Left Interval Bounds (Optimized): " << endl;
	for (unsigned i = 0 ; i < leftIntervalBounds.size() ; i++) {
		vector<double> component = leftIntervalBounds.at(i);
		cout << "Component " << i << ": " << endl;
		for (unsigned j = 0 ; j < component.size(); j++) {
			cout << component.at(j) << ", " << endl;
		}
		cout << "end of component" << endl;
	}
	cout << "Final Test on testData correctness: " << finalTestCorrectness << endl;
}

bool readFile(DataSet *trainingSet, DataSet *estimateSet, DataSet *testingSet) {
	ifstream file("C:\\Users\\Kelsey\\Desktop\\pr_data.txt");

	string line;
	if (!file) {
		cout << "Couldn't read the file" << endl;
		return 0;
	}
	vector<DataPoint> trainingDataVector;
	vector<DataPoint> estimateDataVector;
	vector<DataPoint> testingDataVector;
	int Counter = 0;
    while (!file.eof()) // reads until the end of th efile
    {

            vector<double> tempVecFirst; // temporary values to store the first 33333 datapoints of file
            double tempDataFirst;

            vector<double> tempVecSecond; // temporary values to store the second 33333 datapoints of file
            double tempDataSecond;

            vector<double> tempVecThird; // temporary values to store the last 33334 datapoints of file
            double tempDataThird;

            for (int j = 0; j < 6; j++)
            {
                if(Counter < 3333333) // First 3,333,333 datapoints
                {
                    file  >> tempDataFirst;
                    tempVecFirst.push_back(tempDataFirst); // store each line into a temporary vector
                }

                else if((Counter > 3333332) && (Counter < 6666666)) // Second 3,333,333 datapoints
                {
                    file >> tempDataSecond;
                    tempVecSecond.push_back(tempDataSecond); // store each line into a temporary vector
                }

                else // Last 3,333,334 datapoints
                {
                    file >> tempDataThird;
                    tempVecThird.push_back(tempDataThird); //store each line into a temp vector
                }
            }

            if(Counter < 3333333) {
                 // place into FileInput line by line using the temporary vectors
            	DataPoint dp = DataPoint({tempVecFirst.at(0),tempVecFirst.at(1),tempVecFirst.at(2),tempVecFirst.at(3),tempVecFirst.at(4)} , tempVecFirst.at(5));
            	trainingDataVector.push_back(dp);
            }
            else if(Counter > 3333332 && Counter < 6666666) {
            	DataPoint dp = DataPoint({tempVecSecond.at(0),tempVecSecond.at(1),tempVecSecond.at(2),tempVecSecond.at(3),tempVecSecond.at(4)} , tempVecSecond.at(5));
                estimateDataVector.push_back(dp);
            }
            else {
            	DataPoint dp = DataPoint({tempVecThird.at(0),tempVecThird.at(1),tempVecThird.at(2),tempVecThird.at(3),tempVecThird.at(4)} , tempVecThird.at(5));
                testingDataVector.push_back(dp);
            }
            Counter++;

    }
    trainingSet->setSet(trainingDataVector);
    estimateSet->setSet(estimateDataVector);
    testingSet->setSet(testingDataVector);
	// this we want to read the data file and create three DataSets, trainingSet, experimentSet and finalSet
	// will return 1 if it worked, 0 if not
	return 1;
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

// takes as input a data set and calculates the prior probabilities of class0 and class1
vector<double> calculatePriors(DataSet *ds) {
	double class0Count = 0, class1Count = 0;
	int currentClass = -1;
	for (unsigned i = 0 ; i < ds->getSet().size() ; i++) {
		currentClass = ds->getSet().at(i).getTrueClass();
		if (currentClass == 0) {
			class0Count++;
		} else {
			class1Count++;
		}
	}
	vector<double> retVector = {class0Count,class1Count};
	class0Count = class0Count/ds->getSet().size();
	retVector.push_back(class0Count);
	class1Count = class1Count/ds->getSet().size();
	retVector.push_back(class1Count);
	return retVector;
}

// inputs the quantized bin names and outputs a string of the binkey
string getBinKey(vector<int> quantizedData) {
	string binKey;
	int a;
	for (unsigned i = 0 ; i < quantizedData.size() ; i++) {
		if (i > 0) {
			binKey += ':';
		}
		a = quantizedData.at(i);
		binKey += a;
	}
	return binKey;
}

//inputs a datapoint and finds out what quantized bin it belongs in
// returns a vector of the quantized bin
// vector<vector<double>> interval Left Bounds is of the form <<1st component left bounds>,<second>,<third>...>
vector<int> getBinNumber(DataPoint dp, vector<vector<double> > intervalLeftBounds) {
	vector<double> dpVect = dp.getDataVector();
	int k;
	double component;
	vector<double> componentBounds;
	vector<int> binNumber;
	for (unsigned i = 0 ; i < 5 ; i++) { // goes through the
		component = dpVect.at(i); // gets i'th component
		componentBounds = intervalLeftBounds.at(i);
		for (unsigned j = 0 ; j < componentBounds.size() ; j++) { // goes through the component bounds
			// and finds which bin we are in, which is the last entry that is smaller than component
			// returns an int 1-L which will be  index in componentBounds+1
			k = 1;
			while (component < componentBounds.at(j)) {
				k++;
			}
		}
		binNumber.push_back(k);
	}
	return binNumber;
}

//inputs the training data set and calculates the bin probabilities for that training data
// by iterating through the data points, finding the quantized bin it is in
// and ++ the counter in the mapped value of the associated key
void calculateBinProbabilities(DataSet *trainingData, vector<vector<double> > leftIntervalBounds, map<string,int> binProbClass0, map<string,int> binProbClass1) {
	vector<DataPoint> trainingSet = trainingData->getSet();
	for (unsigned int i = 0 ; i < trainingSet.size(); i++) {
		DataPoint dp = trainingSet.at(i);
		vector<int> binNumber = getBinNumber(dp, leftIntervalBounds); // find out what bin it is in;
		string binKey = getBinKey(binNumber); // gets the key for the map
		if (dp.getTrueClass() == 0) { // increments respective counters
			binProbClass0[binKey]++;
		} else {
			binProbClass1[binKey]++;
		}

	}

}

//inputs the map for the decision rule, prior probabilities, trainingSet (the second 2/3 of the data)
double bayesProbability(map<string,int> binProbClass0, map<string,int> binProbClass1, vector<double> classPriors, DataSet *estimateSet, vector<vector<double> > leftIntervalBounds) {
	int c00 = 0;
	int c01 = 0;
	int c10 = 0;
	int c11 = 0;
	int assignedClass = -1;
	int trueClass = -1;
	for (unsigned i = 0 ; i < estimateSet->getSet().size() ; i++) { //goes through vector<dataPoint>
		DataPoint dp = estimateSet->getSet().at(i);
		trueClass = dp.getTrueClass();
		vector<int> binNumber = getBinNumber(dp, leftIntervalBounds);
		string binKey = getBinKey(binNumber);
		double binGivenClass0 = (double) binProbClass0[binKey] / classPriors.at(0);
		double binGivenClass1 = (double) binProbClass1[binKey] / classPriors.at(1);
		double probabilityOfClass0 = classPriors.at(2) * binGivenClass0;
		double probabilityOfClass1 = classPriors.at(3) * binGivenClass1;
		if (probabilityOfClass0 < probabilityOfClass1) { // applies decision rule
			dp.assignClass(1);
			assignedClass = 1;
		} else {
			dp.assignClass(0);
			assignedClass = 0;
		}
		if (assignedClass == trueClass) { // increments confusion matrix entries
			if (assignedClass == 1) {
				c11++;
			} else {
				c00++;
			}
		} else if (trueClass == 0) { // here the assignedClass is 1
			c01++;
		} else {
			c10++;
		}
	}
	double probabilityOfCorrectIdentification = (double) c00 + (double) c11;
	probabilityOfCorrectIdentification = probabilityOfCorrectIdentification/estimateSet->getSet().size();
	return probabilityOfCorrectIdentification;
}

vector<vector<double> > initialIntervals(int L, DataSet *trainingSet) {
	trainingSet->sortComponentSet(); // we have a sorted list of component sets here
	int N = trainingSet->getSet().size(); // this is to determine N in the notes
	vector<double> zero = {0}; // this initializes the left boundary for the first quant interval at 0 for all 5 dims
	vector<vector<double> > intervalLeftBounds = {zero,zero,zero,zero,zero}; // initialize at 0 for all 5 dimensions?
	for (int k = 1 ; k < L ; k++) { // iterates through the bounds, partitions into L
		int index = (k*N/L)+1;
		intervalLeftBounds.at(0).push_back(trainingSet->getComponent(1).at(index));
		intervalLeftBounds.at(1).push_back(trainingSet->getComponent(2).at(index));
		intervalLeftBounds.at(2).push_back(trainingSet->getComponent(3).at(index));
		intervalLeftBounds.at(3).push_back(trainingSet->getComponent(4).at(index));
		intervalLeftBounds.at(4).push_back(trainingSet->getComponent(5).at(index));
	}
	return intervalLeftBounds;
}


double Quantizer::generateEstimate(DataSet *estimateSet) {
	int c00 = 0;
	int c01 = 0;
	int c10 = 0;
	int c11 = 0;
	int assignedClass = -1;
	int trueClass = -1;
	vector<DataPoint> dpVect = estimateSet->getSet();
	for (unsigned i = 0 ; i < dpVect.size() ; i++) { //goes through vector<dataPoint>
		DataPoint dp = dpVect.at(i);
		trueClass = dp.getTrueClass();
		vector<int> binNumber = getBinNumber(dp, leftIntervalBounds);
		string binKey = getBinKey(binNumber);
		double binGivenClass0 = (double) decisionRuleClass0[binKey] / priors.at(0);
		double binGivenClass1 = (double) decisionRuleClass1[binKey] / priors.at(1);
		double probabilityOfClass0 = priors.at(2) * binGivenClass0;
		double probabilityOfClass1 = priors.at(3) * binGivenClass1;
		if (probabilityOfClass0 < probabilityOfClass1) { // applies decision rule
			dp.assignClass(1);
			assignedClass = 1;
		} else {
			dp.assignClass(0);
			assignedClass = 0;
		}
		if (assignedClass == trueClass) { // increments confusion matrix entries
			if (assignedClass == 1) {
				c11++;
			} else {
				c00++;
			}
		} else if (trueClass == 0) { // here the assignedClass is 1
			c01++;
		} else {
			c10++;
		}
	}
	double probabilityOfCorrectIdentification = (double) c00 + (double) c11;
	probabilityOfCorrectIdentification = probabilityOfCorrectIdentification/estimateSet->getSet().size();
	return probabilityOfCorrectIdentification;
}

// this is one iteration of the T optimizations we will run to optimize the quantizer
//may have to change the initial Intervals to a pointer if the changes dont remain once function is over
double greedyAlgorithm(vector<vector<double> > initialIntervals, int L, DataSet *trainingSet, vector<double> classPriors) {
	double changeInProbOfCorrectIdentification = 1;
	double Lbest = 0;
		while (changeInProbOfCorrectIdentification < 0.001) {

		int j = rand() % 5 + 1; // random component

		int k = rand() % L + 1; // random bin

		vector<double> componentJ = trainingSet->getComponent(j); // gets the component vector

		double bnew = componentJ.at(k); // gets the bin
		double bBest = bnew;
		double preLoopL = 0;
	// determine minimum distance between bkj
	//and different bij by going through the jth component
		double distanceDown = bnew - componentJ.at(k-1);
		double distanceUp = 0;
		if (k < L) {
			distanceUp = componentJ.at(k+1) - bnew;
		}
	// components are already sorted so we are looking for what kind of M and delta we can use here

		double delta = fRand(distanceDown,distanceUp);
	// small perturbation, between distanceDown and distanceUp

		double maxDistance = min(distanceDown, distanceUp);

		int maxM = (int) floor (maxDistance/(2*delta));

		int M = rand() % maxM;
	// small integer between 0 and min(distanceDown,distanceUp)/delta;

		for (int m = 0 ; m <= 2*M ; m++) {
			bnew = bnew + delta; // perturbs the bound value appropriately
			// need to put the new value into the vector for computation
			initialIntervals.at(j).at(k)=bnew;
			map<string,int> binProbClass0;
			map<string,int> binProbClass1;
			calculateBinProbabilities(trainingSet, initialIntervals, binProbClass0, binProbClass1);//calculateBinProbabilities();
			double probabilityOfCorrectIdentification = bayesProbability(binProbClass0, binProbClass1, classPriors, trainingSet, initialIntervals);
			if (m == 0) {
				preLoopL = probabilityOfCorrectIdentification;
			}
			if (Lbest < probabilityOfCorrectIdentification) { // this new one classifies better
				Lbest = probabilityOfCorrectIdentification;
				bBest = bnew;
			} // otherwise we do not change the values and continue the loop
		}
		changeInProbOfCorrectIdentification = preLoopL - Lbest;
		// replace the kj with bBest in vector
		initialIntervals.at(j).at(k)=bBest;
	}
	return Lbest;
}

//TODO this changes qBest into the best quantizer associated to L
// returns the interval bounds associated to the best decision rule
vector<vector<double> > quantize(int L, int T, DataSet *trainingSet, Quantizer *qBest) {
	cout << "optimizing bounds" << endl;
	vector<double> priors = calculatePriors(trainingSet);
	vector<vector<double> > initialInt = initialIntervals(L,trainingSet);
	double probCorrect = 0;
	double PcL = 0;
	map<string,int> binProbClass0;
	map<string,int> binProbClass1;
	qBest->reSet(L,probCorrect,priors,binProbClass0,binProbClass1);

	for (int i = 0 ; i < T ; i++) {
		probCorrect = greedyAlgorithm(initialInt,L,trainingSet,priors);
		if (probCorrect > PcL) {
			qBest->reSet(L,probCorrect,priors,binProbClass0,binProbClass1);
		}
	}
	return initialInt;
}

//TODO
Quantizer quantizerDriver(vector<int> Lvalues, DataSet *trainingSet, DataSet *estimateSet) {
	cout << "in driver" << endl;
	map<string,int> binClass0;
	map<string,int> binClass1;
	int Lbest = 0;
	double qNewEstimateOfCorrect;
	double LbestEstimateOfCorrect = 0;
	Quantizer qNew = Quantizer(0,0,{0},binClass0,binClass1);
	Quantizer qBest = Quantizer(0,0,{0},binClass0,binClass1);
	for (unsigned i = 0 ; i < Lvalues.size() ; i++) {
		vector<vector<double> > optimalBounds = quantize(Lvalues.at(i),10,trainingSet,&qNew);
		// test my quantizer on data set 2
		qNewEstimateOfCorrect = qNew.generateEstimate(estimateSet);
		if (qNewEstimateOfCorrect > LbestEstimateOfCorrect) {
			qBest = qNew;
			Lbest = Lvalues.at(i);
		}
		cout << "For this round: L is " << Lvalues.at(i) << "and the Probability of Correct Identification is" << qNew.getProb() << endl;
	}
	cout << "The Best L is " << Lbest << "with error estimate of " << endl;
	return qBest;
}

Quantizer trainEstimateTestQuantizer(vector<int> Lvalues, DataSet *trainingSet, DataSet *estimateSet, DataSet *testingSet) {
	Quantizer bestQuantEver = quantizerDriver(Lvalues, trainingSet, estimateSet);
	cout << "Have the best quantizer " << endl;
	double finalTestCorrectness = bestQuantEver.generateEstimate(testingSet);
	bestQuantEver.setFinalTestCorrectness(finalTestCorrectness);
	return bestQuantEver;
}

int main() {
	DataSet trainingSet;
	DataSet estimateSet;
	DataSet testingSet;
	readFile(&trainingSet, &estimateSet, &testingSet);
	cout << "Data Read" << endl;
	vector<int> Lvalues = {1,2,3,4,5,6,7,8,9,10};
	Quantizer theQuantizer = trainEstimateTestQuantizer(Lvalues, &trainingSet, &estimateSet, &testingSet);
	theQuantizer.printQuantizer();
	return 0;

}
