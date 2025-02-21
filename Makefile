all:
	g++ -Wall TimeCodeTests.cpp -o tct

run:
	./tct

try: all run