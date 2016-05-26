all: writeup

writeup:
	latex writingAssn4.tex
	bibtex writingAssn4
	latex writingAssn4.tex
	latex writingAssn4.tex
	dvips -R -Poutline -t letter writingAssn4.dvi -o writingAssn4.ps
	ps2pdf writingAssn4.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
