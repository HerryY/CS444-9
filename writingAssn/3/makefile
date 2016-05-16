all: writeup

writeup:
	latex writingAssn3.tex
	bibtex writingAssn3
	latex writingAssn3.tex
	latex writingAssn3.tex
	dvips -R -Poutline -t letter writingAssn3.dvi -o writingAssn3.ps
	ps2pdf writingAssn3.ps

clean:
	rm -f assignment1 *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
