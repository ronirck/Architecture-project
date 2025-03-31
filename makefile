CC = g++
CFLAGS = -L/usr/local/lib -L/usr/lib -lncurses -llmdb -lcryptopp
SRC = main.cpp \
      Librerias/Custom/src/interface.cpp \
      Librerias/Custom/src/parteMedica.cpp \
      Librerias/Custom/src/parteSistema.cpp \
      Librerias/Custom/src/loginAndJoin.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
