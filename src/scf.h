/* serial_collaborative_filtering.h */
#pragma once

#include <string>

#include "cf.h"

#ifndef scf_h
#define scf_h

/* serial collaborative filtering */
return_code scf(
    std::string filename,
    bool item_option
);


#endif