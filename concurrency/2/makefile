all: concurrency

concurrency: concurrency2.c
	gcc concurrency2.c -o concurrency2 -std=c99 -g -lpthread

clean:
	rm -f concurrency2 *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
