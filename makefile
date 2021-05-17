CFLAGS = -g -Wall -O3 -lm
CXX=gcc -std=c99

analiseIntervalar: src/intervalo.c src/intervalo.h src/floatType.h src/floatType.c src/analiseIntervalar.c  
	$(CXX) -o analiseIntervalar src/analiseIntervalar.c src/intervalo.c src/intervalo.h $(CFLAGS)

clean:
	rm -f analiseIntervalar 
