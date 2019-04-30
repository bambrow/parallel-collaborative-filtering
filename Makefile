CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -fopenmp -O2
INCLUDES = -I./external/eigen
EXTERNAL_DIR = ./external
SRC = ./src/cf.h \
	./src/scf.h \
	./src/pcf.h
OBJ = ./src/cf.o \
	./src/scf.o \
	./src/pcf.o

default: all
# all: $(EXTERNAL_DIR)/eigen cf
all: cf

$(EXTERNAL_DIR)/eigen:
	unzip -qq -o $(EXTERNAL_DIR)/eigen.zip -d $(EXTERNAL_DIR)

%.o: %.cpp $(SRC)
	$(CXX) $(CXXFLAGS) ${INCLUDES} -c -o $@ $< 

cf: $(OBJ)
	${CXX} ${CXXFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -rf cf ./src/*.o $(EXTERNAL_DIR)/eigen