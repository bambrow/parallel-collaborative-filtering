CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -fopenmp -O2
INCLUDES = -I./external/eigen
BUILD_DIR = ./build
EXTERNAL_DIR = ./external
SRC_DIR = ./src
OBJ = $(BUILD_DIR)/scf \
      $(BUILD_DIR)/eigen_test

all: $(BUILD_DIR) $(EXTERNAL_DIR)/eigen $(OBJ)

$(EXTERNAL_DIR)/eigen:
	unzip -qq -o $(EXTERNAL_DIR)/eigen.zip -d $(EXTERNAL_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/scf: $(SRC_DIR)/scf.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} $(SRC_DIR)/scf.cpp -o $(BUILD_DIR)/scf

$(BUILD_DIR)/eigen_test: $(SRC_DIR)/eigen_test.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} $(SRC_DIR)/eigen_test.cpp -o $(BUILD_DIR)/eigen_test

.PHONY: clean
clean:
	rm -rf $(SRC_DIR)/*.o $(EXTERNAL_DIR)/eigen $(BUILD_DIR)