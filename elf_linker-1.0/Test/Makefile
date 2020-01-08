all: testPartie1 testPartie2 testPartie3 testPartie4 testPartie5

testPartie1.o: testPartie1.c ../partie1.h
	-gcc -c $<

testPartie1: testPartie1.o ../partie1.o
	-gcc -o $@ $^

testPartie2.o: testPartie2.c ../partie1.h ../partie2.h
	-gcc -c $<

testPartie2: testPartie2.o ../partie1.o ../partie2.o
	-gcc -o $@ $^

testPartie3.o: testPartie3.c ../partie1.h ../partie2.h ../partie3.h
	-gcc -c $<

testPartie3: testPartie3.o ../partie1.o ../partie2.o ../partie3.o
	-gcc -o $@ $^

testPartie4.o: testPartie4.c ../partie1.h ../partie2.h ../partie4.h
	-gcc -c $<

testPartie4: testPartie4.o ../partie1.o ../partie2.o ../partie4.o
	-gcc -o $@ $^

testPartie5.o: testPartie5.c ../partie1.h ../partie2.h  ../partie3.h ../partie4.h  ../partie5.h
	-gcc -c $<

testPartie5: testPartie5.o ../partie1.o ../partie2.o ../partie3.o ../partie4.o  ../partie5.o
	-gcc -o $@ $^

clean:
	rm *.o testPartie1 testPartie2 testPartie3 testPartie4 testPartie5
