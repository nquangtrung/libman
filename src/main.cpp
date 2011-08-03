/*
 * main.cpp
 *
 *  Created on: Jun 28, 2011
 *  Author: nquangtrung
 */
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "book.h"
#include "interface.h"
#include "time_.h"
#include "data_manipulation.h"

using namespace std;

void init() {
    link(&head, &tail);
    link(&retHead, &retTail);
    
	load_raw_data();
}
int main() {
	init();
	while (true) {
		CURRENT_MENU = show_interface();
	}
	return 0;
}
