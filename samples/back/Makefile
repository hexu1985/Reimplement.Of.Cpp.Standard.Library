
RM = rm -rf
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 #-DNDEBUG
INCLUDES = -I../../include/shared_ptr
LDFLAGS = -lmini_stl
LDPATH = -L../../src

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS) *.dSYM

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
