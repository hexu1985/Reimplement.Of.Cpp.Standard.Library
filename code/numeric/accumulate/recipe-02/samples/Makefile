
RM = rm -rf
CXX = g++
CXXFLAGS = -Wall -g -std=c++14 #-DNDEBUG
INCLUDES = -I../include
LDFLAGS = # -lpthread
LDPATH =

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS) *.dSYM

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
