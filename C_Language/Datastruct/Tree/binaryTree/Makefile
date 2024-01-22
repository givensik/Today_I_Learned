
odBinaryTree.o : BinaryTree.h BinaryTree.c
    gcc -c BinaryTree.c

BinaryTreeMain.o : BinaryTreeMain.c
    gcc -c BinaryTreeMain.c 

main.out : BinaryTree.o BinaryTreeMain.o
    gcc BinaryTreeMain.o BinaryTree.o -o main.out
