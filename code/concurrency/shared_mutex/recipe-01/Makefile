
RM = rm -rf
CXX = g++
CXXFLAGS = -Wall -g #-std=c++17 #-DNDEBUG
INCLUDES = -Isrc
LDFLAGS = -lpthread
LDPATH =

LIB_SRC = $(shell ls src/*.cpp src/*.hpp)
SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

%: %.cpp $(LIB_SRC)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
