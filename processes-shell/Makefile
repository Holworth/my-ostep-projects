SRC = wish.h wish.c
TEST_SRC = main.c
TARGET = wish

$(TARGET): $(SRC) $(TEST_SRC)
	gcc -g wish.c main.c -o $(TARGET)
clean:
	rm -f *.o $(TARGET)
