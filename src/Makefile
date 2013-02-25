SRC      = $(wildcard *.cpp)
OBJ      = $(SRC:.cpp=.o)
CPPFLAGS = -Wall
CXXFLAGS = -I../dep/libsoldout -g -O3 -fPIC

.PHONY: all clean

all: $(OBJ)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o
	rm -f *.a