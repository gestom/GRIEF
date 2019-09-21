CC=g++

SOURCES=$(TARGET).cpp 

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=$(TARGET)

#CFLAGS := -Wall -g -O3 -Wfatal-errors -mssse3 -msse2 -march=native
CFLAGS := -ggdb 
LDFLAGS :=  grief/grief.cpp -Llib -Wl,-rpath='$$ORIGIN/lib' -g `pkg-config opencv --libs` 

# OpenCV
#CFLAGS += `pkg-config opencv --cflags`

all: match_all t-test

match_all: match_all.cpp grief/grief.cpp 
	$(CC) match_all.cpp $(CFLAGS) $(LDFLAGS) -o $@ 

t-test: t-test.cpp 
	$(CC) t-test.cpp -lalglib $(CFLAGS) -o $@ 

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) 
	rm -f match_all t-test

cleanall:
	rm -f *.o 
