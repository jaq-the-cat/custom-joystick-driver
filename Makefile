obj-m += custom-joystick-driver.o

KERNEL ?= $(shell uname -r)
KERNELDIR := /usr/src/kernels/$(KERNEL)
SRC     := $(wildcard src/*.c)
CC      := clang
CFLAGS  := -Iheaders -I$(KERNELDIR)/include
CFLAGS  += -Wall -O2 -std=gnu11 -D__KERNEL__ -DMODULE

.PHONY: dev clean compile cnr

dev:
	echo $(CFLAGS) | tr " " "\n" > compile_flags.txt
	echo $(LDFLAGS) | tr " " "\n" >> compile_flags.txt

module:
	$(MAKE) -C $(KERNELDIR) V=1 SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) SUBDIRS=$(PWD) clean

#compile: $(SRC)
	#$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out

#cnr: $(SRC)
	#$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out
	#./a.out
	#$(RM) *.o a.out
