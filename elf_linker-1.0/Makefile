all: ELFRead

ELFRead: ELFRead.o partie1.o partie2.o partie3.o
	gcc -o $@ $^

ELFRead.o: ELFRead.c ELFRead.h partie1.h partie2.h partie3.h
	gcc -c $<

partie1.o: partie1.c partie1.h
	gcc -c $<

partie2.o: partie2.c partie2.h
	gcc -c $<

partie3.o: partie3.c partie3.h partie2.h
	gcc -c $<

clean:
	rm *.o ELFRead
