BinaryTree3.o: BinaryTree3.c BinaryTree3.h
	gcc -c BinaryTree2.c
BinarySearchTree.o: BinarySearchTree.c BinarySearchTree.h
	gcc -c BinarySearchTree.c
BinarySearchTreeMain.o: BinarySearchTreeMain.c
	gcc -c BinarySearchTreeMain.c
BinarySearchTreeMain.out: BinaryTree2.o BinarySearchTree.o BinarySearchTreeMain.o
	gcc -o BinarySearchTreeMain.out BinaryTree2.o BinarySearchTree.o BinarySearchTreeMain.o
QuickSort.out: QuickSort.c
	gcc -o QuickSort.out QuickSort.c
clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf *.out