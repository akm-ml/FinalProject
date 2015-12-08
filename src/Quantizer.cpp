/*
 * Quantizer.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: Kelsey
 */

// this should hold all of the things we want to do to the objects and main

#include "Quantizer.h"
#include <random>
#include <math.h>

Quantizer::Quantizer() {
	// TODO
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
vector<double> calculatePriors(DataSet ds) {
	double class0Count = 0, class1Count = 0;
	int currentClass = -1;
	for (unsigned i = 0 ; i < ds.getSet().size() ; i++) {
		currentClass = ds.getSet().at(i).getTrueClass();
		if (currentClass == 0) {
			class0Count++;
		} else {
			class1Count++;
		}
	}
	class0Count = class0Count/ds.getSet().size();
	class1Count = class1Count/ds.getSet().size();
	return {class0Count,class1Count};
}

void quantize(int L) {
	// will do the actual quantization for L bins/component.
	// I am not sure what it should return, I could create another object that holds the L
	// probability of correct identification and the optimized quantizing interval boundaries for this L?
	// I don't know about this one
}

vector<double> calculateBinProbabilities() {
	// populates tables for the two classes based on the L specified above, this will probably be called INSIDE
	// quantize, two 2d tables where C1[i][j]= bin i,j probability given c1
	return {0,0};
}

double bayesProbability() {
	// returns the probability of correct identification for the specific L that we are looking at
	// this will probably also be called inside quantize
	return 0;
}

vector<vector<double> > intialIntervals(int L, DataSet trainingSet) {
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


//TODO
vector<vector<double> > greedyAlgorithm(vector<vector<double> > initialIntervals, int L, DataSet trainingSet) {
	int j = rand() % 5 + 1;
	int k = rand() % L + 1;
	vector<double> componentJ = trainingSet.getComponent(j);
	double bnew = componentJ.at(k);
	// determine minimum distance between bkj and different bij by going through the jth component
	double distanceDown = bnew - componentJ.at(k-1);
	double distanceUp = 0;
	if (k < L) {
		distanceUp = componentJ.at(k+1) - bnew;
	} // components are already sorted so we are looking for what kind of M and delta we can use here

	double delta = fRand(distanceDown,distanceUp);// small perturbation, between distanceDown and distanceUp

	double maxDistance = min(distanceDown, distanceUp);

	int maxM = (int) floor (maxDistance/(2*delta));

	int M = rand() % maxM;// small integer between 0 and min(distanceDown,distanceUp)/delta;

	for (int m = 0 ; m <= 2*M ; m++) {
		bnew = bnew + delta;
		vector<double> binProbabilities = calculateBinProbabilities();
		double bayesRule = bayesProbability();
		// save expected gain
	}
}

