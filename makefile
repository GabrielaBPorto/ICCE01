CFLAGS = -g -Wall -O3

analiseIntervalar:analiseIntervalar.c
	gcc -o analiseIntervalar analiseIntervalar.c $(CFLAGS)

clean:
	rm -f analiseIntervalar 
