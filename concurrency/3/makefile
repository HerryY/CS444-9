all: concurrency

concurrency: concurrency3.c
	gcc concurrency3.c -o concurrency3 -std=c99 -g -lpthread

clean:
	rm -f concurrency3 *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
