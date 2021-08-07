INCLUDE= \
-Iinclude

CFLAGS?=-O2 -g $(INCLUDE) -Werror
CXXFLAGS?=-std=c++11

OBJS = \
src/alsa.o \
src/main.o \
src/WavHeader.o \
src/Delay.o \
src/LFO.o \
src/Effects/DelayEffect.o \
src/Effects/ReverseEffect.o \
src/Effects/EchoEffect.o \
src/Effects/VibratoEffect.o \
src/Effects/MoveSourceEffect.o

.PHONY: all clean
.SUFFIXES: .o .c .cpp .S
.SILENT:

all: clean main

main: $(OBJS)
	$(CXX) -o $@ $(CFLAGS) $(CXXFLAGS) -lasound $(OBJS)

.cpp.o:
	$(CXX) -MD -c $< -o $@ $(CFLAGS) $(CXXFLAGS)

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CXXFLAGS)

clean:
	rm -f main
	rm -f $(OBJS)
	rm -f $(OBJS:.o=.d)
