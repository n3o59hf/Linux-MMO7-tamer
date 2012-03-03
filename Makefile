# Generic Makefile for simple projects that lack one
# Mārtiņš Kriķis (Martins Krikis)

TARGET := mouse

CFLAGS = -g -O2
LDFLAGS =
LIBS = -lusb -lXtst -lX11

CC := gcc
CXX := g++

src_c := $(wildcard *.c)
src_cc := $(wildcard *.cc)
src_cpp := $(wildcard *.cpp)
src_cxx := $(wildcard *.cxx)

OBJ := $(src_c:%.c=%.o) \
       $(src_cc:%.cc=%.o) \
       $(src_cpp:%.cpp=%.o) \
       $(src_cxx:%.cxx=%.o)

.PHONY:	all clean

all:	$(TARGET)

clean:
	$(RM) $(TARGET) *.o

link_line = $(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(TARGET):	$(OBJ)
	$(link_line)

cc_line = $(CC) $(CFLAGS) -o $@ -c $<
cxx_line = $(CXX) $(CFLAGS) -o $@ -c $<

allhfiles := $(wildcard *.h)

%.o:	%.c $(allhfiles)
	$(cc_line)

%.o:	%.cc $(allhfiles)
	$(cxx_line)

%.o:	%.cpp $(allhfiles)
	$(cxx_line)

%.o:	%.cxx $(allhfiles)
	$(cxx_line)

