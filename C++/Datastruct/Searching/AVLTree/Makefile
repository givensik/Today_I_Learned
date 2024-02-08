BinaryTree3.o: BinaryTree3.c BinaryTree3.h
	gcc -c BinaryTree3.c
BinarySearchTree3.o: BinarySearchTree3.c BinarySearchTree3.h
	gcc -c BinarySearchTree3.c
AVLRebalance.o: AVLRebalance.c
	gcc -c AVLRebalance.c
AVLTreeMain.o: AVLTreeMain.c
	gcc -c AVLTreeMain.c
AVLTreeMain.out: BinaryTree3.o BinarySearchTree3.o AVLRebalance.o AVLTreeMain.o
	gcc -o AVLTreeMain.out BinaryTree3.o BinarySearchTree3.o AVLTreeMain.o AVLRebalance.o
BinarySearchTreeDelMain.out: BinaryTree3.o BinarySearchTree2.o BinarySearchTreeDelMain.o
	gcc -o BinarySearchTreeDelMain.out BinaryTree3.o BinarySearchTree3.o BinarySearchTreeDelMain.o AVLRebalance.o
clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf *.out