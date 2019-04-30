# Parallel Collaborative Filtering
This project presents a parallel collaborative filtering algorithm using OpenMP.

## How to Compile & Run
To compile the program, simply run:
```
make
```
In certain cases, specifying g++ version is needed:
```
make CXX=g++-8
```
Then to run the program, use:
```
./cf <filename> <extra_options>
```
Where the extra options are:
```
-s: this option turns on serial algorithm
-p: this option (default) turns on parallel algorithm
-i: this option turns on item-item similarity
-u: this option (default) turns on user-user similarity
```