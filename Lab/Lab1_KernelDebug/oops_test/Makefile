oops-obj:=oops_test.o
obj-m:=oops_test.o
MY_CFLAGS += -g -DDEBUG

all:
	make -C /lib/modules/4.19.23/build M=$(PWD) modules;

debug:
	make -C /lib/modules/4.19.23/build M=$(PWD) modules;
	EXTRA_CFLAGS="$(MY_CFLAGS)";

clean:
	make -C /lib/modules/4.19.23/build M=$(PWD) clean;
