/*
 * Quantizer.h
 *
 *  Created on: Nov 30, 2015
 *      Author: Kelsey
 */

#ifndef QUANTIZER_H_
#define QUANTIZER_H_

#include "DataSet.h"

// for a specific L, this structure will hold everything specific to that quantizer
//such as L, probability of correct identification, the bayes rule (i.e. bin probabilities or class tables)
//and the quantized interval boundaries required for testing
// we will compare the current Quantizer with the one associated with Lbest, and replace if current is better
// we will only have one at a time, we only need to store Lbest always


class Quantizer {

public:
	Quantizer();
	virtual ~Quantizer();


private:
	int quantizedLevelsPerComponent; // the best one so far
	double probabilityOfCorrectIdentification; //this is just the one associated with Lbest
	vector<vector<double> > bayesTableForClass0;
	// this is the decision rule table for class0 where each i,j entry holds the
	// value we compare for the bayes rule
	vector<vector<double> > bayesTableForClass1; // same for class 1
};



#endif /* QUANTIZER_H_ */
