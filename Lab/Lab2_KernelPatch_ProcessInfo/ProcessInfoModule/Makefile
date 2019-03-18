ifneq	($(KERNELRELEASE),)
obj-m	:= tasklist.o 

else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)
default:	
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules 
	rm -r -f .tmp_versions *.mod.c .*.cmd *.o *.symvers 
	sudo insmod tasklist.ko
	dmesg | tail

endif

remove:
	sudo rmmod tasklist.ko
	dmesg | tail