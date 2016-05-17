all: writeup

writeup:
	latex hw3.tex
	bibtex hw3
	latex hw3.tex
	latex hw3.tex
	dvips -R -Poutline -t letter hw3.dvi -o hw3.ps
	ps2pdf hw3.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
