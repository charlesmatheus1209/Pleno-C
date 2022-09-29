all: FuncoesDebug.o PesquisaArquivos.o
	gcc -std=gnu11 FuncoesDebug.o PesquisaArquivos.o main.c -o main.exe
	make clean

FuncoesDebug.o: FuncoesDebug.h
	gcc -c FuncoesDebug.c

PesquisaArquivos.o: PesquisaArquivos.h
	gcc -c PesquisaArquivos.c

clean:
	del *.o

