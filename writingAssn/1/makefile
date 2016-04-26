all: writeup

writeup:
	latex writingAssn1.tex
	bibtex writingAssn1
	latex writingAssn1.tex
	latex writingAssn1.tex
	dvips -R -Poutline -t letter writingAssn1.dvi -o writingAssn1.ps
	ps2pdf writingAssn1.ps

clean:
	rm -f assignment1 *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps
