all: clean compile link

compile:
	g++ -I src/include -c src/main.cpp -o obj/main.o -g
	g++ -I src/include -c src/interface.cpp -o obj/interface.o -g
	g++ -I src/include -c src/torre.cpp -o obj/torre.o -g
	g++ -I src/include -c src/MenuState.cpp -o obj/MenuState.o -g
	g++ -I src/include -c src/GameState.cpp -o obj/GameState.o -g
	g++ -I src/include -c src/EndState.cpp -o obj/EndState.o -g

link:
	g++ \
	obj/main.o \
	obj/interface.o \
	obj/torre.o \
	obj/MenuState.o \
	obj/GameState.o \
	obj/EndState.o \
	-o hanoi -L src/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -g

clean:
	del /q obj\*.* hanoi.exe