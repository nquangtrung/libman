/*
 * overall_header.h
 *
 *  Created on: Jun 29, 2011
 *      Author: nquangtrung
 */

#include <string>

using namespace std;

#ifndef OVERALL_HEADER_H_
#define OVERALL_HEADER_H_

bool debug = true;

void dbg(string s) {
	if (debug)
		cout << s << endl;
}

void dbg(int s) {
	if (debug)
		cout << s << endl;
}

void log(string s) {
	cout << s;
}

#endif /* OVERALL_HEADER_H_ */
