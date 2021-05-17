CFLAGS = -g -Wall -O3 -lm
CXX=gcc -std=c99

analiseIntervalar: intervalo.c intervalo.h analiseIntervalar.c  
	$(CXX) -o analiseIntervalar analiseIntervalar.c intervalo.c intervalo.h $(CFLAGS)

clean:
	rm -f analiseIntervalar 
