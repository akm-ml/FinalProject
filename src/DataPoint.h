#ifndef DATAPOINT_H_
#define DATAPOINT_H_

#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

class DataPoint {

public:
	DataPoint(vector<double> dataVector, int givenTrueClass);
	virtual ~DataPoint();
	vector<double> getDataVector();
	int getAssignedClass();
	int getTrueClass();
	void assignClass(int c);

private:
	vector<double> data;
	int assignedClass;
	int trueClass;

};


#endif
