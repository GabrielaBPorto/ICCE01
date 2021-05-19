CFLAGS = -g -Wall -O3 -lm
CXX=gcc -std=c99

principal: intervalo.c intervalo.h operacao.c operacao.h principal.c  
	$(CXX) -o principal principal.c intervalo.c intervalo.h operacao.c operacao.h $(CFLAGS)

clean:
	rm -f principal 
