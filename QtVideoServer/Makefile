
OBJS=main.c camera.c convert.c srv.c tcp.c
CC=arm-none-linux-gnueabi-gcc
TARGET=camera

$(TARGET):$(OBJS)
	$(CC) $^ -o $@ -lpthread -ljpeg -L/home/farsight/jpeg_8/lib

clean:
	rm -rf $(TARGET)
	rm *~
