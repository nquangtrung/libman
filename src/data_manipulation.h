/*
 * data_manipulation.h
 *
 *  Created on: Jun 28, 2011
 *      Author: nquangtrung
 */

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "book.h"
#include "time_.h"
#include "overall_header.h"

using namespace std;

#ifndef DATA_MANIPULATION_H_
#define DATA_MANIPULATION_H_

#define fname = "db";

void load_raw_data();
void save_raw_data();

void load_raw_data() {
	dbg("\n\rLoading raw data from file... ");

	ifstream inFile ("db");
	
	empty_record();

	while (inFile) {
		string line = "";
		getline(inFile, line);

		string sub = "";
		string title = "", author = "", id = "";
		struct tm * timeinfo;
		bool lended;
		int cnt = 0;
		for (int i = 0; i < (signed)line.size(); i++) {
			if (line[i] != '|') {
				sub += line[i];
			} else {
				switch(cnt) {
				case 0: title = sub;
					break;
				case 1: author = sub;
					break;
				case 2: id = sub;
					break;
				case 3: timeinfo = fromString((char *)sub.c_str());
					break;
				}
				cnt++;
			    sub = "";
			}
		}
		if (sub == "1") {
			lended = true;
		} else {
			lended = false;
		}

		// Only parse if title is not blank
		if (title != "") {
			struct BOOK_C * book = (BOOK_C *) malloc(sizeof(BOOK_C));
			book->title = new char[100];
			strcpy(book->title, title.c_str());

			book->author = new char[100];
			strcpy(book->author, author.c_str());

			book->id = new char[100];
			strcpy(book->id, id.c_str());

			book->lended = lended;
			
			book->lended_time = timeinfo;
			
			addBook(book);
		}
	}
	inFile.close();

	dbg("Finished.");
}

void save_raw_data() {
	dbg("\n\rStart saving data... ");
	struct BOOK_C * current = head.next;

	ofstream outFile ("db");

	int no = 0;
	while (current != &tail) {
		outFile << current->title << "|";
        outFile << current->author << "|" ;
        outFile << current->id << "|";
		outFile << current->lended_time->tm_mday << "/";
        outFile << current->lended_time->tm_mon + 1 << "/";
        outFile << current->lended_time->tm_year + 1900 << "|";
		if (current->lended) {
			outFile << "1" << endl;
		} else {
			outFile << "0" << endl;
		}
		current = current->next;
		no+=1;
	}
	outFile.close();
	dbg("Finished.");
}

#endif /* DATA_MANIPULATION_H_ */
