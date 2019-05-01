/* parallel_collaborative_filtering.h */
#pragma once

#include <string>

#include "cf.h"

#ifndef pcf_h
#define pcf_h

/* parallel collaborative filtering */
return_code pcf(
    std::string filename,
    int num_threads,
    bool item_option
);


#endif