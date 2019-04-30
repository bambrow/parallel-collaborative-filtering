
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

#include "cf.h"
#include "scf.h"

using namespace std;

return_code scf(
    string filename,
    bool item_option
) {
    ifstream in(filename);
    if (!in.is_open()) {
        cout << "File not found: the file does not exist." << endl;
        return FILE_NOT_FOUND;
    }
    string line;

    int temp_num_users, temp_num_items;
    try { 
        if (!getline(in, line)) {
            throw ILLEGAL_FILE_FORMAT;
        }
        istringstream iss1(line);
        if (!(iss1 >> temp_num_users)) {
            throw ILLEGAL_FILE_FORMAT;
        }
        if (!getline(in, line)) {
            throw ILLEGAL_FILE_FORMAT;
        }
        istringstream iss2(line);
        if (!(iss2 >> temp_num_items)) {
            throw ILLEGAL_FILE_FORMAT;
        }
    }
    catch (...) {
        cout << "Illegal file format: the file does not correspond to the standard format." << endl;
        return ILLEGAL_FILE_FORMAT;
    }

    const int num_users = temp_num_users;
    const int num_items = temp_num_items;

    if (num_users <= 1) {
        cout << "Illegal argument: number of users must be greater than 1." << endl;
        return ILLEGAL_ARGUMENT;
    }
    if (num_items < 1) {
        cout << "Illegal argument: number of items must be greater than 0." << endl;
        return ILLEGAL_ARGUMENT;
    }

    float** utility = new float*[num_users];
    for (int i = 0; i < num_users; i++) {
        utility[i] = new float[num_items];
    }

    try {
        for (int i = 0; i < num_users; i++) {
            if (!getline(in, line)) {
                throw ILLEGAL_FILE_FORMAT;
            }
            istringstream iss(line);
            for (int j = 0; j < num_items; j++) {
                float temp_rating;
                if (iss >> temp_rating) {
                    utility[i][j] = temp_rating;
                } else {
                    throw ILLEGAL_FILE_FORMAT;
                }
            }
        } 
    }
    catch (...) {
        cout << "Illegal file format: the file does not correspond to the standard format." << endl;
        return ILLEGAL_FILE_FORMAT;
    }
    in.close();

    #ifdef DEBUG_1
    cout << "orginal matrix: " << endl;
    for (int i = 0; i < num_users; i++) {
        for (int j = 0; j < num_items; j++) {
            printf("%.2f\t", utility[i][j]);
        }
        cout << endl;
    }
    #endif

    float missing_rating = (float) MISSING_RATING;

    for (int i = 0; i < num_users; i++) {
        float user_item_sum = 0;
        int user_item_count = 0;
        for (int j = 0; j < num_items; j++) {
            if (abs(utility[i][j] - missing_rating) > 1e-3) {
                user_item_sum += utility[i][j];
                user_item_count++;
            }
        }
        float user_average = user_item_sum / user_item_count;
        for (int j = 0; j < num_items; j++) {
            if (abs(utility[i][j] - missing_rating) > 1e-3) {
                utility[i][j] -= user_average;
            }
        }
    }

    #ifdef DEBUG_1
    cout << "normalized matrix: " << endl;
    for (int i = 0; i < num_users; i++) {
        for (int j = 0; j < num_items; j++) {
            printf("%.2f\t", utility[i][j]);
        }
        cout << endl;
    }
    #endif

    float* user_norm = new float[num_users];

    float** similarity = new float*[num_users];
    for (int i = 0; i < num_users; i++) {
        similarity[i] = new float[num_users];
    }

    for (int i = 0; i < num_users; i++) {
        float user_square_sum = 0;
        for (int j = 0; j < num_items; j++) {
            if (abs(utility[i][j] - missing_rating) > 1e-3) {
                user_square_sum += utility[i][j] * utility[i][j];
            }
        }
        user_norm[i] = sqrt(user_square_sum);
    }

    #ifdef DEBUG_1
    cout << "rating norm per user: " << endl;
    for (int i = 0; i < num_users; i++) {
        printf("%.2f\t", user_norm[i]);
        cout << endl;
    }
    #endif

    for (int i = 0; i < num_users; i++) {
        for (int j = 0; j < i; j++) {
            float temp_cosine = 0;
            for (int k = 0; k < num_items; k++) {
                if (abs(utility[i][k] - missing_rating) > 1e-3 && abs(utility[j][k] - missing_rating) > 1e-3) {
                    temp_cosine += utility[i][k] * utility[j][k];
                }
            }
            similarity[i][j] = similarity[j][i] = temp_cosine / (user_norm[i] * user_norm[j]);
        }
        similarity[i][i] = 1;
    }

    #ifdef DEBUG_1
    cout << "similarity matrix: " << endl;
    for (int i = 0; i < num_users; i++) {
        for (int j = 0; j < num_users; j++) {
            printf("%.2f\t", similarity[i][j]);
        }
        cout << endl;
    }
    #endif

    return SUCCESS;
}
