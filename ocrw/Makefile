obj-m += ocrw.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

load:
	insmod ocrw.ko

unload:
	rmmod ocrw.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
