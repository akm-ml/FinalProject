/*
 * binAddress.h
 *
 *  Created on: Dec 16, 2015
 *      Author: Kelsey
 */

#ifndef BINADDRESS_H_
#define BINADDRESS_H_
#include "DataSet.h"

class binAddress{
public:
	binAddress(vector<int> quantizedBinNumber);
	string binKey();
	vector<int> binNumber;
};




#endif /* BINADDRESS_H_ */


binAddress::binAddress(vector<int> quantizedBinNumber) {
	binNumber = quantizedBinNumber;
	string binKey;
	for (unsigned int i = 0 ; i < quantizedBinNumber.size() ; i++) {
		if (i > 0) {
			binKey += ':';
		}
		int a = quantizedBinNumber.at(i);
		binKey += a;
	}
}

