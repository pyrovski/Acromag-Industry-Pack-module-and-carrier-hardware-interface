
NAMES := dev8620 isr408 isr1k110 isr400 isr409 isr482 isr330 isr340 isr470 isr480
TARGET := apc8620
MODULE := $(TARGET).ko
OBJS  := $(addsuffix .o, $(NAMES))

ifneq ($(KERNELRELEASE),)
obj-m := $(TARGET).o

$(TARGET)-objs := $(OBJS)

else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install: default
	install -m 0755 -d /lib/modules/$(shell uname -r)/extra/
	install -m 0644 $(MODULE) /lib/modules/$(shell uname -r)/extra/
	/sbin/depmod -A
endif

clean:
	rm -f ./*.o ./*~ ./*.ko ./.*o.cmd
