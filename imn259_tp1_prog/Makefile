.SUFFIXES:
.SUFFIXES: .o .cpp

CCC        = g++
CCFLAGS    = -g -std=c++11
LIBS       = -lm
EXECUTABLE = all
OBJETS     = MImage.o

all:tp1

tp1: tp1.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)
	
.cpp.o:
	$(CCC) $(CCFLAGS) -c $< -o $@ 

clean:
	rm tp1 *~ *.o *.exe gc/*o
