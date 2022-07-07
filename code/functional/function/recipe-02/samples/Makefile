
RM = rm -rf
CXX = g++
CXXFLAGS = -Wall -g -std=c++17 -fsanitize=address -fno-omit-frame-pointer #-DNDEBUG
INCLUDES = -I../include
LDFLAGS = 
LDPATH =

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
