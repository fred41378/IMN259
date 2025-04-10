.SUFFIXES:
.SUFFIXES: .o .cpp

CCC        = g++
CCFLAGS    = -g -std=c++20
LIBS       = -lm
EXECUTABLE = all
OBJETS     = MImage.o

all:tp4A tp4B tp4C

tp4A: tp4A.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp4B: tp4B.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp4C: tp4C.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

.cpp.o:
	$(CCC) $(CCFLAGS) -c $< -o $@ 

clean:
	rm tp4A tp4B tp4C *~ *.o *.exe
