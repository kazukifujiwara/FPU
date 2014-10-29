EXES = fadd fmul fneg fsub finv fsqrt
LIBS = def.o print.o
CC = gcc
CFLAGS = -Wall
LD = gcc
LDFLAGS = -lm

all: $(EXES)

fadd: fadd_main.o fadd.o $(LIBS)

fmul: fmul_main.o fmul.o $(LIBS)

fneg: fneg_main.o fneg.o $(LIBS)

fsub: fsub_main.o fsub.o fadd.o fneg.o $(LIBS)

finv: finv_main.o finv.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

fsqrt: fsqrt_main.o fsqrt.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXES) *.o *~

.PHONY: all clean
