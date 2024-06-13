obj-m += JoysticSimulModule.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR)  M=$(shell pwd) modules

mod:
	rmmod JoysticSimulModule.ko
	make -C $(KDIR) M=$(shell pwd) modules
	insmod JoysticSimulModule.ko
modprog:
	rmmod JoysticSimulModule.ko
	make -C $(KDIR) M=$(shell pwd) modules
	insmod JoysticSimulModule.ko
	gcc -o JoysticSimulUserProg JoysticSimulUserProg.c
	./JoysticSimulUserProg
prog:
	gcc -o JoysticSimulUserProg JoysticSimulUserProg.c
	./JoysticSimulUserProg
ins:
	make -C $(KDIR) M=$(shell pwd) modules
	insmod JoysticSimulModule.ko
rm:
	rmmod JoysticSimulModule.ko

clean:
	make -C $(KDIR)  M=$(shell pwd) clean
