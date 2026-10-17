# Character Device Driver For Linux

a Linux Driver that handles a virtual character device exposed at /dev/char_device.
Written in c using linux kernel headers, it provides the following operations:

## Setup

1) clone the repository using git

```
git clone https:///www.github.com/manoj-marimuthu/char-device-driver.git
cd char-device-driver
```

2) besure to have make and the kernel header files along with a compiler.

```
make
```

3) load the kernel module using:

```
sudo insmod char_driver.ko
```

You now have the character device driver loaded.

## Operations

### open

The device on open, logs to kernel log. Usually invoked during
read operation and write operation given below. You can inspect
the kernel log using

```
sudo dmesg | tail -10
```

During a read/write operation, the driver also logs that the character
device has been opened.

### read

Prints the stored buffer held in the kernel space. Test it using

```
sudo cat /dev/char_device
```

### write

Writes string to the buffer in the kernel space.

```
echo "hello world" | sudo tee /dev/char_device
```

The future commits will address more character device driver features.

### Author

Manoj K M 
