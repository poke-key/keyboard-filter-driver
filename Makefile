obj-m += kbd_filter.o
kbd_filter-objs := src/kbd_filter.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

install:
	sudo insmod kbd_filter.ko

uninstall:
	sudo rmmod kbd_filter