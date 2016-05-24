all: writeup

writeup:
	latex week7.tex
	dvips -R -Poutline -t letter week7.dvi -o week7.ps
	ps2pdf week7.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
