TEX	= latex -shell-escape
BIBTEX	= bibtex
DVIPS	= dvips
DVIPDF  = dvipdft
XDVI	= xdvi -gamma 4
GH		= gv

EXAMPLES = $(wildcard *.h)
SRC	:= $(shell egrep -l '^[^%]*\\begin\{document\}' *.tex)
TRG	= $(SRC:%.tex=%.dvi)
PSF	= $(SRC:%.tex=%.ps)
PDF	= $(SRC:%.tex=%.pdf)

all: writeup concurrency

concurrency: assignment1.c
	gcc assignment1.c -o assignment1 -std=c99 -g -lpthread

writeup:
	latex hw1.tex
	bibtex hw1
	latex hw1.tex
	latex hw1.tex
	dvips -R -Poutline -t letter hw1.dvi -o hw1.ps
	ps2pdf hw1.ps

clean:
	rm -f assignment1 *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
