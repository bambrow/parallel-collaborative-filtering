/* collaborative_filtering.h */
#pragma once

#ifndef cf_h
#define cf_h

/* debug flags */
// #define DEBUG_1
// #define DEBUG_2
// #define DEBUG_3

/* constants */
#define MISSING_RATING 99;
#define NUM_MOST_SIMILAR 5;
#define MAX_RATING 10;
#define MIN_RATING 1;

/* similarity tuple struct */
struct SimTuple
{
    SimTuple(int id, float similarity)
            : id(id), similarity(similarity) {}
    int id;
    float similarity;
};

/* similarity tuple comparator */
struct CompareSimTuple {
    bool operator()(const SimTuple& p1, const SimTuple& p2);
};

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