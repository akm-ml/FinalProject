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
#include <time.h>

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

void Quantizer::setIntervalBounds(vector<vector<double> > intervalB) {
	leftIntervalBounds = intervalB;
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
    while (!file.eof()) // reads until the end of the file
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
            	//cout << "created data point that has size " << dp.getDataVector().size() << endl;
            	//cout << "training Data now has " << trainingDataVector.size() << endl;
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
    cout << "training set size " << trainingSet->getSet().size() << endl;;
    estimateSet->setSet(estimateDataVector);
    cout << "estimate set size " << estimateSet->getSet().size() << endl;
    testingSet->setSet(testingDataVector);
    cout << "testing set size " << testingSet->getSet().size() << endl;
    trainingSet->createComponentSet();
    trainingSet->sortComponentSet();
    //estimateSet->createComponentSet();
    //estimateSet->sortComponentSet();
    //testingSet->createComponentSet();
    //testingSet->sortComponentSet();
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
	cout << "in calculate priors" << endl;
	vector<DataPoint> dpVect = ds->getSet();
	double class0Count = 0, class1Count = 0;
	int currentClass = -1;
	for (unsigned i = 0 ; i < dpVect.size() ; i++) {
		//cout << "in loop for " << i << endl;
		currentClass = dpVect.at(i).getTrueClass();
		if (currentClass == 0) {
			class0Count++;
		} else {
			class1Count++;
		}
	}
	vector<double> retVector = {class0Count,class1Count};
	class0Count = class0Count/dpVect.size();
	retVector.push_back(class0Count);
	class1Count = class1Count/dpVect.size();
	retVector.push_back(class1Count);
	cout << "Priors" << retVector.at(0) << ", " << retVector.at(1) << ", " << retVector.at(2) << ", " << retVector.at(3) << endl;
	//cout << "done calculating priors" << endl;
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
		ostringstream ss;
		ss << a;
		binKey += ss.str();
	}
	return binKey;
}

//inputs a datapoint and finds out what quantized bin it belongs in
// returns a vector of the quantized bin
// vector<vector<double>> interval Left Bounds is of the form <<1st component left bounds>,<second>,<third>...>
vector<int> getBinNumber(DataPoint dp, vector<vector<double> > intervalLeftBounds) {
	vector<double> dpVect = dp.getDataVector();
	double component;
	int binCounter = 0;
	vector<double> componentBounds;
	vector<int> binNumber;
	for (unsigned i = 0 ; i < 5 ; i++) { // goes through the
		component = dpVect.at(i); // gets i'th component
		componentBounds = intervalLeftBounds.at(i); // gets component bounds for i component
		for (unsigned j = 0 ; j < componentBounds.size() ; j++) { // goes through the component bounds
			//finds which bin we are in, which is the last entry that is smaller than component
			// returns an int 0,L-1 which will be  index in componentBounds
			if (component > componentBounds.at(j)) {
				binCounter = j;
			}
		}
		binNumber.push_back(binCounter);
	}
	return binNumber;
}

//inputs the training data set and calculates the bin probabilities for that training data
// by iterating through the data points, finding the quantized bin it is in
// and ++ the counter in the mapped value of the associated key
void calculateBinProbabilities(DataSet *trainingData, vector<vector<double> > *leftIntervalBounds, map<string,int> *binProbClass0, map<string,int> *binProbClass1) {
	//cout << "calculating bin probabilities" << endl;
	vector<DataPoint> trainingSet = trainingData->getSet();
	binProbClass0->clear();
	binProbClass1->clear();
	for (unsigned int i = 0 ; i < trainingSet.size(); i++) {
		DataPoint dp = trainingSet.at(i);
		vector<int> binNumber = getBinNumber(dp, *leftIntervalBounds); // find out what bin it is in;
		string binKey = getBinKey(binNumber); // gets the key for the map
		if (dp.getTrueClass() == 0) { // increments respective counters
			(*binProbClass0)[binKey]++;
		} else {
			(*binProbClass1)[binKey]++;
		}
	}
	//cout << "calculated bin probabilities" << endl;
}

//inputs the map for the decision rule, prior probabilities, trainingSet (the second 2/3 of the data)
double bayesProbability(map<string,int> *binProbClass0, map<string,int> *binProbClass1, vector<double> classPriors, DataSet *estimateSet, vector<vector<double> > *leftIntervalBounds) {
	//cout << "in bayes probability" << endl;
	int c00 = 0;
	int c01 = 0;
	int c10 = 0;
	int c11 = 0;
	int assignedClass = -1;
	int trueClass = -1;
	vector<DataPoint> estimateDPvect = estimateSet->getSet();
	for (unsigned i = 0 ; i < estimateDPvect.size() ; i++) { //goes through vector<dataPoint>
		DataPoint dp = estimateDPvect.at(i);
		trueClass = dp.getTrueClass();
		vector<int> binNumber = getBinNumber(dp, *leftIntervalBounds);
		string binKey = getBinKey(binNumber);
		double binGivenClass0 = (double) (*binProbClass0)[binKey] / classPriors.at(0);
		double binGivenClass1 = (double) (*binProbClass1)[binKey] / classPriors.at(1);
		//cout << "bin given c0: " << binGivenClass0 << " and bin given c1: " << binGivenClass1 << endl;
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
	//cout << "counts for prci" << probabilityOfCorrectIdentification << endl;
	probabilityOfCorrectIdentification = probabilityOfCorrectIdentification/estimateSet->getSet().size();
	//cout << "probability of correct identification is " << probabilityOfCorrectIdentification << endl;
	return probabilityOfCorrectIdentification;
}

vector<vector<double> > initialIntervals(int L, DataSet *trainingSet) {
	//cout << "in initial intervals" << endl;
	//trainingSet->sortComponentSet(); // we have a sorted list of component sets here
	int N = trainingSet->getSet().size(); // this is to determine N in the notes
	vector<double> zero = {0}; // this initializes the left boundary for the first quant interval at 0 for all 5 dims
	vector<vector<double> > intervalLeftBounds = {zero,zero,zero,zero,zero};
	vector<double> * componentOne = trainingSet->getComponent(1);// initialize at 0 for all 5 dimensions?
	//cout << "have component one: " << componentOne->size() << endl;
	vector<double> * componentTwo = trainingSet->getComponent(2);
	//cout << "have component two" << endl;
	vector<double> * componentThree = trainingSet->getComponent(3);
	//cout << "have component three" << endl;
	vector<double> * componentFour = trainingSet->getComponent(4);
	//cout << "have component four" << endl;
	vector<double> * componentFive = trainingSet->getComponent(5);
	//cout << "have component five" << endl ;
	int counter = N/L;
	cout << counter << endl;
	for (int k = 1 ; k < L ; k++) { // iterates through the bounds, partitions into L
		cout << "doing interval " << k << endl;
		int index = k*counter;
		//cout << index << endl;
		intervalLeftBounds.at(0).push_back(componentOne->at(index));
		intervalLeftBounds.at(1).push_back(componentTwo->at(index));
		intervalLeftBounds.at(2).push_back(componentThree->at(index));
		intervalLeftBounds.at(3).push_back(componentFour->at(index));
		intervalLeftBounds.at(4).push_back(componentFive->at(index));
	}
	return intervalLeftBounds;
}


double Quantizer::generateEstimate(DataSet *estimateSet) {
	cout << "in generate estimate" << endl;
	int c00 = 0;
	int c11 = 0;
	int assignedClass = -1;
	int trueClass = -1;
	vector<DataPoint> dpVect = estimateSet->getSet();
	cout << "size of dpVect " << dpVect.size() << endl;
	for (unsigned i = 0 ; i < dpVect.size() ; i++) { //goes through vector<dataPoint>

		DataPoint dp = dpVect.at(i);
		trueClass = dp.getTrueClass();
		//cout << trueClass << endl;
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
		}
	}
	double probabilityOfCorrectIdentification = (double) c00 + (double) c11;
	probabilityOfCorrectIdentification = probabilityOfCorrectIdentification/dpVect.size();
	cout << "done generating estimates" << endl;
	return probabilityOfCorrectIdentification;
}

// this is one iteration of the T optimizations we will run to optimize the quantizer
//may have to change the initial Intervals to a pointer if the changes dont remain once function is over
double greedyAlgorithm(vector<vector<double> > *initialIntervals, int L, DataSet *trainingSet, vector<double> classPriors, double lastRunPrCorrect) {
	double changeInProbOfCorrectIdentification = 1;
	double Lbest = lastRunPrCorrect;
	double probabilityOfCorrectIdentification;
		while (changeInProbOfCorrectIdentification > 0.001) {

		int j = rand() % 5;// + 1; // random component

		int k = rand() % L;// + 1; // random bin

		//vector<double> componentJ = trainingSet->getComponent(j); // gets the component vector

		double bnew = initialIntervals->at(j).at(k); // gets the bin
		double bBest = bnew;
	//	double preLoopL = lastRunPrCorrect;
		//cout << "selected bin (" << j ", " << k << ")" << endl;
		cout << "(j,k)" << j <<", " << k << " which has boundary " << bnew << " which is " << trainingSet->getComponent(j)->at(k) << endl;
		double dDown;
		if (k > 0) {
			dDown = bnew - initialIntervals->at(j).at(k-1);
		} else { dDown = bnew - 0; }
		double dUp;
		if (k < L-1) {
			dUp = initialIntervals->at(j).at(k+1) - bnew;
		} else { dUp = 1 - bnew; }
		int M = rand() % 20;
		cout << "Ddown Dup are " << dDown << ", " << dUp << endl;
		double maxDistance = min(dDown,dUp);
		cout << "maxDistance = " << maxDistance << endl;
		double delta = fRand(-maxDistance/(2*M), maxDistance/(2*M));
		cout << "have to run greedy algo loop " << 2*M << " times, with delta equal to " << delta << endl;
		for (int m = 0 ; m <= 2*M ; m++) {
			cout << " in greedy algorithm loop for " << m << endl;
			bnew = bnew + delta; // perturbs the bound value appropriately
			// need to put the new value into the vector for computation
			initialIntervals->at(j).at(k)=bnew;
			map<string,int> binProbClass0;
			map<string,int> binProbClass1;
			calculateBinProbabilities(trainingSet, initialIntervals, &binProbClass0, &binProbClass1);//calculateBinProbabilities();
			probabilityOfCorrectIdentification = bayesProbability(&binProbClass0, &binProbClass1, classPriors, trainingSet, initialIntervals);
//			if (m == 0) {
//				preLoopL = probabilityOfCorrectIdentification;
//			}
			if (Lbest < probabilityOfCorrectIdentification) { // this new one classifies better
				Lbest = probabilityOfCorrectIdentification;
				cout << "Lbest, i.e. old prob is " << Lbest << ", and new prob is " << probabilityOfCorrectIdentification << endl;
				bBest = bnew;
			} // otherwise we do not change the values and continue the loop
			cout << "probability of correct id is "<< Lbest << endl;
		}
		changeInProbOfCorrectIdentification = abs(probabilityOfCorrectIdentification - Lbest);
		// replace the kj with bBest in vector
		initialIntervals->at(j).at(k)=bBest;
	}
	return Lbest;
}

//TODO this changes qBest into the best quantizer associated to L
// returns the interval bounds associated to the best decision rule
vector<vector<double> > quantize(int L, int T, DataSet *trainingSet, Quantizer *qBest) {
	cout << "in quantize" << endl;
	vector<double> priors = calculatePriors(trainingSet);
	cout << "have calculated priors" << endl;
	vector<vector<double> > initialInt = initialIntervals(L,trainingSet);
	cout << "have initial intervals" << endl;
	double probCorrect = 0;
	double PcL = 0;
	map<string,int> binProbClass0;
	map<string,int> binProbClass1;
	qBest->reSet(L,probCorrect,priors,binProbClass0,binProbClass1);
	cout << "going to run greedy for " << T << " times" << endl;
	for (int i = 0 ; i < T ; i++) {
		cout << "starting greedy for " << i << "run" << endl;
		if (i == 0) {
			PcL = greedyAlgorithm(&initialInt, L, trainingSet, priors, 0);
		}
		probCorrect = greedyAlgorithm(&initialInt,L,trainingSet,priors, PcL);
		cout << "done for " << i << endl;
		cout << probCorrect << "is probCorrect for the " << i << " iteration of greedy Alg" << endl;
		if (probCorrect > PcL) {
			calculateBinProbabilities(trainingSet, &initialInt, &binProbClass0, &binProbClass1);
			qBest->reSet(L,probCorrect,priors,binProbClass0,binProbClass1);
			qBest->setIntervalBounds(initialInt);
		}//Todo
	}
	return initialInt;
}

//TODO
Quantizer quantizerDriver(vector<int> Lvalues, DataSet *trainingSet, DataSet *estimateSet) {
	cout << "in the driver" << endl;
	map<string,int> binClass0;	map<string,int> binClass1; map<string,int> binClass02; map<string,int> binClass12;
	int Lbest = 0;
	double qNewEstimateOfCorrect;
	double LbestEstimateOfCorrect = 0;
	Quantizer qNew = Quantizer(0,0,{0},binClass0,binClass1);
	Quantizer qBest = Quantizer(0,0,{0},binClass02,binClass12);
	for (unsigned i = 0 ; i < Lvalues.size() ; i++) {
		vector<vector<double> > optimalBounds = quantize(Lvalues.at(i),10,trainingSet,&qNew);
		cout << "have the optimal bounds for L" << endl;
		qNew.setIntervalBounds(optimalBounds);
		qNewEstimateOfCorrect = qNew.generateEstimate(estimateSet);
		cout << "have generated the estimate of correct identification for the optimal bounds" << endl;
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
	srand(time(NULL));
//	for (int i = 0 ; i < 15 ; i++) {
//	int M = rand() % 20;
//	double maxDistance = 1;
//	//double minDistance = 1;
//	double delta = fRand(-maxDistance/(2*M), maxDistance/(2*M));
//	cout << delta << endl;
//	}
	DataSet trainingSet;
	DataSet estimateSet;
	DataSet testingSet;
	readFile(&trainingSet, &estimateSet, &testingSet);
	cout << "Data Read" << endl;
	vector<int> Lvalues = {5};

	Quantizer theQuantizer = trainEstimateTestQuantizer(Lvalues, &trainingSet, &estimateSet, &testingSet);
	theQuantizer.printQuantizer();
	return 0;


}
