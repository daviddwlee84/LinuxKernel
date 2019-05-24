ifeq ($(KERNELRELEASE),)

	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

modules:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

.PHONY: modules clean

else

	# called from kernel build system: just declare what our modules are
	obj-m := rpi_7seg.o

	rpi_7seg-objs := rpi_7seg_module.o seven_seg.o

endif
