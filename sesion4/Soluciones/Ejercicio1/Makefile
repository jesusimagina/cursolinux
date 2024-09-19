
VLINUX = $(shell /bin/uname -r)
BUILDDIR = /lib/modules/$(VLINUX)/build

MODULE=hellokm
PWD=$(shell pwd)
obj-m := $(MODULE).o

all:
	make -C $(BUILDDIR) M=$(PWD) modules
clean:
	make -C $(BUILDDIR) M=$(PWD) clean

