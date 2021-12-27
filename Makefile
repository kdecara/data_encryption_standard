
#define variables to refer to respective directories
SRCDIR = ./src
#OBJDIR = ./obj
#BINDIR = ./bin
INCDIR = ./inc

#explicitly define the SRCS variable, which consists of all of our source files
SRCS = $(SRCDIR)/block.c $(SRCDIR)/byte_pack.c $(SRCDIR)/des.c $(SRCDIR)/file_pack.c $(SRCDIR)/key.c

#define the INCLUDE variable, which consists of all of our header files
INCLUDE = $(addprefix -I, $(INCDIR))
#define the OBJS variable from the SRCS variable
#OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJS = $(SRCS:$(SRCDIR)/%.c=%.o)

CC = gcc
#CFLAGS variable, note that $(INCLUDE) is in here, so we don't need to declare in the recipes below
CFLAGS = $(INCLUDE) -Wall -g -std=c99 -pedantic-errors

#our file executable will be name des
#TARGET = $(BINDIR)/des

TARGET = des

#make will go to this target when called
all: $(TARGET)

#Recipe for the target, compile from OBJS variable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) 

#compile each OBJS variable from its respective source file and header file
#$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c 
#	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): %.o : $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

#clean will remove the compiled object files and executable
clean:
	rm -f $(OBJS)
	rm des

#the executable must be compiled from each object file
#each object file must be compiled with its equivalent source file and header file as arguments (header included
#in source files)
#each source file must be compiled with its equivalent header file as arguments (perhaps not needed???)

#teste: $(TARGET).c file_pack.c byte_pack.c key.c
#	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c key.c
#	./des tfile.txt out.enc 0xAAAAA e
#	ls -l tfile.txt
#	ls -l out.enc
#	cat out.enc
#	hexdump out.enc

#testd: $(TARGET).c file_pack.c byte_pack.c key.c
#	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c key.c
#	./des out.enc out.dec 0xAAAAA d
#	ls -l tfile.txt
#	ls -l out.dec
#	cat out.dec
#	hexdump tfile.txt
#	hexdump out.enc
#	hexdump out.dec

#debug: $(TARGET).c file_pack.c byte_pack.c
#	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c file_pack.c block.c byte_pack.c
#	gdb ./des tfile.txt out.enc 0xAAAAA e

#val:
#	valgrind --leak-check=yes ./des tfile.txt out.enc 0xAAAAA e
#	valgrind --leak-check=yes ./des tfile.txt out.enc 0xAAAAA d
