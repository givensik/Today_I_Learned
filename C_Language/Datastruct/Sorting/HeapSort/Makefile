HeapSort.o: HeapSort.c
	gcc -c HeapSort.c
UsefulHeap.o: UsefulHeap.c
	gcc -c UsefulHeap.c UsefulHeap.h
HeapSort.out: HeapSort.o UsefulHeap.o
	gcc -o HeapSort.out HeapSort.o UsefulHeap.o

clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf *.out