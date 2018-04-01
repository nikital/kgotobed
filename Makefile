ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m  := kgotobed.o

else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build
modules:
	$(MAKE) -C $(KDIR) M=$$PWD modules
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

endif
