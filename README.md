OPERATING SYSTEM - LINUX KERNEL MODULE

----------------------------------------

Mục tiêu hiểu về Linux kernel module và hệ thống quản lý file và device trong linux,
giao tiếp giữa tiến trình ở user space và code kernel space

+ Viết một module dùng để tạo ra số ngẫu nhiên.

+ Module này sẽ tạo một character device để cho phép các tiến trình ở userspace
có thể open và read các số ngẫu nhiên.
Viết một module dùng để tạo ra số ngẫu nhiên.
Module này sẽ tạo một character device để cho phép các tiến trình ở userspace có thể open và read các số ngẫu nhiên.

----------------------------------------

Cách thực hiện:
	
	make
	make in
	make test
	make admin
	./test
	make out

Các lệnh được rút ngắn: 
	
	make in = sudo insmod character_driver.ko 
	make out = sudo rmmod character_driver
	make test = cc -o test test.c
	make admin = sudo chmod 666 /dev/vchar_dev

----------------------------------------

Reference:
https://vimentor.com/vi/lesson/linux-kernel-module
