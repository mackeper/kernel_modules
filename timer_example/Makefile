obj-m += timer.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

load:
	insmod timer.ko

unload:
	rmmod timer.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
