.PHONY: all clean test-hlpr


CC := gcc
CFLAGS := -pthread -pedantic -fgnu-tm -static -lm -O3 -g -fopenmp -fgnu-tm
SRC_FILES := hlpr-maxflow.c

all:
	${CC} ${CFLAGS} hlpr-maxflow.c -c -o hlpr-maxflow.o
	${CC} ${CFLAGS} main.c hlpr-maxflow.o -o hlpr-maxflow.out

clean:
	rm *.o
	rm hlpr-maxflow.out

test-hlpr:
	./check-ans.sh ./hlpr-maxflow.out
