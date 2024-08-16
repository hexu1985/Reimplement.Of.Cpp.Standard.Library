
RM = rm -rf
CXX = g++
CXXFLAGS = -Wall -g -fsanitize=leak -fno-omit-frame-pointer #-DNDEBUG
INCLUDES = -Isrc
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
