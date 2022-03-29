 	obj-m := main.o
	KERN_DIR ?= /usr/src/linux-headers-5.13.0-35-generic/
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order *.mod

