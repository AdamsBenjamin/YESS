CC = gcc -Wall -std=c99

yess: main.o registers.o writebackStage.o memory.o decodeStage.o\
memoryStage.o executeStage.o  fetchStage.o dump.o tools.o
	$(CC) main.o registers.o writebackStage.o memory.o decodeStage.o\
	 memoryStage.o executeStage.o fetchStage.o dump.o tools.o efbloader -o yess

main.o: main.c instructions.h forwarding.h status.h

registers.o: registers.c registers.h

writebackStage: writebackStage.c writebackStage.h instructions.h forwarding.h status.h

executeStage.o: executeStage.c executeStage.h instructions.h forwarding.h status.h

memory.o: memory.c memory.h

decodeStage.o: decodeStage.c decodeStage.h instructions.h forwarding.h

memoryStage.o: memoryStage.c memoryStage.h instructions.h forwarding.h status.h

fetchStage.o: fetchStage.c fetchStage.h

dump.o: dump.c dump.h

tools.o: tools.c tools.h

clean:
	 rm -f *.o
