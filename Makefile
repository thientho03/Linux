KDIR = /lib/modules/`uname -r`/build

all: test
	make -C $(KDIR) M=`pwd`
	
clean:
	make -C $(KDIR) M=`pwd` clean
	rm test
	
test:
	cc -o test test.c

in: 
	sudo insmod random_number.ko

out:
	sudo rmmod random_number

admin:
	sudo chmod 666 /dev/Random_dev
