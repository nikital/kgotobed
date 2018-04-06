.PHONY: modules clean
KDIR ?= /lib/modules/`uname -r`/build
modules:
	$(MAKE) -C $(KDIR) M=$$PWD/module modules
clean:
	$(MAKE) -C $(KDIR) M=$$PWD/module clean
