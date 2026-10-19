#ifndef CHAR_DRIVER_IOCTL_H
#define CHAR_DRIVER_IOCTL_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define CHAR_DEVICE_MAGIC 'M'
#define CHAR_DEVICE_GET_LENGTH _IOR(CHAR_DEVICE_MAGIC,1,__u32)
#define CHAR_DEVICE_CLEAR_BUFFER _IO(CHAR_DEVICE_MAGIC,2)


#endif
