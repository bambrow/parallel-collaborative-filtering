
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <queue>
#include <math.h>
#include <omp.h>

#include "cf.h"
#include "pcf.h"

using namespace std;

void pcf_user
(
    const int& num_users, 
    const int& num_items, 
    float** normalized_utility, 
    float* user_average, 
    float** filled_utility,
    int& num_threads
);

void pcf_item
(
    const int& num_users, 
    const int& num_items, 
    float** normalized_utility, 
    float* user_average, 
    float** filled_utility,
    int& num_threads
);

return_code pcf
(
    string filename,
    int num_threads, 
    bool item_option
) 
{
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
        cout << "Illegal file format: number of users must be greater than 1." << endl;
        return ILLEGAL_FILE_FORMAT;
    }
    if (num_items < 1) {
        cout << "Illegal file format: number of items must be greater than 0." << endl;
        return ILLEGAL_FILE_FORMAT;
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

    float* user_average = new float[num_users];

    float missing_rating = (float) MISSING_RATING;

    float** normalized_utility = new float*[num_users];
    for (int i = 0; i < num_users; i++) {
        normalized_utility[i] = new float[num_items];
    }

    #pragma omp parallel for num_threads(num_threads) \
        default(none) \
        firstprivate(utility, num_users, num_items, missing_rating) \
        shared(user_average) \
        schedule(static,1)
    for (int i = 0; i < num_users; i++) {
        float user_item_sum = 0;
        int user_item_count = 0;
        for (int j = 0; j < num_items; j++) {
            if (abs(utility[i][j] - missing_rating) > 1e-3) {
                user_item_sum += utility[i][j];
                user_item_count++;
            }
        }
        float user_avg = user_item_sum / user_item_count;
        user_average[i] = user_avg;
    }

    #pragma omp parallel num_threads(num_threads) \
        default(none) \
        firstprivate(num_users, num_items, utility, missing_rating, user_average) \
        shared(normalized_utility)
    for (int i = 0; i < num_users; i++) {

        #pragma omp for schedule(static,1)
        for (int j = 0; j < num_items; j++) {
            if (abs(utility[i][j] - missing_rating) > 1e-3) {
                normalized_utility[i][j] = utility[i][j] - user_average[i];
            } else {
                normalized_utility[i][j] = utility[i][j];
            }
        }
    }


    #ifdef DEBUG_1
    cout << "user average: " << endl;
    for (int i = 0; i < num_users; i++) {
        printf("%.2f\t", user_average[i]);
        cout << endl;
    }
    cout << "normalized matrix: " << endl;
    for (int i = 0; i < num_users; i++) {
        for (int j = 0; j < num_items; j++) {
            printf("%.2f\t", normalized_utility[i][j]);
        }
        cout << endl;
    }
    #endif

    float** filled_utility = new float*[num_users];
    for (int i = 0; i < num_users; i++) {
        filled_utility[i] = new float[num_items];
    }

    if (!item_option) {
        pcf_user(num_users, num_items, normalized_utility, user_average, filled_utility, num_threads);
    } else {
        pcf_item(num_users, num_items, normalized_utility, user_average, filled_utility, num_threads);
    }

    #ifdef DEBUG_1
        cout << "filled normalized matrix: " << endl;
        for (int i = 0; i < num_users; i++) {
            for (int j = 0; j < num_items; j++) {
                printf("%.2f\t", filled_utility[i][j]);
            }
            cout << endl;
        }
    #endif

    #pragma omp parallel num_threads(num_threads) \
        default(none) \
        firstprivate(num_users, num_items, user_average) \
        shared(filled_utility)
    for (int i = 0; i < num_users; i++) {

        #pragma omp for schedule(static,1)
        for (int j = 0; j < num_items; j++) {
            filled_utility[i][j] += user_average[i];
        }
    }

    #ifdef DEBUG_1
        cout << "filled original matrix: " << endl;
        for (int i = 0; i < num_users; i++) {
            for (int j = 0; j < num_items; j++) {
                printf("%.2f\t", filled_utility[i][j]);
            }
            cout << endl;
        }
    #endif

    string trunc_filename = filename.substr(0, filename.find_last_of("."));
    string output_filename = trunc_filename + "_output.txt";
    ofstream out(output_filename);
    if (out.is_open()) {
        out << num_users << endl;
        out << num_items << endl;
        for (int i = 0; i < num_users; i++) {
            for (int j = 0; j < num_items; j++) {
                out << fixed << setprecision(2) << filled_utility[i][j] << " ";
            }
            out << endl;
        }
    }

    return SUCCESS;
}

void pcf_user
(
    const int& num_users, 
    const int& num_items, 
    float** normalized_utility, 
    float* user_average, 
    float** filled_utility,
    int& num_threads
) 
{
    float missing_rating = (float) MISSING_RATING;

    float* user_norm = new float[num_users];

    float** similarity = new float*[num_users];
    for (int i = 0; i < num_users; i++) {
        similarity[i] = new float[num_users];
    }

    #pragma omp parallel for num_threads(num_threads) \
        default(none) \
        firstprivate(normalized_utility, num_users, num_items, missing_rating) \
        shared(user_norm) \
        schedule(static,1)
    for (int i = 0; i < num_users; i++) {
        float user_square_sum = 0;
        for (int j = 0; j < num_items; j++) {
            if (abs(normalized_utility[i][j] - missing_rating) > 1e-3) {
                user_square_sum += normalized_utility[i][j] * normalized_utility[i][j];
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

    #pragma omp parallel num_threads(num_threads) \
        default(none) \
        firstprivate(normalized_utility, user_norm, num_users, num_items, missing_rating) \
        shared(similarity)
    for (int i = 0; i < num_users; i++) {

        #pragma omp for schedule(static,1)
        for (int j = 0; j < i; j++) {
            float temp_cosine = 0;
            for (int k = 0; k < num_items; k++) {
                if (abs(normalized_utility[i][k] - missing_rating) > 1e-3 && abs(normalized_utility[j][k] - missing_rating) > 1e-3) {
                    temp_cosine += normalized_utility[i][k] * normalized_utility[j][k];
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

//    to fill the missing value in the matrix
    unsigned num_most_similar = NUM_MOST_SIMILAR;

    #pragma omp parallel num_threads(num_threads) \
        default(none) \
        firstprivate(normalized_utility, similarity, num_users, num_items, missing_rating, num_most_similar, user_average) \
        shared(filled_utility)
    for (int i = 0; i < num_users; i++) {

        #pragma omp for schedule(static,1)
        for (int k = 0; k < num_items; k++) {

            float user_possible_max = (float) MAX_RATING;
            user_possible_max -= user_average[i];
            float user_possible_min = (float) MIN_RATING;
            user_possible_min -= user_average[i];

            if (abs(normalized_utility[i][k] - missing_rating) < 1e-3) {
                priority_queue<SimTuple, vector<SimTuple>, CompareSimTuple> pq;

                for (int l = 0; l < num_users; l++) {
                    if (l == i) {
                        continue;
                    }
                    if (abs(normalized_utility[l][k] - missing_rating) > 1e-3) {

                        pq.push(SimTuple(l, similarity[i][l]));

                        if (pq.size() == num_most_similar + 1) {
                            pq.pop();
                        }
                    }
                }

                float denominator = 0;
                float numerator = 0;

                while (!pq.empty()) {
                    int user = pq.top().id;
                    pq.pop();

                    denominator += abs(similarity[i][user]);
                    numerator += normalized_utility[user][k] * similarity[i][user];
                }
                if (denominator != 0){
                    filled_utility[i][k] = numerator/denominator;
                    filled_utility[i][k] = min(user_possible_max, filled_utility[i][k]);
                    filled_utility[i][k] = max(user_possible_min, filled_utility[i][k]);
                }

            } else {
                filled_utility[i][k] = normalized_utility[i][k];
            }

        }
    }
}

void pcf_item
(
    const int& num_users, 
    const int& num_items, 
    float** normalized_utility, 
    float* user_average, 
    float** filled_utility,
    int& num_threads
) 
{
    float missing_rating = (float) MISSING_RATING;

    float* item_norm = new float[num_items];

    float** similarity = new float*[num_items];
    for (int i = 0; i < num_items; i++) {
        similarity[i] = new float[num_items];
    }

    #pragma omp parallel for num_threads(num_threads) \
        default(none) \
        firstprivate(normalized_utility, num_users, num_items, missing_rating) \
        shared(item_norm) \
        schedule(static,1)
    for (int i = 0; i < num_items; i++) {
        float item_square_sum = 0;
        for (int j = 0; j < num_users; j++) {
            if (abs(normalized_utility[j][i] - missing_rating) > 1e-3) {
                item_square_sum += normalized_utility[j][i] * normalized_utility[j][i];
            }
        }
        item_norm[i] = sqrt(item_square_sum);
    }

    #ifdef DEBUG_1
    cout << "rating norm per item: " << endl;
    for (int i = 0; i < num_items; i++) {
        printf("%.2f\t", item_norm[i]);
        cout << endl;
    }
    #endif

    #pragma omp parallel num_threads(num_threads) \
        default(none) \
        firstprivate(normalized_utility, item_norm, num_users, num_items, missing_rating) \
        shared(similarity)
    for (int i = 0; i < num_items; i++) {

        #pragma omp for schedule(static,1)
        for (int j = 0; j < i; j++) {
            float temp_cosine = 0;
            for (int k = 0; k < num_users; k++) {
                if (abs(normalized_utility[k][i] - missing_rating) > 1e-3 && abs(normalized_utility[k][j] - missing_rating) > 1e-3) {
                    temp_cosine += normalized_utility[k][i] * normalized_utility[k][j];
                }
            }
            similarity[i][j] = similarity[j][i] = temp_cosine / (item_norm[i] * item_norm[j]);
        }
        similarity[i][i] = 1;
    }

    #ifdef DEBUG_1
    cout << "similarity matrix: " << endl;
    for (int i = 0; i < num_items; i++) {
        for (int j = 0; j < num_items; j++) {
            printf("%.2f\t", similarity[i][j]);
        }
        cout << endl;
    }
    #endif

//    to fill the missing value in the matrix
    unsigned num_most_similar = NUM_MOST_SIMILAR;

    #pragma omp parallel num_threads(num_threads) \
        default(none) \
        firstprivate(normalized_utility, similarity, num_users, num_items, missing_rating, num_most_similar, user_average) \
        shared(filled_utility)
    for (int i = 0; i < num_items; i++) {

        #pragma omp for schedule(static,1)
        for (int k = 0; k < num_users; k++) {

            float user_possible_max = (float) MAX_RATING;
            user_possible_max -= user_average[k];
            float user_possible_min = (float) MIN_RATING;
            user_possible_min -= user_average[k];

            if (abs(normalized_utility[k][i] - missing_rating) < 1e-3){
                priority_queue<SimTuple, vector<SimTuple>, CompareSimTuple> pq;

                for (int l = 0; l < num_items; l++) {
                    if (l == i) {
                        continue;
                    }
                    if (abs(normalized_utility[k][l] - missing_rating) > 1e-3) {

                        pq.push(SimTuple(l, similarity[i][l]));

                        if (pq.size() == num_most_similar + 1) {
                            pq.pop();
                        }
                    }
                }

                float denominator = 0;
                float numerator = 0;

                while (!pq.empty()) {
                    int item = pq.top().id;
                    pq.pop();

                    denominator += abs(similarity[i][item]);
                    numerator += normalized_utility[k][item] * similarity[i][item];
                }
                if (denominator != 0) {
                    filled_utility[k][i] = numerator/denominator;
                    filled_utility[k][i] = min(user_possible_max, filled_utility[k][i]);
                    filled_utility[k][i] = max(user_possible_min, filled_utility[k][i]);
                }

            } else {
                filled_utility[k][i] = normalized_utility[k][i];
            }

        }
    }
}
