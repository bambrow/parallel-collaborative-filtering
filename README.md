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
./cf <filename> <num_threads> <extra_options>
```
Where the number of threads can be provided in:
```
<num_threads> (default is 5) will be ignored when -s option is chosen
```
Moreover, the extra options are:
```
-s: this option turns on serial algorithm
-p: this option (default) turns on parallel algorithm
-i: this option turns on item-item similarity
-u: this option (default) turns on user-user similarity
```