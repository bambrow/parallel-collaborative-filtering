/* collaborative_filtering.cpp */
#include <iostream>
#include <string>

#include "cf.h"
#include "scf.h"
#include "pcf.h"

using namespace std;

/* main function */

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        cout << "Usage: ./cf <filename> <num_threads> <extra_options>" << endl;
        cout << "<num_threads> (default is 5) will be ignored when -s option is chosen" << endl;
        cout << "-s: this option turns on serial algorithm" << endl;
        cout << "-p: this option (default) turns on parallel algorithm" << endl;
        cout << "-i: this option turns on item-item similarity" << endl;
        cout << "-u: this option (default) turns on user-user similarity" << endl;
        cout << "Examples: " << endl;
        cout << "./cf <filename> -s" << endl;
        cout << "./cf <filename> 5" << endl;
        cout << "./cf <filename> -s -i" << endl;
        cout << "./cf <filename> 5 -i" << endl;
        return ILLEGAL_ARGUMENT_NUMBER;
    }

    const string filename = argv[1]; // parse filename

    bool serial_option = false; // default is parallel
    bool item_option = false; // default is user-user
    int num_threads = 5; // default number of threads

    // parse additional command line arguments
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            string cur_arg = argv[i];
            if (cur_arg == "-s" || cur_arg == "-S") {
                serial_option = true;
            } 
            else if (cur_arg == "-p" || cur_arg == "-P") {
                serial_option = false;
            }
            else if (cur_arg == "-i" || cur_arg == "-I") {
                item_option = true;
            } 
            else if (cur_arg == "-u" || cur_arg == "-U") {
                item_option = false;
            }
            else {
                try {
                    num_threads = atoi(argv[i]);
                } 
                catch (...) {
                    cout << "Illegal argument: please check the readme for how to use this program." << endl;
                    cout << "Usage: ./cf <filename> <num_threads> <extra_options>" << endl;
                    throw ILLEGAL_ARGUMENT;
                }
            }
        }
    }

    if (serial_option) {
        return scf(filename, item_option);
    }

    return pcf(filename, num_threads, item_option);
}

/* similarity tuple comparator */

bool CompareSimTuple::operator()(const SimTuple& p1, const SimTuple& p2)
{
    return p1.similarity < p2.similarity;
}

