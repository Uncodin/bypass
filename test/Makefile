SRC      = $(wildcard *.cpp)
TESTS    = $(SRC:.cpp=_test)
CPPFLAGS = -Wall
CXXFLAGS = -isystem $(BOOST_HOME)
LDFLAGS  = -L../src -Wl,-rpath,../src
LDLIBS   = -lbypass

.PHONY: all clean

all: check

%_test: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(addsuffix _tests,$(subst .,_,$<)) $< $(LDFLAGS) $(LDLIBS)

test: $(TESTS)

check: test
	for test in *_tests; do "./$${test}"; done

clean:
	rm -f *_tests
	rm -f *.o
	rm -f *.so
	rm -rf *.dSYM