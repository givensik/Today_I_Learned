### make 사용법

make를 통해 만들 명령어 : 필요한 파일
	명령어를 치면 실행할 명령어들

(ex)
UsefulHeap.out: UsefulHeap.o UsefulHeapMain.o
	gcc -o UsefulHeap.out UsefulHeap.o UsefulHeapMain.o

이런식으로 만들고 make 명령어를 치면 실행됨
근데 vscode에서 탭키를 누르면 \t로 인식이 안되는거 같음.. 그래서 이 파일의 양식을 복사하면 \t가 같이 복사돼서 사용 가능함..