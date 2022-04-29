
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++14
INCLUDES = 
LDFLAGS = -lboost_thread -lboost_system -pthread
LDPATH =

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS) *.o

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)

$(PROGS): %: %.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
