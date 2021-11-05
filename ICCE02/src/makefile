CFLAGS = -lm -I/usr/include -L/usr/lib -lmatheval
CXX=cc -std=c99

ep02: ep02.c utils.h utils.c sl.h sl.c
	$(CXX) utils.h utils.c sl.h sl.c ep02.c -o ep02 $(CFLAGS)

clean:
	rm -f ep02 
