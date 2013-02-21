SRC      = $(wildcard *.cpp)
TESTS    = $(SRC:.cpp=_test)
CPPFLAGS = -Wall
CXXFLAGS = -isystem $(BOOST_HOME) -I../src -std=c++0x -fcolor-diagnostics -g -O3 -Wno-unknown-pragmas
LDFLAGS  = -L../src -Wl,-rpath,../src
LDLIBS   = -lbypass

.PHONY: all clean

all: check

%_test: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(addsuffix _tests,$(subst .,_,$<)) $< $(LDFLAGS) $(LDLIBS)

test: $(TESTS)

check: test
	for test in *_tests; do echo "• $${test}" && "./$${test}"; done

clean:
	rm -f *_tests
	rm -rf *.dSYM