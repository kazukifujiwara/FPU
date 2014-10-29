EXES = fadd fmul fneg fsub finv fsqrt

all: $(EXES)

fadd: fadd.c fadd_main.c print.c
	gcc -Wall fadd.c fadd_main.c print.c -o fadd

fmul: fmul.c fmul_main.c print.c
	gcc -Wall fmul.c fmul_main.c print.c -o fmul

fneg: fneg.c fneg_main.c print.c
	gcc -Wall fneg.c fneg_main.c print.c -o fneg

fsub: fsub.c fsub_main.c print.c fadd.c fneg.c
	gcc -Wall fsub.c fadd.c fneg.c fsub_main.c print.c -o fsub

finv: finv.c finv_main.c print.c
	gcc -Wall finv.c finv_main.c print.c -o finv -lm

fsqrt: fsqrt.c fsqrt_main.c print.c
	gcc -Wall fsqrt.c fsqrt_main.c print.c -o fsqrt -lm

clean:
	rm -f $(EXES) *.o *~
