CFLAGS = -lm -g -I/usr/include -L/usr/lib -lmatheval
CXX=cc -std=c99

all: trab1.c utils.h utils.c newton.h newton.c
	$(CXX) utils.h utils.c newton.h newton.c trab1.c -o newtonSNL $(CFLAGS)

clean:
	rm -f trab1 
