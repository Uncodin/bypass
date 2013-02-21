LIBSRC   = $(wildcard *.cpp)
LIBOBJ   = $(LIBSRC:.cpp=.o)
CXXFLAGS = -I../dep/libsoldout -std=c++0x -g -O3 -Wno-unknown-pragmas -fPIC
CPPFLAGS = -Wall

.PHONY: all clean

all: libbypass.a

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

libbypass.a: $(LIBOBJ)
	libtool -o libbypass.a -static $(LIBOBJ)

clean:
	rm -f *.o
	rm -f *.a
	rm -rf *.dSYM
