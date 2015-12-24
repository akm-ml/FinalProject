/*
 * DataSet.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: Kelsey
 */

#include "DataSet.h"
#include <algorithm>

DataSet::DataSet() {
	vector<double> componentOne = {0};
	vector<double> componentTwo = {0};
	vector<double> componentThree = {0};
	vector<double> componentFour = {0};
	vector<double> componentFive = {0};
}

DataSet::~DataSet() {

}

vector<DataPoint> DataSet::getSet() {
	return set;
}

void DataSet::setSet(vector<DataPoint> dpVect) {
	set = dpVect;
}

void DataSet::createComponentSet() { // not sure about the bounds for my counter here
	//cout << "creating component set " << endl;
	for (unsigned int i = 0 ; i < set.size() ; i++) {
		DataPoint dp = set.at(i);
		vector<double> dataVector = dp.getDataVector();
		componentOne.push_back(dataVector.at(0));
		componentTwo.push_back(dataVector.at(1));
		componentThree.push_back(dataVector.at(2));
		componentFour.push_back(dataVector.at(3));
		componentFive.push_back(dataVector.at(4));
	}
	//cout << "component set created " << endl;
}

vector<double>* DataSet::getComponent(int i) { // i am not error checking here so I trust we will use this properly
	if (i == 1) {
		return &componentOne;
	} else if (i == 2) {
		return &componentTwo;
	} else if (i == 3) {
		return &componentThree;
	} else if (i == 4) {
		return &componentFour;
	} else {
		return &componentFive;
	}
}

void DataSet::sortComponentSet() {
	//cout << "sorting component set " << endl;
	sort(componentOne.begin(),componentOne.end());
	sort(componentTwo.begin(),componentTwo.end());
	sort(componentThree.begin(),componentThree.end());
	sort(componentFour.begin(),componentFour.end());
	sort(componentFive.begin(),componentFive.end());
	//cout << "component set sorted " << endl;
}
