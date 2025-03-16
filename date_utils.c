#include "date_utils.h"
#include <time.h>

int isValidDate(int year, int month, int day) {
    struct tm date = {0};
    
    // Set the date components
    date.tm_year = year - 1900;  // tm_year is years since 1900
    date.tm_mon = month - 1;     // tm_mon is 0-11
    date.tm_mday = day;          // tm_mday is 1-31
    
    // mktime will normalize the date and return -1 if date is invalid
    time_t timestamp = mktime(&date);
    if (timestamp == -1) {
        return 0;  // Invalid date
    }
    
    // Check if mktime adjusted the date (which means original was invalid)
    return (date.tm_year == year - 1900 && 
            date.tm_mon == month - 1 && 
            date.tm_mday == day);
}