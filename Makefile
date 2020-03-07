CXX       := g++-9
CXXFLAGS  := --std=c++11 -O2 -Wall
LIBS      := -ledit

SRC_DIR   := src
TARGET    := lisp.out



all:
	$(CXX) $(CXXFLAGS) $(LIBS) -o $(SRC_DIR)/$(TARGET) \
	$(SRC_DIR)/main.cpp $(SRC_DIR)/lispvalue.cpp $(SRC_DIR)/parsing.cpp $(SRC_DIR)/evaluation.cpp $(SRC_DIR)/builtin.cpp $(SRC_DIR)//lib/mpc.cpp

.PHONY: all