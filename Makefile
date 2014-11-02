EXES = fadd fmul fneg fsub finv fsqrt
TESTS = test_finv test_fmul
LIBS = def.o print.o
CC = gcc
CFLAGS = -Wall
LD = gcc
LDFLAGS = -lm

all: $(EXES) $(TESTS)

fadd: fadd_main.o fadd.o $(LIBS)

fmul: fmul_main.o fmul.o $(LIBS)

fneg: fneg_main.o fneg.o $(LIBS)

fsub: fsub_main.o fsub.o fadd.o fneg.o $(LIBS)

finv: finv_main.o finv.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

fsqrt: fsqrt_main.o fsqrt.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_finv: test_finv.o finv.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_fmul: test_fmul.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXES) $(TESTS) *.o *~

.PHONY: all clean
