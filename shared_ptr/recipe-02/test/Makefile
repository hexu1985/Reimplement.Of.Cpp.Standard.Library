
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g #-std=c++11
INCLUDES = -I$(HOME)/local/include -I../include
LDFLAGS = -lgtest_main -lgtest -lpthread
LDPATH = -L$(HOME)/local/lib 
VPATH = 

LIB_SRC = $(shell ls ../src/*.cpp)
SOURCES = $(shell ls *.cpp)
PROGS = shared_ptr_test

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

$(PROGS): $(SOURCES) $(LIB_SRC)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
