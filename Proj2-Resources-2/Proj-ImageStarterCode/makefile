OBJS = main.o image.o pixel.o
CC = g++
CFLAGS = -Wall -g -c
LFLAGS = -Wall -g

all: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o imageManipulate 

main.o: main.cpp image.h stb_image.h stb_image_write.h
	$(CC) $(CFLAGS) main.cpp

image.o: image.cpp image.h
	$(CC) $(CFLAGS) image.cpp

pixel.o: pixel.cpp pixel.h
	$(CC) $(CFLAGS) pixel.cpp

clean:
	rm *.o imageManipulate
