all: writeup

writeup:
	latex week6.tex
	dvips -R -Poutline -t letter week6.dvi -o week6.ps
	ps2pdf week6.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
