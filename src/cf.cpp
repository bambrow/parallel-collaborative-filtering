
#include <iostream>
#include <string>

#include "cf.h"
#include "scf.h"
#include "pcf.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        cout << "Usage: ./cf <filename> <extra_options>" << endl;
        cout << "-s: this option turns on serial algorithm" << endl;
        cout << "-p: this option (default) turns on parallel algorithm" << endl;
        // cout << "-m: this option turns on sparse matrix" << endl;
        cout << "-i: this option turns on item-item similarity" << endl;
        cout << "-u: this option (default) turns on user-user similarity" << endl;
        return ILLEGAL_ARGUMENT_NUMBER;
    }

    const string filename = argv[1];

    bool serial_option = false;
    // bool sparse_option = false;
    bool item_option = false;

    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            string cur_arg = argv[i];
            if (cur_arg == "-s" || cur_arg == "-S") {
                serial_option = true;
            } 
            else if (cur_arg == "-p" || cur_arg == "-P") {
                serial_option = false;
            }
            /* 
            else if (cur_arg == "-m" || cur_arg == "-M") {
                sparse_option = true;
            } 
            */
            else if (cur_arg == "-i" || cur_arg == "-I") {
                item_option = true;
            } 
            else if (cur_arg == "-u" || cur_arg == "-U") {
                item_option = false;
            }
        }
    }

    if (serial_option) {
        return scf(filename, item_option);
    }

    return pcf(filename, item_option);
}

bool CompareSimTuple::operator()(const SimTuple& p1, const SimTuple& p2)
{
    return p1.similarity < p2.similarity;
}

