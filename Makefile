EXES = fadd fmul fneg fsub

all: $(EXES)

fadd: fadd.c fadd_main.c print.c
	gcc -Wall fadd.c fadd_main.c print.c -o fadd

fmul: fmul.c fmul_main.c print.c
	gcc -Wall fmul.c fmul_main.c print.c -o fmul

fneg: fneg.c fneg_main.c print.c
	gcc -Wall fneg.c fneg_main.c print.c -o fneg

fsub: fsub.c fsub_main.c print.c fadd.c fneg.c
	gcc -Wall fsub.c fadd.c fneg.c fsub_main.c print.c -o fsub

clean:
	rm -f $(EXES) *.o *~
