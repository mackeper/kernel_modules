obj-m += kthread_ex.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

load:
	insmod kthread_ex.ko

unload:
	rmmod kthread_ex.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
