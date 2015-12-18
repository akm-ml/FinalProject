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

Quantizer::Quantizer(int L, double pr, double prc0, double prc1, map<string,int> binProbabilities) {
	quantizedLevelsPerComponent = L;
	probabilityOfCorrectIdentification = pr;
	priorClass0 = prc0;
	priorClass1 = prc1;
	decisionRule = binProbabilities; //the map
}

Quantizer::~Quantizer() {
	// TODO
}

bool readFile() {
	// this we want to read the data file and create three DataSets, trainingSet, experimentSet and finalSet
	// will return 1 if it worked, 0 if not
	return 0;
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
	class0Count = class0Count/ds->getSet().size();
	class1Count = class1Count/ds->getSet().size();
	return {class0Count,class1Count};
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
map<string,int> calculateBinProbabilities(DataSet *trainingData, vector<vector<double> > leftIntervalBounds) {
	vector<DataPoint> trainingSet = trainingData->getSet();
	map<string,int> binProb;
	for (unsigned int i = 0 ; i < trainingSet.size(); i++) {
		DataPoint dp = trainingSet.at(i);
		vector<int> binNumber = getBinNumber(dp, leftIntervalBounds); // find out what bin it is in;
		string binKey = getBinKey(binNumber); // gets the key for the map
		binProb[binKey]++; // increments the counter in the map
	}
	return binProb; // returns the map
}

//inputs the map for the decision rule, prior probabilities, trainingSet (the second 2/3 of the data)
double bayesProbability(map<string,int> binProbabilities, vector<double> classPriors, DataSet *estimateSet, vector<vector<double> > leftIntervalBounds) {
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
		double binProb = (double) binProbabilities[binKey] / estimateSet->getSet().size(); // TODO need to divide by size of set?!?!
		double probabilityOfClass0 = classPriors.at(0) * binProb;
		double probabilityOfClass1 = classPriors.at(1) * binProb;
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
	probabilityOfCorrectIdentification = probabilityOfCorrectIdentification/estimateSet.getSet().size();
	return probabilityOfCorrectIdentification;
}

vector<vector<double> > initialIntervals(int L, DataSet trainingSet) {
	trainingSet.sortComponentSet(); // we have a sorted list of component sets here
	int N = trainingSet.getSet().size(); // this is to determine N in the notes
	vector<double> zero = {0}; // this initializes the left boundary for the first quant interval at 0 for all 5 dims
	vector<vector<double> > intervalLeftBounds = {zero,zero,zero,zero,zero}; // initialize at 0 for all 5 dimensions?
	for (int k = 1 ; k < L ; k++) { // iterates through the bounds, partitions into L
		int index = (k*N/L)+1;
		intervalLeftBounds.at(0).push_back(trainingSet.getComponent(1).at(index));
		intervalLeftBounds.at(1).push_back(trainingSet.getComponent(2).at(index));
		intervalLeftBounds.at(2).push_back(trainingSet.getComponent(3).at(index));
		intervalLeftBounds.at(3).push_back(trainingSet.getComponent(4).at(index));
		intervalLeftBounds.at(4).push_back(trainingSet.getComponent(5).at(index));
	}
	return intervalLeftBounds;
}

// TODO
// this is one iteration of the M optimizations we will run to optimize the quantizer
// this should return a quantizer???
double greedyAlgorithm(vector<vector<double> > initialIntervals, int L, DataSet trainingSet, vector<double> classPriors) {
	int j = rand() % 5 + 1; // random component

	int k = rand() % L + 1; // random bin

	vector<double> componentJ = trainingSet.getComponent(j); // gets the component vector

	double bnew = componentJ.at(k); // gets the bin

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
		// need to put it into the vector
		map<string,int> binProbabilities = calculateBinProbabilities(&trainingSet, initialIntervals);//calculateBinProbabilities();
		double probabilityOfCorrectIdentification = bayesProbability(binProbabilities, classPriors, &estimateSet, initialIntervals);
		// save expected gain, if higher
	}
	return 0;
}

//TODO, this is a rough outline of how we want quantizer to work
// I am having pointer problems....
void quantize(int L, int M) {
	// READ FILE
	DataSet trainingSet();
	DataSet estimateSet();
	DataSet testingSet();
	// THEN WE SORT THE COMPONENT SETS FOR ALL OF THESE?
	double prCorrectForL = 0;
	double prLbest = 0;

	//CALCULATE CLASS PRIORS
	vector<double> classPriors = calculatePriors(trainingSet);

	//gets initial intervals
	vector<vector<double> > initialLeftBounds = initialIntervals(L,trainingSet);

	// runs greedy algorithm M times
	for (int i = 0 ; i < M ; i++) {
		prCorrectForL = greedyAlgorithm(initialLeftBounds,L,estimateSet);
		if (prLbest == 0) {
			Lbest = Quantizer(L, prLbest,classPriors.at(0), classPriors.at(1),)
		}
	}

	// will do the actual quantization for L bins/component.
	// I am not sure what it should return, I could create another object that holds the L
	// probability of correct identification and the optimized quantizing interval boundaries for this L?
	// I don't know about this one
}

int main() {
	map<string,int> binProb;
	binProb["hello"] = 7;
	int a = binProb["hello"];
	cout << a << endl;
	cout << "hi" << endl;
	return 0;
}
