/* collaborative_filtering.h */
#pragma once

#ifndef cf_h
#define cf_h

/* debug flags */
#define DEBUG_1
#define DEBUG_2
#define DEBUG_3
#define DEBUG_4
#define DEBUG_5

/* constants */
#define MISSING_RATING 99;

/* return codes */
enum return_code {
     SUCCESS,
     ILLEGAL_ARGUMENT_NUMBER,
     ILLEGAL_ARGUMENT,
     FILE_NOT_FOUND,
     ILLEGAL_FILE_FORMAT,
     UNKNOWN_ERROR
};

#endif