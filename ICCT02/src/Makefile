       PROG   = principal

           CC = gcc # -std=c11
         OBJS = utils.o newton.o newtonOpt.o

       LIKWID = /home/soft/likwid
 LIKWID_FLAGS = -I$(LIKWID)/include -DLIKWID_PERFMON 
  LIKWID_LIBS = -L$(LIKWID)/lib -llikwid

    AVX_FLAGS = -O3 -mavx2 -march=native -fopt-info-vec # -fstrict-aliasing

      LFLAGS = $(LIKWID_LIBS)
      CFLAGS = $(LIKWID_FLAGS) 
      CFLAGS += -lm -g -I/usr/include -L/usr/lib -lmatheval
.PHONY: all debug clean limpa purge faxina

%.o: %.c %.h
	$(CC) -c $< -o $@ $(CFLAGS)

all: likwid avx

debug:            CFLAGS += -g -DDEBUG
avx:              CFLAGS += $(AVX_FLAGS)

avx likwid debug: $(PROG)

$(PROG) : % : $(OBJS) %.o
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)
  

clean limpa:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge faxina:   clean
	@echo "Faxina ...."
	@rm -f  $(PROG) $(PROG_AUX) *.o $(OBJS) core a.out
	@rm -f *.png marker.out *.log
