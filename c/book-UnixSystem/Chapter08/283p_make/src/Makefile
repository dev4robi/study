CFLAGS = -c -O -I../include
LFLAGS = 

# Object file list
TARGET = main
OBJS = main.o subA.o subB.o subAa.o

.c.0:
		gcc $(CFLAGS) $<

# Target
all : $(OBJS)
		gcc -o $(TARGET) $(OBJS) $(LFLAGS)

clean :
		rm $(OBJS)
		rm $(TARGET)