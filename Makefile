.PHONY: modules clean install uninstall
KDIR ?= /lib/modules/`uname -r`/build
modules:
	$(MAKE) -C $(KDIR) M=$$PWD/module modules
clean:
	$(MAKE) -C $(KDIR) M=$$PWD/module clean

install:
	dkms install module
	cp gotobed /usr/local/bin
	cp kgotobed.service /etc/systemd/system
	systemctl daemon-reload
	systemctl enable kgotobed.service
	systemctl start kgotobed.service
uninstall:
	-dkms remove kgotobed/1.0 --all
	-rm -f /usr/local/bin/gotobed
	-rm -f /etc/systemd/system/kgotobed.service
	-systemctl daemon-reload
