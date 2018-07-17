test.exe : genLines.o findPatterns.o task.cpp
	g++ -o test.exe task.cpp genLines.o findPatterns.o
genLines.o : genLines.h genLines.cpp
	g++ -c -o genLines.o genLines.cpp
findPatterns.o : findPatterns.h findPatterns.cpp
	g++ -c -o findPatterns.o findPatterns.cpp
clean :
	rm test.exe genLines.o findPatterns.o patterns.txt errors.txt
