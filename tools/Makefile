CC=g++

SOURCES=$(TARGET).cpp 

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=$(TARGET)

CFLAGS := -Wall -g -O3 -Wfatal-errors -mssse3 -msse2 -march=native
#CFLAGS := -g -O3 -mssse3 -msse2 -march=native
LDFLAGS :=  grief/grief.cpp -Llib -Wl,-rpath='$$ORIGIN/lib' -g `pkg-config opencv --libs` 

# OpenCV
#CFLAGS += `pkg-config opencv --cflags`

match_all: match_all.cpp
	$(CC) match_all.cpp $(CFLAGS) $(LDFLAGS) -o $@ 

evolve_grief: evolve_grief.cpp
	$(CC) evolve_grief.cpp $(FLAGS) $(LDFLAGS) -o $@
 
annotate: match_all.cpp
	$(CC) annotate.cpp $(FLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) 

cleanall:
	rm -f *.o 
