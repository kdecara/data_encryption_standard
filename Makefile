
CC = gcc
CFLAGS = -Wall -g -std=c99 -pedantic-errors

TARGET = des

all: $(TARGET)

$(TARGET): $(TARGET).c file_pack.c byte_pack.c key.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c key.c

clean:
	rm $(TARGET)

teste: $(TARGET).c file_pack.c byte_pack.c key.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c key.c
	./des tfile.txt out.enc 0xAAAAA e
	ls -l tfile.txt
	ls -l out.enc
	cat out.enc
	hexdump out.enc

testd: $(TARGET).c file_pack.c byte_pack.c key.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c key.c
	./des out.enc out.dec 0xAAAAA d
	ls -l tfile.txt
	ls -l out.dec
	cat out.dec
	hexdump tfile.txt
	hexdump out.enc
	hexdump out.dec

debug: $(TARGET).c file_pack.c byte_pack.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c
	gdb ./des tfile.txt out.enc 0xAAAAA e

val:
	valgrind --leak-check=yes ./des tfile.txt out.enc 0xAAAAA e
	valgrind --leak-check=yes ./des tfile.txt out.enc 0xAAAAA d
