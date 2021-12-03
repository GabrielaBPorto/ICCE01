       PROG   = gaussJacobi-likwid
     PROG_AUX = geraSL

           CC = gcc # -std=c11
         OBJS = gaussJacobi.o gaussJacobiOpt.o utils.o

       LIKWID = /home/soft/likwid
 LIKWID_FLAGS = -I$(LIKWID)/include -DLIKWID_PERFMON 
  LIKWID_LIBS = -L$(LIKWID)/lib -llikwid

    AVX_FLAGS = -O3 -mavx2 -march=native -fopt-info-vec # -fstrict-aliasing

       CFLAGS = $(LIKWID_FLAGS) 
       LFLAGS = $(LIKWID_LIBS) -lm

.PHONY: all debug clean limpa purge faxina

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

all: likwid

debug:            CFLAGS += -g -DDEBUG
avx:              CFLAGS += $(AVX_FLAGS)

avx likwid debug: $(PROG)

$(PROG_AUX).o: $(PROG_AUX).c
	$(CC) -c $< -o $@

$(PROG) : % : $(OBJS) %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(PROG_AUX) : $(PROG_AUX).o
	$(CC) -o $@ $^ -lm 

clean limpa:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge faxina:   clean
	@echo "Faxina ...."
	@rm -f  $(PROG) $(PROG_AUX) *.o $(OBJS) core a.out
	@rm -f *.png marker.out *.log
