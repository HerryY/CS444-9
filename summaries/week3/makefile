all: writeup

writeup:
	latex week3.tex
	dvips -R -Poutline -t letter week3.dvi -o week3.ps
	ps2pdf week3.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
