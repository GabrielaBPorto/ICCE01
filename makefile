CFLAGS = -g -Wall -O3 -lm
CXX=gcc -std=c99

principal: src/intervalo.c src/intervalo.h src/operacao.c src/operacao.h src/principal.c  
	$(CXX) -o principal src/principal.c src/intervalo.c src/intervalo.h src/operacao.c src/operacao.h $(CFLAGS)

clean:
	rm -f principal 
