SRC      = $(wildcard *.cpp)
OBJ		 = $(SRC:.cpp=.o)
TESTS    = $(SRC:.cpp=_tests)
CPPFLAGS = -Wall
CXXFLAGS = -isystem $(BOOST_HOME) -I../src -std=c++0x -g -O3 -Wno-unknown-pragmas
LDFLAGS  = -L../src -Wl,-rpath,../src
LDLIBS   = -lbypass

.PHONY: all clean

all: check

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%_tests: %.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(addsuffix _tests,$(subst .,_,$<)) $< $(LDFLAGS) $(LDLIBS)

test: $(TESTS)

check: test
	for test in *_tests; do echo "• $${test}" && "./$${test}"; done

clean:
	rm -f *_tests
	rm -rf *.dSYM