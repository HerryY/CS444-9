all: smokers

smokers: smokers.c
	gcc smokers.c -o smoker -std=c99 -g -lpthread

clean:
	rm -f smoker *.o

.PHONY: all show clean ps pdf showps
