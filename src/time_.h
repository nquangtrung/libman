/*
 * time_.h
 *
 *  Created on: Jun 29, 2011
 *      Author: nquangtrung
 */
 
#include <time.h>

#ifndef TIME__H_
#define TIME__H_

int diffFromNow(struct tm * timeinfo) {
    time_t now;
    time(&now);
    double dif = difftime(now, mktime(timeinfo));
    return (int)dif/3600/24;
}
struct tm * fromString(char * str) {
       string sub = "";
       int date = 0, month = 0, year = 0;
       int cnt = 0;
       for (int i = 0; i < strlen(str); i++) {
           if (str[i] == '/') {
              switch(cnt) {
              case 0: date = atoi(sub.c_str()); break;
              case 1: month = atoi(sub.c_str()); break;
              }
              cnt++;
              sub = "";
           } else {
             sub += str[i];
           }
       }
       
       year = atoi(sub.c_str());
       
       struct tm * timeinfo = (struct tm *) malloc(sizeof(tm));
       timeinfo->tm_year = year - 1900;
       timeinfo->tm_mon = month - 1;
       timeinfo->tm_mday = date;
       timeinfo->tm_hour = 0;
       timeinfo->tm_min = 0;
       timeinfo->tm_sec = 1;
       timeinfo->tm_isdst = -1;
       
       return timeinfo;
}

#endif /* TIME__H_ */
