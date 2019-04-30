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
#define NUM_MOST_SIMILAR 2;

/* structs */
struct SimTuple
{
    SimTuple(int id, float similarity)
            : id(id), similarity(similarity) {}
    int id;
    float similarity;
};

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