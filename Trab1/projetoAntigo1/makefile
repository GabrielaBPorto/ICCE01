CFLAGS = -g -Wall -O0 -lm
CXX=gcc -std=c99

analiseIntervalar: src/intervalo.c src/intervalo.h src/analiseIntervalar.c  
	$(CXX) -o analiseIntervalar src/analiseIntervalar.c src/intervalo.c src/intervalo.h $(CFLAGS)

clean:
	rm -f analiseIntervalar 
