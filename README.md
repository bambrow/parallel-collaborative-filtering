# Parallel Collaborative Filtering
This project presents a parallel collaborative filtering algorithm using OpenMP.

## Project Contributors
- Weiqiang Li
- Dongxu Lin

## Overview
Collaborative filtering is one of the most important algorithms for recommendation systems. The algorithm predicts the preferences from users to every item based on the similarities with other users. Collaborative filtering is computationally intensive as the dataset will be very large in practice. In this report, an efficient algorithm to parallelize collaborative filtering using OpenMP was proposed and implemented.

## How to Compile & Run
To compile the program, simply run:
```
make
```
In certain cases, specifying `g++` version is needed:
```
make CXX=g++-8
```
Also, to clean the compiled `*.o` files and the executable, simply run:
```
make clean
```
Then to run the program, use:
```
./cf <filename> <num_threads> <extra_options>
```
Where the number of threads can be provided in:
```
<num_threads> (default is 5) will be ignored when -s option (serial) is chosen
```
Moreover, the extra options are:
```
-s: this option turns on serial algorithm
-p: this option (default) turns on parallel algorithm
-i: this option turns on item-item similarity
-u: this option (default) turns on user-user similarity
```
Since test files are provided, we suggest the following running examples:
```
./cf tests/test2.txt -s
./cf tests/test2.txt -p 10
./cf tests/test2.txt -s -i
./cf tests/test2.txt -p 10 -i
```
This program will output the filled utility matrix based on the input file given, to a `*.txt` file under the same folder as the input file.

## Test Files
Descriptions of the test files:
```
tests/test1.txt: 10*10 user-item utility matrix, half-filled
tests/test2.txt: 1000*100 user-item utility matrix, half-filled
tests/test3.txt: 1000*500 user-item utility matrix, half-filled
```
All the files follow the following format:
- The first line contains one number: the number of users
- The second line contains one number: the number of items
- The rest are the utility matrix, with effective ratings from 1 to 10, and missing ratings as 99.

To generate more test files, a script is also given:
```
python tests/gen_test.py <num_users> <num_items>
```

Note that this script can only generate half-filled utility matrix, with effective ratings from 1 to 10, and missing ratings as 99.

