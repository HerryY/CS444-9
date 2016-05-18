all: barber share

barber: barber.c
	gcc barber.c -o barber -std=c99 -g -lpthread

share: share.c
	gcc share.c -o share -std=c99 -g -lpthread

clean:
	rm -f share barber *.o

.PHONY: all show clean ps pdf showps
