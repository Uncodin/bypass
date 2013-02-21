LIBSRC   = $(wildcard *.cpp)
LIBOBJ   = $(LIBSRC:.cpp=.o)
CXXFLAGS = -I../dep/libsoldout -std=c++0x -g -O3 -Wno-unknown-pragmas -fPIC
CPPFLAGS = -Wall
LDLIBS   = -lc
LDFLAGS  = -shared -Wl -install_name

.PHONY: all clean

all: libbypass.so

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

libbypass.so: $(LIBOBJ)
	$(CXX) $(LDFLAGS) -o libbypass.so $(LIBOBJ) $(LDLIBS)

clean:
	rm -f *.o
	rm -f *.so
	rm -rf *.dSYM
