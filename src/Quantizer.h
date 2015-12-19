/*
 * Quantizer.h
 *
 *  Created on: Nov 30, 2015
 *      Author: Kelsey
 */

#ifndef QUANTIZER_H_
#define QUANTIZER_H_

#include "DataSet.h"
#include <map>
// for a specific L, this structure will hold everything specific to that quantizer
//such as L, probability of correct identification, the bayes rule (i.e. bin probabilities or class tables)
//and the quantized interval boundaries required for testing
// we will compare the current Quantizer with the one associated with Lbest, and replace if current is better
// we will only have one at a time, we only need to store Lbest always


class Quantizer {

public:
	Quantizer(int L, double pr, vector<double> priorProb, map<string,int> binProbabilitiesClass0, map<string,int> binProbabilitiesClass1);
	virtual ~Quantizer();
	void reSet(int L, double pr, vector<double> priorProb, map<string,int> binProbabilitiesClass0, map<string,int> binProbabilitiesClass1);
	double getProb();
	vector<double> getPriors();
	void setIntervalBounds(vector<vector<double> > intervalB);
	double generateEstimate(DataSet *estimateSet);
	void setFinalTestCorrectness(double ftc);
	void printQuantizer();

private:
	int quantizedLevelsPerComponent; // the best one so far
	double probabilityOfCorrectIdentification; //this is just the one associated with Lbest
	vector<double> priors; // change into hash map
	// this is the decision rule table for class0 where each i,j entry holds the
	// value we compare for the bayes rule
	 // change into hash map
	map<string,int> decisionRuleClass0;
	map<string,int> decisionRuleClass1;
	vector<vector<double> > leftIntervalBounds;
	double finalTestCorrectness = 0;
};



#endif /* QUANTIZER_H_ */
