all:
	g++ -Wall TimeCodeTests.cpp -o tct
	g++ -g -Wall TimeCode.cpp NasaLaunchAnalysis.cpp -o nasa
	# g++ -g -Wall TimeCode.cpp PaintDryTimer.cpp -o pdt

run:
	./tct

try: all run