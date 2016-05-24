all: writeup

writeup:
	latex week8.tex
	dvips -R -Poutline -t letter week8.dvi -o week8.ps
	ps2pdf week8.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
