/*
 * book.h
 *
 *  Created on: Jun 29, 2011
 *      Author: nquangtrung
 */

#include <string.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>

using namespace std;

#ifndef BOOK_H_
#define BOOK_H_

#define MAX_LEND_TIME 30
#define OUTDATE_FEE 5

#define SEARCH_TYPE_TITLE       0
#define SEARCH_TYPE_AUTHOR      1

#define SORT_ASCENDING          true
#define SORT_DECENDING          false

struct BOOK_C{
	char* title;
	char* author;
	char* id;
	bool lended;
	
	struct tm * lended_time;
	
	struct BOOK_C * next;
	struct BOOK_C * prev;
};

struct BOOK_C head = {(char *)"head_title", (char *)"head_author", (char *)"head_id", 1, NULL, NULL, NULL};
struct BOOK_C tail = {(char *)"tail_title", (char *)"tail_author", (char *)"tail_id", 1, NULL, NULL, NULL};

struct BOOK_C retHead = {(char *)"head_title", (char *)"head_author", (char *)"head_id", 1, NULL, NULL, NULL};
struct BOOK_C retTail = {(char *)"tail_title", (char *)"tail_author", (char *)"tail_id", 1, NULL, NULL, NULL};

BOOK_C * get(char * id);
bool remBook(char * id);
void removeBook(BOOK_C * id);
BOOK_C * search(int sType, char * kword);
BOOK_C * get(char * id);
void link(BOOK_C * first, BOOK_C * next);
BOOK_C * bookcpy(BOOK_C * bookptr);
void clear_search();
BOOK_C * goNext(BOOK_C * bookptr, int step, BOOK_C * sTail = &tail);
BOOK_C * goPrev(BOOK_C * bookptr, int step, BOOK_C * sHead = &head);
void query(char * title = NULL, 
           char * author = NULL, 
           char * id = NULL, 
           int lended = 0, 
           struct tm * lended_time = NULL,
           int days_before = 0, 
           BOOK_C * tHead = &head,
           BOOK_C * tTail = &tail);
int compTitle(BOOK_C * book1, BOOK_C * book2);
void sort(BOOK_C * tHead = &retHead, 
          BOOK_C * tTail = &retHead, 
          int comp(BOOK_C *, BOOK_C *) = &compTitle, 
          bool direction = SORT_ASCENDING);
bool empty_record();
bool search_empty();

void clear_search() {
     link(&retHead, &retTail);     
}

void link(BOOK_C * first, BOOK_C * next) {
     first->next = next;
     next->prev = first;
}

BOOK_C * goNext(BOOK_C * bookptr, int step, BOOK_C * sTail) {
       BOOK_C * retptr = bookptr;
       for (int i = 0; i < step && retptr->next != sTail; i++) 
           retptr = retptr->next;
       return retptr;
}
BOOK_C * goPrev(BOOK_C * bookptr, int step, BOOK_C * sHead) {
       BOOK_C * retptr = bookptr;
       for (int i = 0; i < step && retptr->prev != sHead; i++)
           retptr = retptr->prev;
       return retptr;
}

bool remBook(char * id) {
	BOOK_C * current = head.next;
	while (current != &tail) {
		if (strcmp(id, current->id) == 0) {
            if (!current->lended) {
			   removeBook(current);
			   return true;
            } else {
			  return false;
            }
		}
		current = current->next;
	}
	return false;
}
bool hadID(char * id) {
	return (get(id) != NULL);
}
BOOK_C * get(char * id) {
	BOOK_C * current = head.next;
	while (current != NULL) {
		if (strcmp(id, current->id) == 0) return current;
		current = current->next;
	}
	return NULL;
}

void addBook(BOOK_C * bookptr) {
     link(tail.prev, bookptr);
     link(bookptr, &tail);
}

void removeBook(BOOK_C * bookptr) {
     link(bookptr->prev, bookptr->next);
     free(bookptr);
}

bool match(char * str, char * kword) {
     int count = 0;
     for (int i = 0; i < strlen(str); i++) {
         if (str[i] == kword[count]) count++;
         if (count == strlen(kword)) return true;
     }
     return false;
}

BOOK_C * bookcpy(BOOK_C * bookptr) {
        struct BOOK_C * book = (BOOK_C *) malloc(sizeof(BOOK_C));
        book->title = new char[100];
        strcpy(book->title, bookptr->title);
        
        book->author = new char[100];
        strcpy(book->author, bookptr->author);
        
        book->id = new char[100];
        strcpy(book->id, bookptr->id);
        
        book->lended_time = bookptr->lended_time;
        
        book->lended = bookptr->lended;
        
        return book;
}
int compTitle(BOOK_C * book1, BOOK_C * book2) {
    return strcmp(book1->title, book2->title);
           
}
void sort(BOOK_C * tHead, 
          BOOK_C * tTail, 
          int comp(BOOK_C *, BOOK_C *), 
          bool direction) {
     BOOK_C * subHead = bookcpy(tHead);
     BOOK_C * subTail = bookcpy(tTail);
     if (tHead == &retHead && tTail == &retTail) {
         // Copy the record into another list just in case tHead and tTail is 
         // retHead adn retTail
         link(subHead, subTail);
         
         BOOK_C * current = tHead->next;
         while (current != tTail) {
               BOOK_C * sub = bookcpy(current);
               link(subTail->prev, sub);
               link(sub, subTail);
               current = current->next;
         }
      } else {
           subHead = tHead;        
           subTail = tTail;
      }
      
      BOOK_C * current_i = subHead->next;
      while (current_i != subTail->prev) {
            BOOK_C * current_j = current_i->next;
            while (current_j != subTail) {
                  cout << current_j->title;
                  bool swap = ((comp(current_i, current_j) > 0) && direction)
                            ||((comp(current_i, current_j) < 0) && !direction);
                  if (swap) {                     
                     BOOK_C * prevtemp = current_j->prev;
                     BOOK_C * nexttemp = current_j->next;
                     
                     link(current_i->prev, current_j);
                     link(current_j, current_i->next);
                     
                     link(prevtemp, current_i);
                     link(current_i, nexttemp);
                  }
                  
                  current_j = current_j->next;
            }
            current_i = current_i->next;
      }
      retHead = *subHead;
      retTail = *subTail;
}

void query(char * title, 
           char * author, 
           char * id, 
           int lended, 
           struct tm * lended_time,
           int days_before,
           BOOK_C * tHead,
           BOOK_C * tTail) {
     BOOK_C * subHead = bookcpy(tHead);
     BOOK_C * subTail = bookcpy(tTail);
     if (tHead == &retHead && tTail == &retTail) {
         // Copy the record into another list just in case tHead and tTail is 
         // retHead adn retTail
         link(subHead, subTail);
         
         BOOK_C * current = tHead->next;
         while (current != tTail) {
               BOOK_C * sub = bookcpy(current);
               link(subTail->prev, sub);
               link(sub, subTail);
               current = current->next;
         }
      } else {
           subHead = tHead;        
           subTail = tTail;
      }
     
     // Clear the result
     link(&retHead, &retTail);
     
     BOOK_C * current = subHead->next;
     while (current != subTail) {
           bool m = true;
           if (m && title != NULL)
              m = m && match(current->title, title);
           if (m && author != NULL)
              m = m && match(current->author, author);
           if (m && id != NULL)
              m = m && (strcmp(current->id, id) == 0);
           if (m && lended != 0) {
              if (lended > 0)
                 m = m && (current->lended);
              else
                 m = m && (!current->lended);  
           }
           if (m && lended_time != NULL) {
              time_t t1 = mktime(lended_time);
              time_t t2 = mktime(current->lended_time);
              
              if (days_before > 0) {
                 m = m && (difftime(t1, t2)/3600/24 <= days_before && difftime(t1, t2)/3600/24 > 0);
              } else if (days_before < 0) {
                m = m && (difftime(t1, t2)/3600/24 >= days_before && difftime(t1, t2)/3600/24 < 0);
              } else {
                m = m && (difftime(t1, t2)/3600/24 == 0);
              }
           }
           
           if (m) {
               BOOK_C * bookptr = bookcpy(current);
               link(retTail.prev, bookptr);
               link(bookptr, &retTail);
           }
           current = current->next;
     }
}

BOOK_C * search(int sType, char * kword) {
     if (sType == SEARCH_TYPE_TITLE)
        query(kword);
     else if (SEARCH_TYPE_AUTHOR)
        query(NULL, kword);
}
void print(BOOK_C * bookptr) {
     cout << bookptr->title << "-" << bookptr->author << "-" << bookptr->id << "-" << bookptr->lended << endl;
}
bool empty_record() {
     struct BOOK_C * current = &head;
     while (current->next != &tail) {
           current = current->next;
           free(current->prev);
     }
     link(&head, &tail);
}
bool search_empty() {
     return (retHead.next = &retTail);
}
#endif /* BOOK_H_ */
