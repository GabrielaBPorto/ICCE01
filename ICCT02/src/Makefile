       PROG   = trab1

           CC = gcc # -std=c11
         OBJS = utils.o newton.o

       LIKWID = /home/soft/likwid
 LIKWID_FLAGS = -I$(LIKWID)/include -DLIKWID_PERFMON 
  LIKWID_LIBS = -L$(LIKWID)/lib -llikwid

    AVX_FLAGS = -O3 -mavx2 -march=native -fopt-info-vec # -fstrict-aliasing
    CFLAGS = -lm -g -I/usr/include -L/usr/lib -lmatheval

      #  CFLAGS = $(AVX_FLAGS) $(LIKWID_FLAGS)
       LFLAGS = $(LIKWID_LIBS)

.PHONY: all debug clean limpa purge faxina

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

all: likwid

debug:            CFLAGS += -g -DDEBUG
#avx:              CFLAGS += $(AVX_FLAGS)

avx likwid debug: $(PROG)

$(PROG) : % : $(OBJS) %.o
	$(CC) -o $@ $^ $(CFLAGS)
  #$(LFLAGS)

clean limpa:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge faxina:   clean
	@echo "Faxina ...."
	@rm -f  $(PROG) $(PROG_AUX) *.o $(OBJS) core a.out
	@rm -f *.png marker.out *.log
