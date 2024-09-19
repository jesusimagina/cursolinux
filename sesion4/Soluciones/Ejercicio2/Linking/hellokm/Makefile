VLINUX = $(shell /bin/uname -r)
BUILDDIR = /lib/modules/$(VLINUX)/build


MODULE=hellokm
PWD=$(shell pwd)
obj-m += $(MODULE)_lkm.o
hellokm_lkm-objs := $(MODULE).o ../loggerkm/loggerkm.o

#obj-m := $(MODULE).o ../loggerkm/loggerkm.

all:
		make -C $(BUILDDIR) M=$(PWD) modules
clean:	
		make -C $(BUILDDIR) M=$(PWD) clean
