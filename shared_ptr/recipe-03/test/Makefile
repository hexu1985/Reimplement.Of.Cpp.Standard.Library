
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g #-std=c++11
INCLUDES = -I$(HOME)/local/include -I../include
LDFLAGS = -lgtest_main -lgtest -lpthread
LDPATH = -L$(HOME)/local/lib 
VPATH = 

LIB_SRC = $(shell ls ../src/*.cpp)
LIB_OBJ = $(subst .cpp,.o,$(LIB_SRC))
SOURCES = $(shell ls *.cpp)
OBJECTS = $(subst .cpp,.o,$(SOURCES))
PROGS = shared_ptr_test

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS) $(LIB_OBJ) $(OBJECTS)

$(LIB_OBJ): %.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INCLUDES) $<

$(OBJECTS): %.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INCLUDES) $<

$(PROGS): $(OBJECTS) $(LIB_OBJ)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
