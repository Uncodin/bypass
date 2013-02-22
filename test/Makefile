SUTOBJ   = $(wildcard ../src/*.o)
SRC      = $(wildcard *.cpp)
OBJ		 = $(SRC:.cpp=.o)
TESTS    = $(SRC:.cpp=.tst)
CPPFLAGS = -Wall
CXXFLAGS = -isystem $(BOOST_HOME) -I../src -std=c++0x -g -O3 -Wno-unknown-pragmas

.PHONY: all clean check test
.SECONDARY:

all: check

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.tst: %.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(addsuffix .tst,$(basename $<)) $(SUTOBJ) $^

test: $(TESTS)

check: test
	for test in *.tst; do echo "• $${test}" && "./$${test}"; done

clean:
	rm -f *.o
	rm -f *.tst
	rm -rf *.dSYM