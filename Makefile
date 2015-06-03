# Makefile for process.exe and testing library code.

CC = g++
WARNS = -Wall -Wno-sign-compare
CFLAGS = -O3 -std=c++11 ${WARNS}

PROCESS_ARGS = includes.txt library.txt
LIBRARY = algorithms.h macros.h math.h mathtypes.h matrix.h misc.h parsing.h


all: process.exe library_tests.exe

test: library_tests.exe
	./library_tests.exe

process.exe: process.cc
	$(CC) -o $@ $< $(CFLAGS)

processed_tests.cc: library_tests.cc process.exe $(PROCESS_ARGS) $(LIBRARY)
	./process.exe $(PROCESS_ARGS) <$< >$@

library_tests.exe: processed_tests.cc
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f "process.exe" "library_tests.exe" "processed_tests.cc"
