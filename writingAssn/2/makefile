all: writeup

writeup:
	latex writingAssn2.tex
	bibtex writingAssn2
	latex writingAssn2.tex
	latex writingAssn2.tex
	dvips -R -Poutline -t letter writingAssn2.dvi -o writingAssn2.ps
	ps2pdf writingAssn2.ps

clean:
	rm -f assignment1 *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
