CC=g++ -g -Wall

# List of source files for your thread library
THREAD_SOURCES=thread.cpp threadImpl.cpp globals.cpp cpu.cpp cpuImpl.cpp mutex.cpp mutexImpl.cpp cv.cpp cvImpl.cpp

# Generate the names of the thread library's object files
THREAD_OBJS=${THREAD_SOURCES:.cpp=.o}
tests = test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 test13 test14 test15 test16 test17 test18 test21 test22 test_runtime1 test_runtime2 test_runtime3
all: libthread.o app $(tests)

# Compile the thread library and tag this compilation
libthread.o: ${THREAD_OBJS}
	./autotag.sh
	ld -r -o $@ ${THREAD_OBJS}

# Compile an application program
$(tests):
 %: %.cpp libthread.o libcpu.o
		${CC} -o $@ $^ -ldl -pthread
app: test1.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${THREAD_OBJS} libthread.o app $(tests)
