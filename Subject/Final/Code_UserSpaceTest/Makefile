CC = gcc
CFLAGS := -g

test.out: test.o BCM2837_GPIO.o seven_seg.o
	$(CC) $(CFLAGS) -o test.out test.o BCM2837_GPIO.o seven_seg.o

test.o: test/test.c header/BCM2837_GPIO.h
	$(CC) $(CFLAGS) -c test/test.c

BCM2837_GPIO.o: gpio/BCM2837_GPIO.c header/BCM2837_GPIO.h
	$(CC) $(CFLAGS) -c gpio/BCM2837_GPIO.c

seven_seg.o: gpio/seven_seg.c header/seven_seg.h
	$(CC) $(CFLAGS) -c gpio/seven_seg.c

clean:
	rm -rf *.o *.out
