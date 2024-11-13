all: clean compile link

compile:
	g++ -I src/include -c src/main.cpp -o obj/main.o -g
	g++ -I src/include -c src/interface.cpp -o obj/interface.o -g
	g++ -I src/include -c src/torre.cpp -o obj/torre.o -g

link:
	g++ \
	obj/main.o \
	obj/interface.o \
	obj/torre.o \
	-o hanoi -L src/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -g

clean:
	del /q obj\*.* hanoi.exe