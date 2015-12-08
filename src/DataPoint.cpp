/*
 * DataPoint.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: Kelsey
 */

#include "DataPoint.h"

DataPoint::DataPoint(vector<double> dataVector, int givenTrueClass) {
	data = dataVector;
	trueClass = givenTrueClass;
	assignedClass = -1;
}

DataPoint::~DataPoint() {

}

vector<double> DataPoint::getDataVector() {
	return data;
}

int DataPoint::getAssignedClass() {
	return assignedClass;
}

int DataPoint::getTrueClass() {
	return trueClass;
}

void DataPoint::assignClass(int c) {
	assignedClass = c;
}
