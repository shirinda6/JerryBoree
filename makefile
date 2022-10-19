prog: HashTable.o Jerry.o KeyValuePair.o LinkedList.o JerryBoreeMain.o MultiValueHashTable.o
	gcc HashTable.o Jerry.o KeyValuePair.o LinkedList.o JerryBoreeMain.o MultiValueHashTable.o -o JerryBoree

HashTable.o: HashTable.c HashTable.h KeyValuePair.h LinkedList.h Defs.h
	gcc -c HashTable.c

Jerry.o: Jerry.c Jerry.h Defs.h
	gcc -c Jerry.c

KeyValuePair.o: KeyValuePair.c KeyValuePair.h Defs.h
	gcc -c KeyValuePair.c

LinkedList.o: LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c

JerryBoreeMain.o: JerryBoreeMain.c HashTable.h Jerry.h KeyValuePair.h LinkedList.h MultiValueHashTable.h Defs.h
	gcc -c JerryBoreeMain.c
	
MultiValueHashTable.o: MultiValueHashTable.h HashTable.h KeyValuePair.h LinkedList.h Defs.h
	gcc -c MultiValueHashTable.c

clean:
	rm -f *.o JerryBoree
