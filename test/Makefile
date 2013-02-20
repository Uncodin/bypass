SRC      = $(wildcard *.cpp)
OBJ      = $(SRC:.c=.o)
CPPFLAGS = -Wall
CXXFLAGS = -isystem $(BOOST_HOME)


.PHONY: all clean

all: test

test: $(SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(addsuffix _tests,$(subst .,_,$<)) $<

check: test
	./$^

clean:
	rm -f *_tests
	rm -f *.o
	rm -f *.so
	rm -rf *.dSYM