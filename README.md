# CharacterDeviceDriver

Implemented a Pipe Using Character Device in kernel module that acts as a pipe for multiple producers and consumers. Handled concurrency, synchronization and used various kernel primitives to achieve this.

This is a classical producer-consumer problem. A character device must maintain a FIFO queue (i.e. a pipe) of maximum N bytes (configured as a module parameter).
1. In user space, there are several concurrent processes that consume and produce bytes.
2. Producers produce bytes and write them to a character device (say /dev/mypipe).
3. Consumers read the bytes from the character device and output them to a file.
4. When the pipe is full, i.e. when there are N bytes stored in character device, then any  producer trying to write will block.
5. When the pipe is empty, i.e. when there are 0 bytes stored in character device, then any consumer trying to read will block.
6. When a consumer reads from a (full) pipe, it wakes up all blocked producers. In this case, no blocked consumer should be woken up.
7. When a producer writes to a (empty) pipe, it wakes up all blocked consumers. In this case, no blocked producer should be woken up.
8. Consumers may read a different number of bytes than what producers produce each time. For example, a producer may first write 10 bytes then 20 bytes, whereas a consumer may read 5 bytes, 15 bytes, and 5 bytes. Your implementation should work correctly for arbitrary byte boundaries.


Steps to run: 

1)To create .ko of the module:
	make

2)Install the module in kernel:
	sudo insmod project3.ko

3)change the mypipe permission
 go to /dev/
 then on terminal write:
 sudo chmod 777 mypipe

4)Compile the producer:
	make producer 


5)Compile the consumer:
	make consumer 

6)Compile the producer and consumer forking child process which write the string to buffer:
	make fork  
 
7)Run the producer:
	./prod <number of bytes>
	eg:  ./prod 5

8)Run the consumer:
	./cons <number of bytes> 
	eg:  ./cons 5
9)Run the multiple producer processes - number denotes the number of child processed to be forked
	./prodf 2
10)Run the multiple producer processes - number denotes the number of child processed to be forked
	./consf 2

9)to check the log:
	cd /var/log/
	dmesg

