# Makefile

AR = ar rv
RM = rm -f
MV = mv
CXX = g++

ifdef slience
QUIET = @
endif

std = c++11

CXXFLAGS = -Wall -g -DNDEBUG -std=$(std)
INCLUDE += -I../include
LDFLAGS = 
LDPATH = 

OBJS = thread.o
LIB = libthread.a

.PHONY: all
all: $(LIB)
	@echo "build OK!"

.PHONY: clean
clean:
	$(QUIET) $(RM) $(OBJS) $(LIB) $(PROGS) *.o *.exe
	@echo "clean OK!"

$(OBJS):%.o:%.cpp
	$(QUIET) $(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

$(LIB): $(OBJS)
	$(QUIET) $(AR) $@ $?
