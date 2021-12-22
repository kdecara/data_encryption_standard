
CC = gcc
CFLAGS = -Wall -g -std=c89 -pedantic-errors

TARGET = des

all: $(TARGET)

$(TARGET): $(TARGET).c file_pack.c byte_pack.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c

clean:
	rm $(TARGET)

teste: $(TARGET).c file_pack.c byte_pack.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c
	./des tfile.txt out.enc 5 e
	ls -l tfile.txt
	ls -l out.enc
	cat out.enc
	hexdump out.enc

testd: $(TARGET).c file_pack.c byte_pack.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c
	./des out.enc out.dec 5 d
	ls -l tfile.txt
	ls -l out.dec
	cat out.dec
	hexdump tfile.txt
	hexdump out.enc
	hexdump out.dec

debug: $(TARGET).c file_pack.c byte_pack.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c
	gdb ./des tfile.txt out.enc 5 e

val:
	valgrind --leak-check=yes ./des tfile.txt out.enc 5 e
