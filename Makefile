CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -fopenmp -O2
SRC = ./src/cf.h \
	./src/scf.h \
	./src/pcf.h
OBJ = ./src/cf.o \
	./src/scf.o \
	./src/pcf.o

default: all
all: cf

%.o: %.cpp $(SRC)
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

cf: $(OBJ)
	${CXX} ${CXXFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -rf cf ./src/*.o