.SUFFIXES:
.SUFFIXES: .o .cpp

CCC        = g++
CCFLAGS    = -g -std=c++11
LIBS       = -lm
EXECUTABLE = all
OBJETS     = MImage.o

all:tp2A tp2B tp2C

tp2A: tp2A.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp2B: tp2B.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp2C: tp2C.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

.cpp.o:
	$(CCC) $(CCFLAGS) -c $< -o $@ 

clean:
	rm tp2A tp2B tp2C *~ *.o *.exe
