CFLAGS = -g -Wall -O3 -lm
CXX=gcc -std=c99

analiseIntervalar:analiseIntervalar.c
	$(CXX) -o analiseIntervalar analiseIntervalar.c $(CFLAGS)

clean:
	rm -f analiseIntervalar 
