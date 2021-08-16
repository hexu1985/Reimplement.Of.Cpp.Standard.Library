
RM = rm -rf
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 #-DNDEBUG
INCLUDES = -I../include
LDFLAGS =
LDPATH =

LIB_SRC = $(shell ls ../src/*.cpp)
SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS) *.dSYM

%: %.cpp $(LIB_SRC)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
