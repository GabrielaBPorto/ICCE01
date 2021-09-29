CFLAGS = -lm -I/usr/include -L/usr/lib -lmatheval
CXX=cc -std=c99

analiseIntervalar: src/intervalo.c src/intervalo.h src/analiseIntervalar.c  
	$(CXX) ep01.c $(CFLAGS)

clean:
	rm -f analiseIntervalar 
