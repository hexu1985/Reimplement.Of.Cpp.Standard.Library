
RM = rm -rf
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -fsanitize=leak -fno-omit-frame-pointer #-DNDEBUG
INCLUDES =
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
