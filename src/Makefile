
RM = rm -f
AR = ar rcs
CXX = g++
CXXFLAGS = -Wall -g -DNDEBUG -std=c++11
INCLUDES = -I../include -I../include/shared_ptr
LDFLAGS = 
LDPATH =
VPATH = 

SOURCES = $(shell ls */*.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
STATICLIB = libmini_stl.a

all: $(STATICLIB)
#	@bash ./gen_mini_utils_header.sh
	@echo "STATICLIB = $(STATICLIB)" 

.PHONY: clean
clean:
	$(RM) $(STATICLIB) $(OBJECTS)

$(OBJECTS): %.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INCLUDES) $<

$(STATICLIB): $(OBJECTS)
	$(AR) $(STATICLIB) $(OBJECTS)

