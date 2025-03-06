all:
	g++ -g -Wall TimeCodeTests.cpp -o tct
	g++ -g -Wall NasaLaunchAnalysis.cpp -o nasa
	g++ -g -Wall PaintDryTimer.cpp -o pdt

run:
	./tct

try: all run