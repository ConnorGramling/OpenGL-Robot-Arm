LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

main: main.c initShader.o myLib.o init.o perspFunc.o lighting.o sun.o
	gcc -o main main.c initShader.o myLib.o init.o perspFunc.o lighting.o sun.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

myLib.o: myLib.h myLib.c
	gcc -c myLib.c

init.o: init.c init.h
	gcc -c init.c
	
presp_func.o: perspFunc.c perspFunc.h
	gcc -c perspFunc.c

lighting.o: lighting.c lighting.h
	gcc -c lighting.c

sun.o: sun.c sun.h
	gcc -c sun.c