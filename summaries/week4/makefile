all: writeup

writeup:
	latex week4.tex
	dvips -R -Poutline -t letter week4.dvi -o week4.ps
	ps2pdf week4.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
