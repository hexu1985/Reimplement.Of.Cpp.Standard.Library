
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g #-std=c++11
INCLUDES = -I$(HOME)/local/include -I../include/shared_ptr
LDFLAGS = -lmini_stl -lgtest -lpthread
LDPATH = -L$(HOME)/local/lib -L../src
VPATH = 

SOURCES = $(shell ls */*.cpp)
PROGS = mini_stl_test

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

$(PROGS): main.cpp $(SOURCES) $(LIB_SOURCES)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
