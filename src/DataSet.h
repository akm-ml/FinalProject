/*
 * DataSet.h
 *
 *  Created on: Nov 30, 2015
 *      Author: Kelsey
 */

#ifndef DATASET_H_
#define DATASET_H_

// ideally this will be 1/3 of the data which we will use for each part
// one named trainingSet, one named estimatingSet, one named finalSet

#include "DataPoint.h"



class DataSet {
public:
	DataSet(); // ideally this will read a file and create a vector of DataPoint from the file
	virtual ~DataSet();
	vector<DataPoint> getSet();
	void createComponentSet();
	vector<double> getComponent(int i); // will return the ith component vector
	void sortComponentSet();

private:
	vector<DataPoint> set;
	vector<double> componentOne;
	vector<double> componentTwo;
	vector<double> componentThree;
	vector<double> componentFour;
	vector<double> componentFive;

};



#endif /* DATASET_H_ */
