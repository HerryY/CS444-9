all: writeup

writeup:
	latex week5.tex
	dvips -R -Poutline -t letter week5.dvi -o week5.ps
	ps2pdf week5.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
