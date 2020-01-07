all: ELFRead ELFFusion

ELFFusion: ELFFusion.o partie1.o partie2.o partie3.o partie4.o partie5.o
	gcc -o $@ $^

ELFFusion.o: ELFFusion.c partie1.h partie2.h partie3.h partie4.h partie5.h
	gcc -c $<

ELFRead: ELFRead.o partie1.o partie2.o partie3.o partie4.o partie5.o
	gcc -o $@ $^

ELFRead.o: ELFRead.c ELFRead.h partie1.h partie2.h partie3.h partie4.h partie5.h
	gcc -c $<

partie1.o: partie1.c partie1.h
	gcc -c $<

partie2.o: partie2.c partie2.h
	gcc -c $<

partie3.o: partie3.c partie3.h partie2.h
	gcc -c $<

partie4.o: partie4.c partie4.h
	gcc -c $<

partie5.o: partie5.c partie5.h partie3.h partie2.h
	gcc -c $<

clean:
	rm *.o ELFRead
