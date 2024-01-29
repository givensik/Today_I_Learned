BinaryTree3.o: BinaryTree3.c BinaryTree3.h
	gcc -c BinaryTree3.c
BinarySearchTree.o: BinarySearchTree.c BinarySearchTree.h
	gcc -c BinarySearchTree.c
BinarySearchTree2.o: BinarySearchTree2.c BinarySearchTree2.h
	gcc -c BinarySearchTree2.c
BinarySearchTreeMain.o: BinarySearchTreeMain.c
	gcc -c BinarySearchTreeMain.c
BinarySearchTreeDelMain.o: BinarySearchTreeDelMain.c
	gcc -c BinarySearchTreeDelMain.c
BinarySearchTreeMain.out: BinaryTree2.o BinarySearchTree.o BinarySearchTreeMain.o
	gcc -o BinarySearchTreeMain.out BinaryTree2.o BinarySearchTree.o BinarySearchTreeMain.o
BinarySearchTreeDelMain.out: BinaryTree3.o BinarySearchTree2.o BinarySearchTreeDelMain.o
	gcc -o BinarySearchTreeDelMain.out BinaryTree3.o BinarySearchTree2.o BinarySearchTreeDelMain.o
clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf *.out