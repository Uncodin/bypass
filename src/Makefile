SRC      = $(wildcard *.cpp)
OBJ      = $(SRC:.cpp=.o)
CXXFLAGS = -I../dep/libsoldout -std=c++0x -g -O3 -fPIC
CPPFLAGS = -Wall

.PHONY: all clean

all: $(OBJ)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o
	rm -f *.a