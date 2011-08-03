/*
 * menu.h
 *
 *  Created on: Jun 28, 2011
 *      Author: nquangtrung
 */

#include <stdlib.h>
#include <conio.h>
#include <string>
#include "book.h"
#include "data_manipulation.h"

using namespace std;

#ifndef INTERFACE_H_
#define INTERFACE_H_

// == Constants ==============================================================
#define MENU_MAIN 				0
#define MENU_BOOK 				1
#define MENU_TRANSACTION	 	2
#define MENU_SEARCH			 	3
#define MENU_ACTION_SELECT		4
#define FORM_BOOK_ID			5
#define FORM_SEARCH_TITLE		6
#define FORM_SEARCH_AUTHOR		7
#define FORM_BOOK_DETAIL		8
#define FORM_ACTION				9
#define FORM_ADD_BOOK			10
#define FORM_EDIT_BOOK			11
#define FORM_REM_BOOK			12
#define FORM_LEND_BOOK			13
#define FORM_RETURN_BOOK		14
#define FORM_SAVE				15
#define FORM_FAIL				16
#define FORM_SUCCESS			17
#define FORM_LOAD				18
#define FORM_LIST_ALL			19
#define FORM_QUERY			    20
#define FORM_VIEW_LENDED        21
#define FORM_OUTDATED_LEND      22

#define ACTION_NONE		 		0
#define ACTION_LEND		 		1
#define ACTION_RETURN	 		2
#define ACTION_ADD_BOOK	 		3
#define ACTION_REM_BOOK	 		4
#define ACTION_EDIT_BOOK	 	5

#define MAX_DISPLAY_LINE        15

// == Global Variables =======================================================
int 	CURRENT_MENU            = MENU_MAIN;
int 	CURRENT_ACTION          = ACTION_NONE;
char * 	CURRENT_BOOK_ID         = (char *) "";

// == Function Prototype =====================================================
void reset();
int show_interface();
int printMenu(string * str);
void clrscr();
BOOK_C * decorate(BOOK_C * head, BOOK_C * tail, int number = 0);

// menus
int mainmenu();
int bookmenu();
int transactionmenu();
int searchmenu();
int actionmenu();
// forms
int form_view_lended();
int form_oudated_lend();
int form_book_id();
int form_book_detail();
int form_book_search(int type);
int form_add_book();
int form_lend_book();
int form_return_book();
int form_rem_book();
int form_edit_book();
int form_success();
int form_fail();
int form_list_all();
int form_display(BOOK_C * head, 
                 BOOK_C * tail, 
                 int maxLine = MAX_DISPLAY_LINE);
int form_query(char * title = NULL, 
               char * author = NULL, 
               char * id = NULL, 
               int lended = 0, 
               struct tm * lended_time = NULL,
               int days_before = 0,
               BOOK_C * tHead = &head,
               BOOK_C * tTail = &tail);

// == Methods ================================================================
int form_view_lended() {
    query(NULL, NULL, NULL, 1);
    return form_display(&retHead, &retTail);
}
int form_outdated_lend() {
    time_t now;
    time(&now);
    struct tm * timeinfo = localtime(&now);
    query(NULL, NULL, NULL, 1, timeinfo, -30);
    return form_display(&retHead, &retTail);
}
int form_lend_book() {
    BOOK_C * bookptr = get(CURRENT_BOOK_ID);
    if (bookptr != NULL) {
       if (!bookptr->lended) {
            cout << "Are you sure you want to lend this book? (" << bookptr->title << ") (Y/N)\n";
            char c = getch();
            if (c == 'y' || c == 'Y') {
               bookptr->lended = true;
               time_t now;
               time(&now);
               bookptr->lended_time = localtime(&now);
               return FORM_SUCCESS;
            } else {
              return MENU_MAIN;
            }
       } else {
         return FORM_FAIL;
       }
    } else {
      return FORM_FAIL;
    }
}

int form_return_book() {
    BOOK_C * bookptr = get(CURRENT_BOOK_ID);
    if (bookptr != NULL) {
        if (bookptr->lended) {
            cout << "Are you sure you want to return this book? (" << CURRENT_BOOK_ID << ") (Y/N)\n";
            char c = getch();
            if (c == 'y' || c == 'Y') {
               int days = diffFromNow(bookptr->lended_time);
               if (days > MAX_LEND_TIME) {
                  // Outdated book
                  cout << "Lend time has surpassed " << MAX_LEND_TIME << " days." << endl;
                  cout << "Fine: " << days * OUTDATE_FEE << ".\n";
               }
               bookptr->lended = false;
               return FORM_SUCCESS;
            } else {
              return MENU_MAIN;
            }
        } else {
             return FORM_FAIL;
        }
    } else {
      return FORM_FAIL;
    }
}
void reset() {
	CURRENT_ACTION = ACTION_NONE;
	CURRENT_BOOK_ID = (char *)"";
	CURRENT_MENU = MENU_MAIN;
}
void clrscr() {
     // For windows only
     system("cls");
}
int printMenu(string str, int max) {
    int choose = 0;
    do {
        cout << str;
        cout << "Choose: "; cin >> choose;
    } while (!(choose >= 1 && choose <= max));
    return choose;
}
int form_query(char * title, 
               char * author, 
               char * id, 
               int lended, 
               struct tm * lended_time,
               int days_before,
               BOOK_C * tHead,
               BOOK_C * tTail) {
    char c;
    char * title_ = new char[100];
    if (title == NULL) {
       cout << "\nDo you want to search for title? (Y/N)"; 
       c = getch(); fflush(stdin);
       if (c == 'y' || c == 'Y') {
          cout << "\nPlease enter the keyword: ";
          gets(title_); fflush(stdin);
       } else {
         title_ = NULL;
       }
    } else {
      title_ = title;
    }
    
    char * author_ = new char[100];
    if (author == NULL) {
       cout << "\nDo you want to search for author? (Y/N)"; 
       c = getch(); fflush(stdin);
       if (c == 'y' || c == 'Y') {
          cout << "\nPlease enter the keyword: ";
          gets(author_); fflush(stdin);
       } else {
         author_ = NULL;
       }
    } else {
      author_ = author;
    }
    
    char * id_ = new char[100];
    if (id == NULL) {
       cout << "\nDo you want to search for ID? (Y/N)"; 
       c = getch(); fflush(stdin);
       if (c == 'y' || c == 'Y') {
          cout << "\nPlease enter the keyword: ";
          gets(id_); fflush(stdin);
       } else {
         id_ = NULL;
       }
    } else {
      id_ = id;
    }
    
    int lended_;
    if (lended == 0) {
       cout << "\nWhat do you want to search for (Lended - L, In Shelf - I, Don't care - D)";
       c = getch(); fflush(stdin);
       if (c == 'l' || c == 'L') lended_ = 1;
       else if (c == 'i' || c == 'I') lended_ = -1;
       else lended_ = 0;
    } else {
      lended_ = lended;
    }
    
    struct tm * timed;
    int days;
    if (lended_time == NULL) {
       cout << "\nDo you need a any time specify search? (Y/N)";
       c = getch(); fflush(stdin);
       if (c == 'y' || c == 'Y') {
          cout << "\nPlease enter the time under this format (Day/Month/Year, eg: 4/7/2010 for July 4th, 2010):  ";
          char * timestr = new char[100];
          gets(timestr); fflush(stdin);
          timed = fromString(timestr);
          cout << "Please enter the duration of time from that date: ";
          cin >> days;
       } else {
         timed = NULL;
         days = 0;
       }
    } else {
      timed = lended_time;
      days = days_before;
    }
    
    query(title_, author_, id_, lended_, timed, days, tHead, tTail);
    
    return form_display(&retHead, &retTail);
}
int form_book_search(int type) {
    char * kword = new char[100];
    
	cout << "Please input keyword: ";
	cin.ignore(); gets(kword); fflush(stdin);
	cout << "Searching..." << endl;
	
	clear_search();
	search(type, kword);
	
	return form_display(&retHead, &retTail);
}

/*
 * Return next menu
 */
int show_interface() {
	switch (CURRENT_MENU) {
    	case MENU_MAIN: return mainmenu();
    	case MENU_BOOK: return bookmenu();
    	case FORM_BOOK_ID: return form_book_id();
    	case FORM_BOOK_DETAIL: return form_book_detail();
    	case MENU_TRANSACTION: return transactionmenu();
    	case MENU_SEARCH: return searchmenu();
    	case FORM_SEARCH_TITLE: return form_book_search(SEARCH_TYPE_TITLE);
    	case FORM_SEARCH_AUTHOR: return form_book_search(SEARCH_TYPE_AUTHOR);
    	case MENU_ACTION_SELECT: return actionmenu();
    	case FORM_SAVE: save_raw_data(); return FORM_SUCCESS;
    	case FORM_LOAD: load_raw_data(); return FORM_SUCCESS;
    	case FORM_ADD_BOOK: return form_add_book();
    	case FORM_REM_BOOK: return form_rem_book();
    	case FORM_EDIT_BOOK: return form_edit_book();
    	case FORM_SUCCESS: return form_success();
    	case FORM_FAIL: return form_fail();
    	case FORM_LIST_ALL: return form_list_all();
    	case FORM_LEND_BOOK: return form_lend_book();
    	case FORM_RETURN_BOOK: return form_return_book();
    	case FORM_QUERY: return form_query();
    	case FORM_VIEW_LENDED: return form_view_lended();
    	case FORM_OUTDATED_LEND: return form_outdated_lend();
	}
	return MENU_MAIN;
}

int form_display(BOOK_C * head, BOOK_C * tail, int maxLine) {
    BOOK_C * _head = head;
    BOOK_C * sub = head;
    char c = 'l';
    do {
       sub = decorate(_head, tail, MAX_DISPLAY_LINE);
       cout << "N = Next | P = Previous | X = Return to Main Menu | A = Choose Book\n";
       cout << "R = Refine Search";
       c = getch(); fflush(stdin);
       if (c == 'n' || c == 'N') {
          if (sub != tail)
             _head = sub;
       } else if (c == 'p' || c == 'P') {
         sub = goPrev(sub, 2 * MAX_DISPLAY_LINE, head);
         _head = sub->prev;
       } else if (c == 'x' || c == 'X') {
         return MENU_MAIN;
       } else if (c == 's' || c == 'S') {
         // sort(head, tail);
         _head = &retHead;
         tail = &retTail;
         return MENU_MAIN;
       } else if (c == 'a' || c == 'A') {
         char * id = new char[100];
         
         bool fail = false;
         do {
            if (fail)
               cout << "Couldn't find the book associated with " << id << ". Please enter again.";
            cout << "\n\rPlease enter the ID: ";
            gets(id); fflush(stdin);
            fail = !hadID(id);
         } while (fail);
         CURRENT_BOOK_ID = id;
         return FORM_BOOK_DETAIL;
       } else if (c == 'r' || c == 'R') {
         return form_query(NULL, NULL, NULL, 0, NULL, 0, head, tail);
       }
    } while (true);
}
int form_list_all() {
    return form_display(&head, &tail);
}

int form_fail() {
	cout << "\n\rAction failed. \n\rPress any key to return to main menu...";
	getch();
	return MENU_MAIN;
}
int form_success() {
	cout << "\n\rAction successed. \n\rPress any key to return to main menu...";
	getch();
	return MENU_MAIN;
}

int form_rem_book() {
    cout << "Are you sure you want to remove this book? (" << CURRENT_BOOK_ID << ") (Y/N)\n";
    char c = getch();
    if (c == 'y' || c == 'Y')
    	if (remBook(CURRENT_BOOK_ID))
		   return form_success();
	    else return form_fail();
    else return MENU_MAIN;
}

int form_edit_book() {
	if (CURRENT_BOOK_ID == (char *)"") {
		return MENU_BOOK;
	} else {
		char t, a, i;
		char * title = new char[100];
		char * author = new char[100];
		char * id = new char[100];

		BOOK_C * book = get(CURRENT_BOOK_ID);
		if (book->lended) {
           cout << "Edit the book while it is lended may result in some fatal error.";
           cout << "This book is currently lended, do you really want to edit it? ";
        }
		cout << endl << "Library Management System - Edit" << endl;
		cout << "Please enter the following requested information to edit the book associated to id (" << CURRENT_BOOK_ID << ")" << endl;

		// Edit title
		cout << "Original Title: " << book->title << endl;
		cout << "Do you want to modify the title? (Y/N/X) ";
		t = getch(); fflush(stdin);

		if (t == 'y' || t == 'Y') {
			cout << "\n\rPlease enter new title: ";
			gets(title);
			fflush(stdin);
		} else if (t == 'x' || t == 'X') {
			return FORM_FAIL;
		} else {
			cout << "\n\rSkipped editing title." << endl;
		}

		// Edit Author
		cout << "Original Author: " << book->author << endl;
		cout << "Do you want to modify the author? (Y/N/X) ";
		a = getch(); fflush(stdin);
		if (a == 'y' || a == 'Y') {
			cout << "\n\rPlease enter new author: ";
			gets(author);
			fflush(stdin);
		} else if (a == 'x' || a == 'X') {
			return FORM_FAIL;
		} else {
			cout << "\n\rSkipped editing author." << endl;
		}

		// Edit ID
		cout << "Original ID: " << book->id << endl;
		cout << "Do you want to modify the ID? (Y/N/X) ";
		i = getch(); fflush(stdin);
		if (i == 'y' || i == 'Y') {
			bool fail;
			do {
				if (fail) {
					cout << "Your ID is not unique, please enter again.";
				}
				cout << "\n\rPlease enter new ID: ";
				gets(id); fflush(stdin);
				fail = hadID(id);
			} while(fail);
		} else if (i == 'x' || i == 'X') {
			return FORM_FAIL;
		} else {
			cout << "\n\rSkipped editing ID." << endl;
		}

		if (t == 'y' || t == 'Y') strcpy(book->title, title);
		if (a == 'y' || a == 'Y') strcpy(book->author, author);
		if (i == 'y' || i == 'Y') strcpy(book->id, id);
		
		return FORM_SUCCESS;
	}
}
int form_add_book() {
	char * title = new char[100];
	char * author = new char[100];
	char * id = new char[100];

	// Clear the cin and ignore the rest of the stream
	cin.clear();
	cin.ignore();

	cout << "Please input the following information to add a book: " << endl;
	cout << "Title: "; gets(title); fflush(stdin);
	cout << "Author: "; gets(author); fflush(stdin);
	
	bool fail = false;
	do {
        if (fail) {
           cout << "The ID is not unique, please enter again.\n";
        }
		cout << "ID: "; gets(id); fflush(stdin);
		fail = hadID(id);
	} while (fail);

	struct BOOK_C * book = (BOOK_C *) malloc(sizeof(BOOK_C));
	book->next = head.next;

	book->title = (char *) malloc(100);
	strcpy(book->title, title);

	book->author = (char *) malloc(100);
	strcpy(book->author, author);

	book->id = (char *) malloc(100);
	strcpy(book->id, id);

	book->lended = false;
	
	time_t now; time(&now);
	book->lended_time = localtime(&now);

	addBook(book);

	return FORM_SUCCESS;
}
int actionmenu() {
    clrscr();
	if (CURRENT_BOOK_ID != (char *)"") {
		if (CURRENT_ACTION == ACTION_NONE) {
			int choose = 0;
			do {
				cout << "Library Management System - Transaction";
				cout << "\n\rChoose the Action you want to do:";
				cout << "\n\r\t1. Edit book";
				cout << "\n\r\t2. Remove book";
				cout << "\n\r\t3. Lend";
				cout << "\n\r\t4. Return";
				cout << "\n\r\t5. Return to main menu";
				cout << "\n\r\tChoose: ";
				cin >> choose;
			} while (!((choose >= 1)&&(choose <= 5)));
			switch (choose) {
			case 1: CURRENT_ACTION = ACTION_EDIT_BOOK;
				break;
			case 2: CURRENT_ACTION = ACTION_REM_BOOK;
				break;
			case 3:	CURRENT_ACTION = ACTION_LEND;
				break;
			case 4:	CURRENT_ACTION = ACTION_RETURN;
				break;
			case 5: return MENU_MAIN;
			default: CURRENT_ACTION = ACTION_NONE;
			}

		}
		switch (CURRENT_ACTION) {
		case ACTION_NONE: dbg("Action none");
			return MENU_MAIN;
		case ACTION_EDIT_BOOK: dbg("Edit book");
			return FORM_EDIT_BOOK;
		case ACTION_REM_BOOK: dbg("Remove book");
			return FORM_REM_BOOK;
		case ACTION_LEND: dbg("Lend book");
			return FORM_LEND_BOOK;
		case ACTION_RETURN: dbg("Return");
			return FORM_RETURN_BOOK;
		default: dbg("Wrong");
			return MENU_MAIN;
		}
		return MENU_MAIN;
	} else {
		int choose = 0;
		do {
			cout << "Library Management System - Transaction";
			cout << "\n\rChoose the Action you want to do:";
			cout << "\n\r\t1. Add book";
			cout << "\n\r\t2. Edit book";
			cout << "\n\r\t3. Remove book";
			cout << "\n\r\t4. Lend";
			cout << "\n\r\t5. Return";
			cout << "\n\r\t6. Return to main menu";
			cout << "\n\r\tChoose: ";
			cin >> choose;
		} while (!((choose >= 1)&&(choose <= 6)));
		switch (choose) {
		case 1:
			CURRENT_ACTION = ACTION_ADD_BOOK;
			return FORM_ADD_BOOK;
		case 2:
			CURRENT_ACTION = ACTION_EDIT_BOOK;
			return FORM_EDIT_BOOK;
		case 3:
			CURRENT_ACTION = ACTION_REM_BOOK;
			break;
		case 4:
			CURRENT_ACTION = ACTION_LEND;
			break;
		case 5:
			CURRENT_ACTION = ACTION_RETURN;
			break;
		case 6:
			return MENU_MAIN;
		default: CURRENT_ACTION = ACTION_NONE;
		}
		return MENU_BOOK;
	}
}
int searchmenu() {
    clrscr();
	int choose = 0;
	do {
		cout << "Library Management System - Transaction";
		cout << "\n\rChoose the Action you want to do:";
		cout << "\n\r\t1. Search by title";
		cout << "\n\r\t2. Search by author";
		cout << "\n\r\t3. List all book";
		cout << "\n\r\t4. Return to main menu";
		cout << "\n\r\tChoose: ";
		cin >> choose;
	} while (!((choose >= 1)&&(choose <= 4)));

	switch (choose) {
	case 1:	return FORM_SEARCH_TITLE;
	case 2:	return FORM_SEARCH_AUTHOR;
	case 3:	return FORM_LIST_ALL;
	case 4: return MENU_MAIN;
	default: return MENU_MAIN;
	}
}

/*
 *
 */
int transactionmenu() {
    clrscr();
	int choose = 0;
	do {
		cout << "Library Management System - Transaction";
		cout << "\n\rChoose the Action you want to do:";
		cout << "\n\r\t1. Lend";
		cout << "\n\r\t2. Return";
		cout << "\n\r\t3. View Lended Book";
		cout << "\n\r\t4. View Oudated Lend";
		cout << "\n\r\t5. Return to main menu";
		cout << "\n\r\tChoose: ";
		cin >> choose;
	} while (!((choose >= 1)&&(choose <= 5)));

	switch (choose) {
	case 1:
		CURRENT_ACTION = ACTION_LEND;
		return MENU_BOOK;
	case 2:
		CURRENT_ACTION = ACTION_RETURN;
		return MENU_BOOK;
	case 3:	return FORM_VIEW_LENDED;
	case 4: return FORM_OUTDATED_LEND;
   	case 5: return MENU_MAIN;
	default: return MENU_MAIN;
	}
}

char * actionName(int action) {
     return "";
}

int form_book_detail() {
    clrscr();
	// This mean we have already chose a book
	BOOK_C * book;
	book = get(CURRENT_BOOK_ID);
    cout << "You've chosen the book associated to  book "<< CURRENT_BOOK_ID << endl;
	cout << "Title: " << book->title << "." << endl;
	cout << "Author: " << book->author << "." << endl;
	cout << "Status: " << (book->lended?"Lended":"In shelf") << "." << endl << endl;
	
    if (CURRENT_ACTION == ACTION_NONE)
    	cout << "Press any key to choose action to do with this book..." << endl;
   	else cout << "Press any key to continue doing the chosen action to this book...";
    getch();
	return MENU_ACTION_SELECT;
}
/*
 * Display book menu
 * Return the detail view form of book
 */
int form_book_id() {
    clrscr();
    
	char * id = new char[100];
    strcpy(id, "");
    
    bool fail = false;
	    
    do {
		//TODO: Repeatedly enter the id until the id is found in the record table, and
		// 		then set the CURRENT_BOOK_ID to inputed id
		cout << "Library Management System - BOOK";
		if (fail) cout << "\n\rCouldn't find any book associated with " << id << "." << endl;
		   else cin.ignore();
		cout << "\n\rPlease enter the book's id: ";
        gets(id); fflush(stdin);
        cout << id << endl;
        fail = !hadID(id);
	} while (!hadID(id));

	CURRENT_BOOK_ID = id;
	
	return FORM_BOOK_DETAIL;
}
/*
 * Display book menu
 * Return the coresponded menu number
 */
int bookmenu() {
    clrscr();
    int choose = 0;
	do {
		cout << "Library Management System - BOOK";
		cout << "\n\rChoose the Action you want to do:";
		cout << "\n\r\t1. Choose book using ID";
		cout << "\n\r\t2. Search for book title";
		cout << "\n\r\t3. Search for book author";
		cout << "\n\r\t4. List all book";
		cout << "\n\r\t5. Custom Query";
		cout << "\n\r\t6. Return to main menu";
		cout << "\n\r\tChoose: ";
		cin >> choose;
	} while (!((choose >= 1)&&(choose <= 6)));

	switch (choose) {
	case 1: return FORM_BOOK_ID;
	case 2: return FORM_SEARCH_TITLE;
	case 3: return FORM_SEARCH_AUTHOR;
	case 4: return FORM_LIST_ALL;
	case 5: return FORM_QUERY;
	case 6: return MENU_MAIN;
	default: return MENU_MAIN;
	}
}
/*
 * Display main menu
 * Return the chosen menu number
 */
int mainmenu() {
    clrscr(); 
    reset();
	int choose = 0;
	do {
		cout << "Library Management System";
		cout << "\n\rChoose the Action you want to do:";
		cout << "\n\r\t1. Books";
		cout << "\n\r\t2. Transactions";
		cout << "\n\r\t3. Search";
		cout << "\n\r\t4. Choose Action";
		cout << "\n\r\t5. Load";
		cout << "\n\r\t6. Save";
		cout << "\n\r\t7. Exit";
		cout << "\n\r\tChoose: ";
		cin >> choose;
	} while (!((choose >= 1)&&(choose <= 7)));

	switch (choose) {
	case 1: return MENU_BOOK;
	case 2: return MENU_TRANSACTION;
	case 3: return MENU_SEARCH;
	case 4:	return MENU_ACTION_SELECT;
	case 5: return FORM_LOAD;
	case 6: return FORM_SAVE;
	case 7: save_raw_data(); exit(0);
	default: return MENU_MAIN;
	}
}

char * fixWidth(char * strptr, int maxWidth) {
     char * ret = new char [100];
     strcpy(ret, strptr);
     int len = strlen(strptr);
     if (len < maxWidth) {
        for (int i = len; i < maxWidth; i++) {
            ret[i] = ' ';
            ret[i+1] = '\0';
        }
     } else if (len > maxWidth) {
       ret[maxWidth] = '\0';
       ret[maxWidth - 1] = '.';
       ret[maxWidth - 2] = '.';
       ret[maxWidth - 3] = '.';
     }
     return ret;
}
BOOK_C * decorate(BOOK_C * head, BOOK_C * tail, int number) {
     clrscr();
     BOOK_C * current = head->next;
     int cnt = 0;
     cout << endl;
     cout << "+==============================================================================+";
     cout << "| ID |                Title               |       Author     |LENDED|  L.Date  |";
     cout << "|----|------------------------------------|------------------|------|----------|";
     while ((current != tail)&&((number == 0) || (cnt < number)) ) {
           cout << '|' << fixWidth(current->id, 4);
           cout << '|' << fixWidth(current->title, 36);
           cout << '|' << fixWidth(current->author, 18);
           cout << '|' << setw(6) << (current->lended?"YES":"NO");
           cout << '|' << setw(2) << current->lended_time->tm_mday << '/';
           cout << setw(2) << current->lended_time->tm_mon + 1 << '/';
           cout << setw(4) << current->lended_time->tm_year + 1900 << '|';
           // cout << '|' << setw(10) << diffFromNow(current->lended_time) << "|";
           current = current -> next;
           cnt += 1;
     }
     if (cnt == 0)
        cout << " <Query empty> \n";
     cout << "+==============================================================================+";
     return current;
}
#endif /* INTERFACE_H_ */
