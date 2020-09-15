all:
	g++ -o vardelays pipelineprocessor.cpp

run:
	rm -f vardelay.txt
	touch vardelay.txt
	@while read -r file; do \
		./vardelays $$file; \
	done < tests1.txt
	gnuplot gnuplot1.plt

run2:
	rm -f vardelay.txt
	touch vardelay.txt
	@while read -r file; do \
		./vardelays $$file; \
	done < tests2.txt
	gnuplot gnuplot2.plt

clean:
	rm -f vardelay.txt plot1.pdf plot2.pdf vardelays eachCycle*
