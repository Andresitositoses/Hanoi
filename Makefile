# Compiler and flags
CXX = g++
CXXFLAGS = -I src/include -g
LDFLAGS = -L src/lib

# Librerías estáticas
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lportaudio_x64 -lfftw3

# Archivos fuente y objetos
SRCS = src/main.cpp \
       src/interface.cpp \
       src/torre.cpp \
       src/MenuState.cpp \
       src/GameState.cpp \
       src/EndState.cpp

OBJS = $(SRCS:src/%.cpp=obj/%.o)

# Ejecutable
TARGET = hanoi

# Objetivo principal
all: clean $(TARGET)

# Enlace
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS) -g

# Regla de compilación
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar
clean:
	del /q obj\*.* $(TARGET).exe
